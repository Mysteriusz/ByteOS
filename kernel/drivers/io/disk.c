#include "disk.h"
#include "mbr.h"
#include "gpt.h"
#include "sata.h"

UINT32 closestFree;
IO_DISK *disks[IO_MAX_DISKS];

// ==================================== |
//              MAIN METHODS            |
// ==================================== |

BT_STATUS ByteAPI RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count){
    closestFree = 0;
    UINT32 msu = 0;
    for (UINT32 i = 0; i < *count; i++){
        IO_DISK *disk = NULL;
        BT_STATUS status = RegisterDisk((PCI*)devices[i].pciAddress, &disk);

        if (status == BT_IO_INVALID_PCI){
            continue;
        }
        else if (BT_ERROR(status)){
            return status;
        }

        msu++;
    }

    *count = msu;

    return BT_SUCCESS;
}
BT_STATUS ByteAPI RegisterDisk(IN PCI *pci, OUT IO_DISK **io){
    if (closestFree >= IO_MAX_DISKS) return BT_IO_DISK_OVERFLOW;
    if (pci == NULL) return BT_INVALID_ARGUMENT;
    if (pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    for (UINT32 i = 0; i < IO_MAX_DISKS; i++){
        if (disks[i]->pci != pci) continue;

        return BT_IO_DISK_REGISTERED;
    }

    UINTN s = sizeof(IO_DISK);
    BT_STATUS status = AllocPhysicalPool((VOID**)io, &s, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) return status;

    UINT32 diskIndex = 0;
    
    // ==================================== |
    //         CREATE NEW DISK ENTRY        |
    // ==================================== |

    disks[diskIndex] = *io;
    disks[diskIndex]->filesystem = 0;
    disks[diskIndex]->pci = pci;
    disks[diskIndex]->partitionIndex = 0;
    disks[diskIndex]->functions.info = IO_DISK_IF_INFO;
    disks[diskIndex]->functions.read = IO_DISK_IF_READ;
    disks[diskIndex]->functions.write = IO_DISK_IF_WRITE;
    disks[diskIndex]->functions.flush = IO_DISK_IF_FLUSH;
    
    // ==================================== |
    //         ALLOC NECESSARY POOLS        |
    // ==================================== |
    
    VOID *tc0 = NULL;
    UINTN tcs0 = MBR_SIZE;
    status = AllocPhysicalPool(&tc0, &tcs0, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    VOID *tc1 = NULL;
    UINTN tcs1 = GPT_SIZE;
    status = AllocPhysicalPool(&tc1, &tcs1, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    VOID *tc2 = NULL;
    UINTN tcs2 = GPT_SIZE;
    status = AllocPhysicalPool(&tc2, &tcs2, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    VOID *tc3 = NULL;
    UINTN tcs3 = sizeof(SATA_IDENTIFY_DEVICE_DATA);
    status = AllocPhysicalPool(&tc3, &tcs3, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    // ==================================== |
    //   READ PROTECTIVE MBR AND TEMP GPT   |
    // ==================================== |
    
    status = disks[diskIndex]->functions.read(disks[diskIndex], 0, 1, tc0);
    if (BT_ERROR(status)) goto CLEANUP;
    
    MBR_MODERN *mbrHeader = (MBR_MODERN*)tc0;
    
    status = disks[diskIndex]->functions.read(disks[diskIndex], mbrHeader->partitionEntry0.firstLba, 1, tc1);
    if (BT_ERROR(status)) goto CLEANUP;
    
    GPT_HEADER *gptHeader = (GPT_HEADER*)tc1;

    // ==================================== |
    //        SETUP DISK GENERAL INFO       |
    // ==================================== |

    status = IO_DISK_IF_SATA(disks[diskIndex], 0, 0, tc3, IO_DISK_COMMAND_INFO);    
    if (BT_ERROR(status)) goto CLEANUP;
    
    SATA_IDENTIFY_DEVICE_DATA *idfData = (SATA_IDENTIFY_DEVICE_DATA*)tc3;
    IO_DISK_INFO_DATA* diskInfoData = (IO_DISK_INFO_DATA*)&disks[diskIndex]->info;

    diskInfoData->logicalBlockCount = idfData->logicalCount;

    if (idfData->sectorInfo.logicalLongerThan256W){
        diskInfoData->logicalBlockSize = idfData->wordsPerLogicalSector;
    }
    else{
        diskInfoData->logicalBlockSize = SATA_BASE_SECTOR_SIZE;
    }

    // ==================================== |
    //      SET CORRECT PARTITION SCHEME    |
    // ==================================== |

    if (gptHeader->signature == GPT_SIGNATURE_LITTLE || gptHeader->signature == GPT_SIGNATURE_BIG){
        disks[diskIndex]->partitionScheme = IO_DISK_SCHEME_GPT;
    }
    else if (mbrHeader->signature == MBR_SIGNATURE_LITTLE || mbrHeader->signature == MBR_SIGNATURE_BIG){
        disks[diskIndex]->partitionScheme = IO_DISK_SCHEME_MBR;
    }
    else{
        disks[diskIndex]->partitionScheme = IO_DISK_SCHEME_UNK;
    }

    // ==================================== |
    //    UPDATE CLOSESNT FREE DISK ENTRY   |
    // ==================================== |

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    CLEANUP:
    if (tc0) FreePhysicalPool((VOID**)&tc0, &tcs0);
    if (tc1) FreePhysicalPool((VOID**)&tc1, &tcs1);
    if (tc2) FreePhysicalPool((VOID**)&tc2, &tcs2);
    if (tc3) FreePhysicalPool((VOID**)&tc3, &tcs3);

    return status;
}

BT_STATUS ByteAPI GetDisk(IN UINT32 index, OUT IO_DISK **disk){
    if (index >= IO_MAX_DISKS) return BT_INVALID_ARGUMENT;

    *disk = disks[index];

    return BT_SUCCESS;   
}

// ==================================== |
//         DISK METHODS INTEFACE        |
// ==================================== |

BT_STATUS ByteAPI IO_DISK_IF_INFO(IN IO_DISK *disk, IN OUT VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    BT_STATUS status = 0;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            status = IO_DISK_IF_SATA(disk, 0, 0, buffer, IO_DISK_COMMAND_INFO);    
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        default:
            return BT_IO_INVALID_PCI;
    }

    CLEANUP:

    return status;
}
BT_STATUS ByteAPI IO_DISK_IF_READ(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return IO_DISK_IF_SATA(disk, lba, count, buffer, IO_DISK_COMMAND_READ);    
        default:
            return BT_IO_INVALID_PCI;
    }
}
BT_STATUS ByteAPI IO_DISK_IF_WRITE(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return IO_DISK_IF_SATA(disk, lba, count, &buffer, IO_DISK_COMMAND_WRITE);    
        default:
            return BT_IO_INVALID_PCI;
    }
}
BT_STATUS ByteAPI IO_DISK_IF_FLUSH(IN IO_DISK *disk){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return IO_DISK_IF_SATA(disk, 0, 0, NULL, IO_DISK_COMMAND_FLUSH);    
        default:
            return BT_IO_INVALID_PCI;
    }
}

BT_STATUS ByteAPI IO_DISK_IF_SATA(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer, IN UINT16 commandType){
    SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)disk->pci->header.h0.bar5);
    SATA_PORT_REGISTER *port = NULL;
    UINT32 portIndex = 0;

    BT_STATUS status = SATA_FIND_PORT(hba, &port, &portIndex);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = SATA_START_DMA_ENGINE(port);
    if (BT_ERROR(status)) goto CLEANUP;
    
    switch (commandType)
    {
        // INFO
        case IO_DISK_COMMAND_INFO:
            status = SATA_IDENTIFY_DEVICE(port, (SATA_IDENTIFY_DEVICE_DATA**)&buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // READ
        case IO_DISK_COMMAND_READ:
            status = SATA_READ_DMA_EXT(port, lba, count, &buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // WRITE
        case IO_DISK_COMMAND_WRITE:        
            status = SATA_WRITE_DMA_EXT(port, lba, count, buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // FLUSH
        case IO_DISK_COMMAND_FLUSH:
            status = SATA_FLUSH_CACHE_EXT(port);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        default:
            break;
    }
    
    status = SATA_SAFE_PORT_RUN(port, portIndex);
    if (BT_ERROR(status)) goto CLEANUP;
    
    CLEANUP:
    SATA_STOP_DMA_ENGINE(port);

    return status;
}