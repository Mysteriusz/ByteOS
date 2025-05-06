#include "gpt.h"
#include "mbr.h"
#include "crc.h"

BOOLEAN ByteAPI IsGpt(IN IO_DISK *disk){
    BT_STATUS status = 0;
    BOOLEAN result = FALSE;

    VOID *gpt = NULL;
    UINTN gpts = disk->info.logicalBlockSize;
    status = AllocPhysicalPool(&gpt, &gpts, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    disk->io.read(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, 1, gpt);
    if (BT_ERROR(status)) goto CLEANUP;

    if (((GPT_HEADER*)gpt)->signature == GPT_SIGNATURE_BIG || ((GPT_HEADER*)gpt)->signature == GPT_SIGNATURE_LITTLE){
        result = TRUE;
    }

    CLEANUP:
    if (gpt) FreePhysicalPool(&gpt, &gpts);

    return result;
}
BOOLEAN ByteAPI GptValidPartition(IN GPT_PARTITON_ENTRY* entry) {
    return ComparePhysicalMemory((VOID*)&entry->typeGuid, sizeof(GUID), (VOID*)(&(GUID)GUID_MIN)) != BT_COMPARE_EQUAL &&
        ComparePhysicalMemory((VOID*)&entry->uniqueGuid, sizeof(GUID), (VOID*)(&(GUID)GUID_MIN)) != BT_COMPARE_EQUAL &&
        entry->firstLba != 0 &&
        entry->lastLba != 0;
}

BT_STATUS ByteAPI GptIdentifyPartitions(IN IO_DISK *disk){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (disk->scheme != IO_DISK_SCHEME_GPT) return BT_INVALID_ARGUMENT;
    
    BT_STATUS status = 0;
    
    VOID *gptSector = NULL;
    UINTN gptSectorSize = FIT_IN_SIZE(disk->info.logicalBlockSize, disk->info.logicalBlockSize + (sizeof(GPT_PARTITON_ENTRY) * GPT_MAX_PARTITIONS));    
    status = AllocPhysicalPool(&gptSector, &gptSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.read(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, gptSectorSize / disk->info.logicalBlockSize, gptSector);
    if (BT_ERROR(status)) goto CLEANUP;

    GPT_PARTITON_ENTRY *entry = (GPT_PARTITON_ENTRY*)(disk->info.logicalBlockSize + (PHYSICAL_ADDRESS)gptSector);
    
    for (UINT32 i = 0; i < GPT_MAX_PARTITIONS; i++){
        if (ComparePhysicalMemory((VOID*)&entry->uniqueGuid, sizeof(GUID), (VOID*)(&(GUID)GUID_MIN)) != BT_SUCCESS){
            IO_DISK_PARTITION_BIT_SET(disk, i);
        }
        
        entry = (GPT_PARTITON_ENTRY*)((PHYSICAL_ADDRESS)entry + sizeof(GPT_PARTITON_ENTRY));
    }

    CLEANUP:
    if (gptSector) FreePhysicalPool(&gptSector, &gptSectorSize);

    return status;
}
BT_STATUS ByteAPI GptUpdateHeaderCrc(IN IO_DISK* disk) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;

    VOID* gptStructure = NULL;
    UINTN gptStructureSize = FIT_IN_SIZE(disk->info.logicalBlockSize, disk->info.logicalBlockSize + (sizeof(GPT_PARTITON_ENTRY) * GPT_MAX_PARTITIONS));
    status = AllocPhysicalPool(&gptStructure, &gptStructureSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    GPT_HEADER* gpt = (GPT_HEADER*)gptStructure;

    status = disk->io.read(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, gptStructureSize / disk->info.logicalBlockSize, gptStructure);
    if (BT_ERROR(status)) goto CLEANUP;

    UINT32 entryCrc = 0;
    VOID* firstEntry = (VOID*)((PHYSICAL_ADDRESS)gptStructure + disk->info.logicalBlockSize);
    status = Crc32((UINT8*)firstEntry, sizeof(GPT_PARTITON_ENTRY) * GPT_MAX_PARTITIONS, &entryCrc);
    if (BT_ERROR(status)) goto CLEANUP;

    gpt->crc32Entry = entryCrc;

    gpt->crc32 = 0;

    UINT32 headerCrc = 0;
    status = Crc32((UINT8*)gptStructure, gpt->headerSize, &headerCrc);
    if (BT_ERROR(status)) goto CLEANUP;

    gpt->crc32 = headerCrc;

    status = disk->io.write(disk, ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba, 1, gptStructure);
    if (BT_ERROR(status)) goto CLEANUP;

CLEANUP:
    if (gptStructure) FreePhysicalPool(&gptStructure, &gptStructureSize);

    return status;
}

BT_STATUS ByteAPI GptWritePartitionEntry(IN IO_DISK* disk, IN UINT32 partitionIndex, IN UINT32 count, IN GPT_PARTITON_ENTRY* buffer) {
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (partitionIndex >= GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (count > GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;

    BT_STATUS status = 0;

    UINTN entriesPerLba = disk->info.logicalBlockSize / sizeof(GPT_PARTITON_ENTRY);
    UINTN startLba = ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba;
    UINTN endIndex = partitionIndex + count - 1;
    UINTN endLba = ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba + (endIndex / entriesPerLba) + 1;
    UINTN lbaToRead = (endLba - startLba) + 1;

    VOID* gptSector = NULL;
    UINTN gptSectorSize = disk->info.logicalBlockSize * lbaToRead;
    status = AllocPhysicalPool(&gptSector, &gptSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    status = disk->io.read(disk, startLba, lbaToRead, gptSector);
    if (BT_ERROR(status)) goto CLEANUP;

    PHYSICAL_ADDRESS entryAddress = (PHYSICAL_ADDRESS)gptSector + disk->info.logicalBlockSize + ((partitionIndex % 32) * sizeof(GPT_PARTITON_ENTRY));

    status = CopyPhysicalMemory((VOID*)buffer, sizeof(GPT_PARTITON_ENTRY) * count, (VOID*)entryAddress);
    if (BT_ERROR(status)) goto CLEANUP;

    status = disk->io.write(disk, startLba, lbaToRead, gptSector);
    if (BT_ERROR(status)) goto CLEANUP;

CLEANUP:
    if (gptSector) FreePhysicalPool(&gptSector, &gptSectorSize);

    return status;
}
BT_STATUS ByteAPI GptReadPartitionEntry(IN IO_DISK* disk, IN UINT32 partitionIndex, IN UINT32 count, IN GPT_PARTITON_ENTRY* buffer){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (partitionIndex >= GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (count > GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;

    BT_STATUS status = 0;

    UINTN entriesPerLba = disk->info.logicalBlockSize / sizeof(GPT_PARTITON_ENTRY);
    UINT32 startLba = ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba;
    UINTN endIndex = partitionIndex + count - 1;
    UINT32 endLba = ((MBR_CLASSIC*)disk->mbr)->partitionEntry0.firstLba + (endIndex / entriesPerLba) + 1;
    UINTN lbaToRead = (endLba - startLba) + 1;
    
    VOID *gptSector = NULL;
    UINTN gptSectorSize = disk->info.logicalBlockSize * lbaToRead;    
    status = AllocPhysicalPool(&gptSector, &gptSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.read(disk, startLba, lbaToRead, gptSector);
    if (BT_ERROR(status)) goto CLEANUP;
    
    PHYSICAL_ADDRESS entryAddress = (PHYSICAL_ADDRESS)gptSector + disk->info.logicalBlockSize + ((partitionIndex % 32) * sizeof(GPT_PARTITON_ENTRY));
    
    status = CopyPhysicalMemory((VOID*)entryAddress, sizeof(GPT_PARTITON_ENTRY) * count, (VOID*)buffer);
    if (BT_ERROR(status)) goto CLEANUP;
    
    CLEANUP:
    if (gptSector) FreePhysicalPool(&gptSector, &gptSectorSize);
    
    return status;
}
BT_STATUS ByteAPI GptSafeReadPartitionEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN GPT_PARTITON_ENTRY *buffer){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (partitionIndex >= GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;
    if (IO_DISK_PARTITION_BIT_CHECK(disk, partitionIndex) == FALSE) return BT_UNINITIALIZED;

    BT_STATUS status = 0;
    
    status = GptReadPartitionEntry(disk, partitionIndex, 1, buffer);

    return status;
}
BT_STATUS ByteAPI GptSafeWritePartitionEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN GPT_PARTITON_ENTRY *buffer){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (partitionIndex >= GPT_MAX_PARTITIONS) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;

    BT_STATUS status = 0;

    GPT_PARTITON_ENTRY* prevPartition = NULL;
    UINTN prevPartitionSize = sizeof(GPT_PARTITON_ENTRY);
    status = AllocPhysicalPool((VOID**)&prevPartition, &prevPartitionSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    status = GptReadPartitionEntry(disk, partitionIndex, 1, prevPartition);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = GptWritePartitionEntry(disk, partitionIndex, 1, buffer);
    if (BT_ERROR(status)) goto CLEANUP;

    status = GptUpdateHeaderCrc(disk);
    if (BT_ERROR(status)) goto CLEANUP;
    
    UINT32 firstLba = (UINT32)prevPartition->firstLba;
    BOOLEAN prevValid = GptValidPartition(prevPartition);

    IO_DISK_REGION* region = NULL;
    status = 0;
    if (prevValid) {
        status = GetRegion(disk, &firstLba, NULL, &region);
        if (BT_ERROR(status)) goto CLEANUP;
    }

    if (!prevValid || status == BT_NOT_FOUND) {
        status = AddRegion(disk, buffer->firstLba, buffer->lastLba, 0, 0, FALSE, NULL, &region);
        if (BT_ERROR(status)) goto CLEANUP;
    }
    else if (status == BT_SUCCESS) {
        status = UpdateRegion(disk, buffer->firstLba, buffer->lastLba, 0, 0, 0, region->next, region);
        if (BT_ERROR(status)) goto CLEANUP;
    }

    if (IO_DISK_PARTITION_BIT_CHECK(disk, partitionIndex) == FALSE) {
        IO_DISK_PARTITION_BIT_SET(disk, partitionIndex);
    }

    CLEANUP:
    if (prevPartition) FreePhysicalPool((VOID**)&prevPartition, &prevPartitionSize);

    return status;
}
