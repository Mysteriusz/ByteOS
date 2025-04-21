#include "disk.h"
#include "mbr.h"
#include "gpt.h"
#include "sata.h"

UINT32 closestFree;
IO_DISK *disks[IO_MAX_DISKS];

// ==================================== |
//              MAIN METHODS            |
// ==================================== |

BT_STATUS ByteAPI InjectDisk(IN PCI *pci, OUT IO_DISK **disk){
    if (closestFree >= IO_MAX_DISKS) return BT_IO_DISK_OVERFLOW;
    if (pci == NULL) return BT_INVALID_ARGUMENT;
    if (pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    BT_STATUS status = 0;
    UINT32 diskIndex = UINT32_MAX;

    for (UINT32 i = 0; i < IO_MAX_DISKS; i++){
        if (disks[i]->pci != pci){
            if (i < diskIndex && disks[i] == NULL){
                diskIndex = i;
            }
            continue;
        }

        return BT_IO_DISK_REGISTERED;
    }

    // CREATE NEW DISK ENTRY  

    UINTN diskSize = sizeof(IO_DISK);
    status = AllocPhysicalPool((VOID**)&disks[diskIndex], &diskSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) return status;

    disks[diskIndex]->pci = pci;
    disks[diskIndex]->scheme = 0;
    disks[diskIndex]->index = diskIndex;
    disks[diskIndex]->io.info = DiskIfInfo;
    disks[diskIndex]->io.read = DiskIfRead;
    disks[diskIndex]->io.write = DiskIfWrite;
    disks[diskIndex]->io.flush = DiskIfFlush;
    
   // ALLOC NECESSARY POOLS    
    
    VOID *idf = NULL;
    UINTN idfs = PAGE_SIZE;
    status = AllocPhysicalPool(&idf, &idfs, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    // READ DEVICE INFO

    status = disks[diskIndex]->io.info(disks[diskIndex], idf);
    if (BT_ERROR(status)) goto CLEANUP;

    // TODO: DEVICE INFO READING METHOD
    switch (pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            disks[diskIndex]->info.logicalBlockSize = SATA_LOGICAL_SIZE(idf); 
            disks[diskIndex]->info.logicalBlockCount = SATA_LOGICAL_COUNT(idf); 
            break;
        default:
            break;
    }
    
    VOID *mbr = NULL;
    UINTN mbrs = disks[diskIndex]->info.logicalBlockSize;
    status = AllocPhysicalPool(&mbr, &mbrs, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    // READ PROTECTIVE MBR AND TEMP GPT

    status = disks[diskIndex]->io.read(disks[diskIndex], 0, 1, mbr);
    if (BT_ERROR(status)) goto CLEANUP;
    disks[diskIndex]->mbr = mbr;
    
    // SET CORRECT PARTITION SCHEME    

    if (IsGpt(disks[diskIndex]) == TRUE){
        disks[diskIndex]->scheme = IO_DISK_SCHEME_GPT;
    }
    else if (((MBR_CLASSIC*)disks[diskIndex]->mbr)->signature == MBR_SIGNATURE_LITTLE || ((MBR_CLASSIC*)disks[diskIndex]->mbr)->signature == MBR_SIGNATURE_BIG){
        disks[diskIndex]->scheme = IO_DISK_SCHEME_MBR;
    }
    else{
        disks[diskIndex]->scheme = IO_DISK_SCHEME_UNK;
    }

    // UPDATE CLOSEST FREE DISK ENTRY

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    *disk = disks[diskIndex];

    CLEANUP:
    if (idf) FreePhysicalPool((VOID**)&idf, &idfs);
    if (BT_ERROR(status) && disks[diskIndex]) FreePhysicalPool((VOID**)&disks[diskIndex], &diskSize);
    if (BT_ERROR(status) && mbr) FreePhysicalPool((VOID**)&mbr, &mbrs);

    return status;
}
BT_STATUS ByteAPI EjectDisk(IN IO_DISK **disk){
    if (*disk == NULL) return BT_INVALID_ARGUMENT;
    
    BT_STATUS status = 0;

    disks[(*disk)->index] = NULL;

    UINTN s = sizeof(IO_DISK);
    status = FreePhysicalPool((VOID**)disk, &s);
    if (BT_ERROR(status)) return status;

    return BT_SUCCESS;
}

// ==================================== |
//         DISK METHODS INTEFACE        |
// ==================================== |

BT_STATUS ByteAPI DiskIfInfo(IN IO_DISK *disk, IN OUT VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    BT_STATUS status = 0;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            status = DiskIfSata(disk, 0, 0, buffer, IO_DISK_COMMAND_INFO);    
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        default:
            return BT_IO_INVALID_PCI;
    }

    CLEANUP:

    return status;
}
BT_STATUS ByteAPI DiskIfRead(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return DiskIfSata(disk, lba, count, buffer, IO_DISK_COMMAND_READ);    
        default:
            return BT_IO_INVALID_PCI;
    }
}
BT_STATUS ByteAPI DiskIfWrite(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return DiskIfSata(disk, lba, count, buffer, IO_DISK_COMMAND_WRITE);    
        default:
            return BT_IO_INVALID_PCI;
    }
}
BT_STATUS ByteAPI DiskIfFlush(IN IO_DISK *disk){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return DiskIfSata(disk, 0, 0, NULL, IO_DISK_COMMAND_FLUSH);    
        default:
            return BT_IO_INVALID_PCI;
    }
}

BT_STATUS ByteAPI DiskIfSata(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer, IN UINT16 commandType){
    SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)disk->pci->header.h0.bar5);
    SATA_PORT_REGISTER *port = NULL;
    UINT32 portIndex = 0;

    BT_STATUS status = SataFindFreePort(hba, &port, &portIndex);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = SataStartDmaEngine(port);
    if (BT_ERROR(status)) goto CLEANUP;
    
    switch (commandType)
    {
        // INFO
        case IO_DISK_COMMAND_INFO:
            status = SataIdentifyDevice(port, (SATA_IDENTIFY_DEVICE_DATA**)&buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // READ
        case IO_DISK_COMMAND_READ:
            status = SataReadDmaExt(port, lba, count, &buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // WRITE
        case IO_DISK_COMMAND_WRITE:        
            status = SataWriteDmaExt(port, lba, count, buffer);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        // FLUSH
        case IO_DISK_COMMAND_FLUSH:
            status = SataFlushDmaExt(port);
            if (BT_ERROR(status)) goto CLEANUP;
            break;
        default:
            break;
    }
    
    status = SataSafePortRun(port, portIndex);
    if (BT_ERROR(status)) goto CLEANUP;
    
    CLEANUP:
    SataStopDmaEngine(port);

    return status;
}