#include "filesystem.h"
#include "sata.h"

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS SetupFilesystem(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type){
    if (diskDevice->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCI;
    }

    switch (diskDevice->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return SetupSataFilesystem(diskDevice, partitionIndex, type);
        default:
            return BT_IO_INVALID_PCI;
    } 
}

// ==================================== |
//             SATA_SPECIFIC            |
// ==================================== |

BT_STATUS SetupSataFilesystem(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type){
    switch (diskDevice->header.common.pi)
    {
        case PCI_PI_0x01_0x06_CAHCI: 
            return SetupSataFilesystemAhci(diskDevice, partitionIndex, type);    
        default:
            return BT_IO_INVALID_PCI;
    }
}
BT_STATUS SetupSataFilesystemAhci(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type){
    SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)diskDevice->header.h0.bar5);
    hba->globalHostControl.interruptEnable = TRUE;

    SATA_PORT_REGISTER *port = NULL;
    UINT32 portIndex = 0;
    BT_STATUS status = SATA_FIND_PORT(hba, &port, &portIndex);
    if (BT_ERROR(status)) return status;

    SATA_START_DMA_ENGINE(port);

    VOID *firstSector = NULL;
    UINTN firstSectorSize = SATA_BASE_SECTOR_SIZE;
    status = AllocPhysicalPool(&firstSector, &firstSectorSize, 0);
    if (BT_ERROR(status)) return status;
    
    status = SATA_READ_DMA_EXT(port, 0, 1, &firstSector);
    if (BT_ERROR(status)) return status;    
    status = SATA_SAFE_PORT_RUN(port, portIndex);
    if (BT_ERROR(status)) return status;
    
    MBR_MODERN* fmbr = (MBR_MODERN*)firstSector;
    if (fmbr->signature != MBR_SIGNATURE) return BT_IO_INVALID_SYMBOL;
    
    MBR_PARTITION_ENTRY **partition = NULL;
    switch (partitionIndex)
    {
        case 0:
            *partition = &fmbr->partitionEntry0;
            break;
        case 1:
            *partition = &fmbr->partitionEntry1;
            break;
        case 2:
            *partition = &fmbr->partitionEntry2;
            break;
        case 3:
            *partition = &fmbr->partitionEntry3;
            break;
        default:
            return BT_INVALID_ARGUMENT;
    }

    VOID* partitionBuffer = NULL;
    UINTN partitionBufferSize = SATA_BASE_SECTOR_SIZE;
    status = AllocPhysicalPool((VOID**)&partitionBuffer, &partitionBufferSize, 0);
    if (BT_ERROR(status)) return status;

    status = SATA_READ_DMA_EXT(port, (*partition)->firstLba, 1, (VOID**)&partitionBuffer);
    if (BT_ERROR(status)) return status;
    status = SATA_SAFE_PORT_RUN(port, portIndex);
    if (BT_ERROR(status)) return status;
    
    GPT_HEADER *gptHeader = (GPT_HEADER*)partitionBuffer;
    if (gptHeader->signature != GPT_SIGNATURE_BIG && gptHeader->signature != GPT_SIGNATURE_LITTLE) return BT_INVALID_BUFFER;

    // TODO: FAT32 BOOT SECTOR INFORMATION WRITING TO FIRST SECTOR

    return BT_SUCCESS;
}