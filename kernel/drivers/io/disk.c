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
            disks[diskIndex]->info.logicalSize = SATA_LOGICAL_SIZE(idf) * SATA_LOGICAL_COUNT(idf);
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

        status = GptIdentifyPartitions(disks[diskIndex]);
        if (BT_ERROR(status)) goto CLEANUP;
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
//                REGIONS               |
// ==================================== |

// TODO: CHA Support for GPT disk mapping
BT_STATUS ByteAPI MapRegions(IN IO_DISK* disk) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;

    disk->info.regionList.sizeOfNode = sizeof(IO_DISK_REGION);
    disk->info.regionList.nextRva = OFFSET_OF(IO_DISK_REGION, next);

    // (~PROTECTIVE) MBR (MASTER BOOT RECORD) region
    IO_DISK_REGION* mbrRegion = NULL;
    status = AddRegion(disk, 0, 0, 0, 0, FALSE, NULL, &mbrRegion);
    if (BT_ERROR(status)) goto CLEANUP;

    UINTN partitionArraySize = 0;
    VOID* partitionArray = NULL;

    // If disk scheme is GPT (GUID PARTITION TABLE)
    if (disk->scheme == IO_DISK_SCHEME_GPT) {
        // Alloc GPT header and GPT map region
        GPT_HEADER* gptHeaderRegion = NULL;
        IO_DISK_REGION* gptHeaderMapRegion = NULL;
        UINTN gptHeaderRegionSize = sizeof(GPT_HEADER);
        UINTN gptHeaderMapRegionSize = sizeof(IO_DISK_REGION);
        status = AllocPhysicalPool((VOID**)&gptHeaderRegion, &gptHeaderRegionSize, BT_MEMORY_KERNEL_RW);
        if (BT_ERROR(status)) goto CLEANUP;
        status = AllocPhysicalPool((VOID**)&gptHeaderMapRegion, &gptHeaderMapRegionSize, BT_MEMORY_KERNEL_RW);
        if (BT_ERROR(status)) goto CLEANUP;

        status = disk->io.read(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, 1, (VOID*)gptHeaderRegion);
        if (BT_ERROR(status)) goto CLEANUP;

        // Calculate GPT header and GPT entries LBA range
        UINTN entriesPerLba = disk->info.logicalBlockSize / sizeof(GPT_PARTITON_ENTRY);
        UINTN endIndex = 0 + GPT_MAX_PARTITIONS - 1;
        UINTN partitonTableLbaCount = ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba + (endIndex / entriesPerLba);

        status = AddRegion(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba + partitonTableLbaCount, 0, 0, FALSE, NULL, &gptHeaderMapRegion);
        if (BT_ERROR(status)) goto CLEANUP;

        // Setup GPT partition entry array buffer
        partitionArraySize = sizeof(GPT_PARTITON_ENTRY) * GPT_MAX_PARTITIONS;
        status = AllocPhysicalPool((VOID**)&partitionArray, &partitionArraySize, BT_MEMORY_KERNEL_RW);
        if (BT_ERROR(status)) goto CLEANUP;

        GPT_PARTITON_ENTRY* gptPartitionArray = (GPT_PARTITON_ENTRY*)partitionArray;

        // Read all GPT parititon entries
        status = GptReadPartitionEntry(disk, 0, GPT_MAX_PARTITIONS, gptPartitionArray);
        if (BT_ERROR(status)) goto CLEANUP;

        // Add all partition`s which fLba > 0 & lLba > 0 & GUID is not empty
        for (UINT32 i = 0; i < GPT_MAX_PARTITIONS; i++){
            if (gptPartitionArray[i].firstLba == 0 || gptPartitionArray[i].lastLba == 0) {
                continue;
            }
            if ((ComparePhysicalMemory(&gptPartitionArray[i].uniqueGuid, sizeof(GUID), &(GUID)GUID_MIN)) == BT_SUCCESS) {
                continue;
            }
            IO_DISK_REGION* gptPartitionMapRegion = NULL;
            status = AddRegion(disk, gptPartitionArray[i].firstLba, gptPartitionArray[i].lastLba, 0, 0, FALSE, NULL, &gptPartitionMapRegion);
            if (BT_ERROR(status)) goto CLEANUP;
        }

        // Sort added regions by their start LBA
        status = LinkedUnsafeSort(&disk->info.regionList, 4, OFFSET_OF(IO_DISK_REGION, startLba));
        if (BT_ERROR(status)) goto CLEANUP;
    }
    // TODO: MBR DISK MAP PARTITION INITIALIZATION
    else if (disk->scheme == IO_DISK_SCHEME_MBR) {

    }
    else {
        status = BT_IO_INVALID_DISK;
        goto CLEANUP;
    }

    CLEANUP:
    if (BT_ERROR(status)) UnMapRegions(disk);
    FreePhysicalPool(&partitionArray, &partitionArraySize);

    return status;
}   
BT_STATUS ByteAPI UnMapRegions(IN IO_DISK* disk){
    if (disk == NULL) return BT_INVALID_ARGUMENT;

    BT_STATUS status = LinkedUnsafeClear(&disk->info.regionList);

    return status;
}

