#include "filesystem.h"

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI SetupFilesystem(IN IO_DISK *disk, IN UINT16 partitionIndex, IN FS_TYPE type){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCI;
    }

    switch (type)
    {
        case FAT32:
            return FAT32_Setup(disk);
        default:
            return BT_INVALID_ARGUMENT;
    } 
}

// BT_STATUS SetupSataFilesystemAhci(IN IO_DISK *disk, IN UINT8 partitionIndex, IN FS_TYPE type){
//     SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)diskDevice->header.h0.bar5);
//     hba->globalHostControl.interruptEnable = TRUE;

//     SATA_PORT_REGISTER *port = NULL;
//     UINT32 portIndex = 0;
//     BT_STATUS status = SATA_FIND_PORT(hba, &port, &portIndex);
//     if (BT_ERROR(status)) goto CLEANUP;

//     SATA_START_DMA_ENGINE(port);

//     // Allocate pools for partition type checks
//     VOID *firstSector = NULL;
//     UINTN firstSectorSize = SATA_BASE_SECTOR_SIZE;
//     status = AllocPhysicalPool(&firstSector, &firstSectorSize, BT_MEMORY_KERNEL_RW);
//     if (BT_ERROR(status)) goto CLEANUP;
//     VOID* partitionBuffer = NULL;
//     UINTN partitionBufferSize = SATA_BASE_SECTOR_SIZE;
//     status = AllocPhysicalPool((VOID**)&partitionBuffer, &partitionBufferSize, BT_MEMORY_KERNEL_RW);
//     if (BT_ERROR(status)) goto CLEANUP;
//     VOID *fsBootSector = NULL;
//     UINTN fsBootSectorSize = SATA_BASE_SECTOR_SIZE;
//     status = AllocPhysicalPool(&fsBootSector, &fsBootSectorSize, BT_MEMORY_KERNEL_RW);
//     if (BT_ERROR(status)) goto CLEANUP;

//     // Read first sector of the entire disk
//     status = SATA_READ_DMA_EXT(port, 0, 1, &firstSector);
//     if (BT_ERROR(status)) goto CLEANUP;    
//     status = SATA_SAFE_PORT_RUN(port, portIndex);
//     if (BT_ERROR(status)) goto CLEANUP;
    
//     // Check if disk is formatted to MBR 
//     // TODO: Auto formatting to MBR
//     MBR_MODERN* fmbr = (MBR_MODERN*)firstSector;
//     if (fmbr->signature != MBR_SIGNATURE){
//         status = BT_IO_INVALID_SYMBOL;
//         goto CLEANUP;
//     }

//     // Read first partition entry`s sector to check if GPT
//     status = SATA_READ_DMA_EXT(port, fmbr->partitionEntry0.firstLba, 1, (VOID**)&partitionBuffer);
//     if (BT_ERROR(status)) goto CLEANUP;
//     status = SATA_SAFE_PORT_RUN(port, portIndex);
//     if (BT_ERROR(status)) goto CLEANUP;
    
//     MBR_PARTITION_ENTRY **partition = NULL;
    
//     // Check if the MBR is only there for protection and rest is GPT
//     GPT_HEADER *gptHeader = (GPT_HEADER*)partitionBuffer;
//     if (gptHeader->signature == GPT_SIGNATURE_BIG || gptHeader->signature == GPT_SIGNATURE_LITTLE){
//         UINT32 entryLba = gptHeader->startingLba + (partitionIndex * gptHeader->sizeOfEntry);

//         switch (type){
//             case FAT32:
//                 status = FAT32_GET_BOOT_SECTOR((FAT32_BOOT_SECTOR*)fsBootSector);
//                 if (BT_ERROR(status)) goto CLEANUP;

//                 // Write FAT32 boot sector information to device
//                 status = SATA_WRITE_DMA_EXT(port, entryLba, 1, fsBootSector);
//                 if (BT_ERROR(status)) goto CLEANUP;
//                 status = SATA_SAFE_PORT_RUN(port, portIndex);
//                 if (BT_ERROR(status)) goto CLEANUP;
                
//                 break;
//             default:
//                 break;
//         }
//     }
//     // If device is purely MBR 
//     else{
//         switch (partitionIndex)
//         {
//             case 0:
//                 *partition = &fmbr->partitionEntry0;
//                 break;
//             case 1:
//                 *partition = &fmbr->partitionEntry1;
//                 break;
//             case 2:
//                 *partition = &fmbr->partitionEntry2;
//                 break;
//             case 3:
//                 *partition = &fmbr->partitionEntry3;
//                 break;
//             default:
//                 break;
//         }
//     }

//     // TODO: FAT32 BOOT SECTOR INFORMATION WRITING TO FIRST SECTOR

//     CLEANUP:
//     if (fsBootSector) FreePhysicalPool((VOID**)&fsBootSector, &fsBootSectorSize);
//     if (partitionBuffer) FreePhysicalPool((VOID**)&partitionBuffer, &partitionBufferSize);
//     if (firstSector) FreePhysicalPool((VOID**)&firstSector, &firstSectorSize);

//     SATA_STOP_DMA_ENGINE(port);
//     return status;
// }