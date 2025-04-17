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
BT_STATUS ByteAPI RegisterDisk(IN PCI *pci, IN OUT IO_DISK **io){
    if (closestFree >= IO_MAX_DISKS) return BT_IO_DISK_OVERFLOW;
    if (pci == NULL) return BT_INVALID_ARGUMENT;
    if (*io == NULL) return BT_INVALID_BUFFER;
    if (pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    for (UINT32 i = 0; i < IO_MAX_DISKS; i++){
        if (disks[i]->pci != pci) continue;

        return BT_IO_DISK_REGISTERED;
    }

    // CREATE NEW DISK ENTRY       
    
    BT_STATUS status = 0;
    UINT32 diskIndex = 0;

    disks[diskIndex] = *io;
    disks[diskIndex]->pci = pci;
    disks[diskIndex]->scheme = 0;
    disks[diskIndex]->partitionCount = 0;
    disks[diskIndex]->partitions = NULL;
    disks[diskIndex]->io.info = DiskIfInfo;
    disks[diskIndex]->io.read = DiskIfRead;
    disks[diskIndex]->io.write = DiskIfWrite;
    disks[diskIndex]->io.flush = DiskIfFlush;
    
    disks[diskIndex]->info.logicalBlockCount = 0;
    disks[diskIndex]->info.logicalBlockSize = 0;
    
   // ALLOC NECESSARY POOLS    
    
    VOID *tc0 = NULL;
    UINTN tcs0 = MBR_SIZE;
    status = AllocPhysicalPool(&tc0, &tcs0, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    VOID *tc1 = NULL;
    UINTN tcs1 = GPT_SIZE;
    status = AllocPhysicalPool(&tc1, &tcs1, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    // READ PROTECTIVE MBR AND TEMP GPT
    
    status = disks[diskIndex]->io.read(disks[diskIndex], 0, 1, tc0);
    if (BT_ERROR(status)) goto CLEANUP;
    
    MBR_MODERN *mbrHeader = (MBR_MODERN*)tc0;
    
    status = disks[diskIndex]->io.read(disks[diskIndex], mbrHeader->partitionEntry0.firstLba, 1, tc1);
    if (BT_ERROR(status)) goto CLEANUP;
    
    GPT_HEADER *gptHeader = (GPT_HEADER*)tc1;

    // SET CORRECT PARTITION SCHEME    

    if (gptHeader->signature == GPT_SIGNATURE_LITTLE || gptHeader->signature == GPT_SIGNATURE_BIG){
        disks[diskIndex]->scheme = IO_DISK_SCHEME_GPT;
    }
    else if (mbrHeader->signature == MBR_SIGNATURE_LITTLE || mbrHeader->signature == MBR_SIGNATURE_BIG){
        disks[diskIndex]->scheme = IO_DISK_SCHEME_MBR;
    }
    else{
        disks[diskIndex]->scheme = IO_DISK_SCHEME_UNK;
    }

    // UPDATE CLOSESNT FREE DISK ENTRY

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    CLEANUP:
    if (tc0) FreePhysicalPool((VOID**)&tc0, &tcs0);
    if (tc1) FreePhysicalPool((VOID**)&tc1, &tcs1);

    return status;
}
BT_STATUS ByteAPI CreatePartition(IN IO_DISK *io, IN UINTN size, IN OUT IO_DISK_PARTITION *partition){
    if (io->scheme == IO_DISK_SCHEME_MBR && io->partitionCount == MBR_MAX_PARTITIONS) return BT_IO_DISK_OVERFLOW;
    if (io->scheme == IO_DISK_SCHEME_GPT && io->partitionCount == GPT_MAX_PARTITIONS) return BT_IO_DISK_OVERFLOW;
    if (io->scheme == IO_DISK_SCHEME_UNK) return BT_IO_INVALID_DISK;
    if (partition == NULL) return BT_INVALID_BUFFER;

    IO_DISK_PARTITION *curr = io->partitions;
    while (curr != NULL){   
        if (curr->partitionIndex < curr->next->partitionIndex - 1){
            partition->partitionIndex = curr->partitionIndex + 1;
            partition->next = curr->next;
            curr->next = partition;
        }

        curr = curr->next;
    }

    partition->disk = io;
    partition->filesystem = NULL;
    partition->size = size;

    return BT_SUCCESS;
}

BT_STATUS ByteAPI GetDisk(IN UINT32 index, OUT IO_DISK **disk){
    if (index >= IO_MAX_DISKS) return BT_INVALID_ARGUMENT;

    *disk = disks[index];

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