BT_STATUS ByteAPI AddRegion(IN IO_DISK* disk, IN UINT32 startLba, IN UINT32 endLba, IN UINT32 startCha, IN UINT32 endCha, IN BOOLEAN free, IN IO_DISK_REGION *next, OUT IO_DISK_REGION **buffer) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;
    
    BT_STATUS status = 0;
    
    status = VerifyRegionRange(disk, startLba, endLba);
    if (BT_ERROR(status)) goto CLEANUP;

    UINTN regionSize = sizeof(IO_DISK_REGION);
    status = AllocPhysicalPool((VOID**)buffer, &regionSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    (*buffer)->next = next;
    (*buffer)->free = free;
    (*buffer)->startLba = startLba;
    (*buffer)->endLba = endLba;
    (*buffer)->startCha = startCha;
    (*buffer)->endCha = endCha;
    
    status = LinkedUnsafeAdd(&disk->info.regionList, (VOID*)*buffer, sizeof(IO_DISK_REGION), 0);
    if (BT_ERROR(status)) goto CLEANUP;

CLEANUP:
    if (BT_ERROR(status) && *buffer) FreePhysicalPool((VOID**)buffer, &regionSize);

    return status;
}
BT_STATUS ByteAPI UpdateRegion(IN IO_DISK* disk, IN UINT32 startLba, IN UINT32 endLba, IN UINT32 startCha, IN UINT32 endCha, IN BOOLEAN free, IN IO_DISK_REGION* next, IN IO_DISK_REGION* buffer) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;

    BT_STATUS status = 0;

    status = VerifyRegionRange(disk, startLba, endLba);
    if (BT_ERROR(status)) return status;

    buffer->next = next;
    buffer->free = free;
    buffer->startLba = startLba;
    buffer->endLba = endLba;
    buffer->startCha = startCha;
    buffer->endCha = endCha;

    return BT_SUCCESS;
}
BT_STATUS ByteAPI VerifyRegionRange(IN IO_DISK* disk, IN UINT32 startLba, IN UINT32 endLba) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;

    IO_DISK_REGION* curr = (IO_DISK_REGION*)disk->info.regionList.root;

    while (curr != NULL) {
        if ((startLba == curr->startLba && endLba == curr->endLba) ||
            (endLba == curr->startLba && startLba == curr->endLba))
            return curr->endLba;

        // [(]a) 
        if (startLba < curr->startLba && endLba > curr->startLba) return BT_INVALID_ARGUMENT;
        // (a[])
        if (startLba > curr->startLba && endLba < curr->endLba) return BT_INVALID_ARGUMENT;
        // (a[)]
        if (startLba < curr->endLba && endLba > curr->endLba) return BT_INVALID_ARGUMENT;
        // [(a)]
        if (startLba < curr->startLba && endLba > curr->endLba) return BT_INVALID_ARGUMENT;

        curr = curr->next;
    }

    return BT_SUCCESS;
}
BT_STATUS ByteAPI GetRegion(IN IO_DISK* disk, IN OPTIONAL UINT32* startLba, IN OPTIONAL UINT32* startCha, OUT IO_DISK_REGION** region) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (startLba == NULL && startCha == NULL) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;

    if (startCha == NULL) {
        status = LinkedUnsafeFind(&disk->info.regionList, (VOID*)startLba, sizeof(UINT32), OFFSET_OF(IO_DISK_REGION, startLba), (VOID**)region);
    }
    else if (startLba == NULL) {
        status = LinkedUnsafeFind(&disk->info.regionList, (VOID*)startCha, sizeof(UINT32), OFFSET_OF(IO_DISK_REGION, startCha), (VOID**)region);
    }

    return status;
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