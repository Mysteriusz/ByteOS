#include "gpt.h"

BT_STATUS ByteAPI GptIdentifyPartitions(IN IO_DISK *disk){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (disk->scheme != IO_DISK_SCHEME_GPT) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;
    
    VOID *partitionArray = NULL;
    UINTN partitionArraySize = FIT_IN_SIZE(disk->info.logicalBlockSize, disk->info.logicalBlockSize + (sizeof(GPT_PARTITON_ENTRY) * GPT_MAX_PARTITIONS));    
    status = AllocPhysicalPool(&partitionArray, &partitionArraySize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.read(disk, disk->gptLba, partitionArraySize / disk->info.logicalBlockSize, partitionArray);
    if (BT_ERROR(status)) goto CLEANUP;

    GPT_PARTITON_ENTRY *entry = (GPT_PARTITON_ENTRY*)(disk->info.logicalBlockSize + (PHYSICAL_ADDRESS)partitionArray);
    
    for (UINT32 i = 0; i < GPT_MAX_PARTITIONS; i++){
        if (ComparePhysicalMemory((VOID*)&entry->uniqueGuid, sizeof(GUID), (VOID*)(&(GUID)GUID_MIN)) != BT_SUCCESS){
            IO_DISK_PARTITION_BIT_SET(disk, i);
        }
        
        entry = (GPT_PARTITON_ENTRY*)((PHYSICAL_ADDRESS)entry + sizeof(GPT_PARTITON_ENTRY));
    }

    CLEANUP:
    if (partitionArray) FreePhysicalPool(&partitionArray, &partitionArraySize);

    return status;
}
BT_STATUS ByteAPI GptWritePartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN GPT_PARTITON_ENTRY *buffer){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;
    
    BT_STATUS status = 0;

    UINTN partitionEntryLba = disk->gptLba + FIT_IN(disk->info.logicalBlockSize, ((partitionIndex + 1) * sizeof(GPT_PARTITON_ENTRY)));

    VOID *partitionBlock = NULL;
    UINTN partitionBlockSize = disk->info.logicalBlockSize;    
    status = AllocPhysicalPool(&partitionBlock, &partitionBlockSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.read(disk, partitionEntryLba, 1, partitionBlock);
    if (BT_ERROR(status)) goto CLEANUP;
       
    PHYSICAL_ADDRESS partitionEntryAddress = (PHYSICAL_ADDRESS)partitionBlock + (partitionIndex % 32) * sizeof(GPT_PARTITON_ENTRY);
    
    status = CopyPhysicalMemory((VOID*)buffer, sizeof(GPT_PARTITON_ENTRY), (VOID*)partitionEntryAddress);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.write(disk, partitionEntryLba, 1, partitionBlock);
    if (BT_ERROR(status)) goto CLEANUP;

    if (IO_DISK_PARTITION_BIT_CHECK(disk, partitionIndex) == FALSE){
        IO_DISK_PARTITION_BIT_SET(disk, partitionIndex);
    }

    CLEANUP:
    if (partitionBlock) FreePhysicalPool(&partitionBlock, &partitionBlockSize);

    return status;
}
BT_STATUS ByteAPI GptReadPartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, OUT GPT_PARTITON_ENTRY **buffer){
    if (disk == NULL) return BT_INVALID_ARGUMENT;
    if (buffer == NULL) return BT_INVALID_BUFFER;
    if (IO_DISK_PARTITION_BIT_CHECK(disk, partitionIndex) == FALSE) return BT_UNINITIALIZED;

    BT_STATUS status = 0;

    UINTN partitionEntryLba = disk->gptLba + FIT_IN(disk->info.logicalBlockSize, ((partitionIndex + 1) * sizeof(GPT_PARTITON_ENTRY)));

    VOID *partitionBlock = NULL;
    UINTN partitionBlockSize = disk->info.logicalBlockSize;    
    status = AllocPhysicalPool(&partitionBlock, &partitionBlockSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = disk->io.read(disk, partitionEntryLba, 1, partitionBlock);
    if (BT_ERROR(status)) goto CLEANUP;
    
    UINTN partitionEntrySize = sizeof(GPT_PARTITON_ENTRY);
    status = AllocPhysicalPool((VOID**)buffer, &partitionEntrySize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    PHYSICAL_ADDRESS partitionEntryAddress = (PHYSICAL_ADDRESS)partitionBlock + (partitionIndex % 32) * sizeof(GPT_PARTITON_ENTRY);
    
    status = CopyPhysicalMemory((VOID*)partitionEntryAddress, sizeof(GPT_PARTITON_ENTRY), (VOID*)*buffer);
    if (BT_ERROR(status)) goto CLEANUP;

    CLEANUP:
    if (partitionBlock) FreePhysicalPool(&partitionBlock, &partitionBlockSize);
    if (BT_ERROR(status) && *buffer) FreePhysicalPool((VOID**)buffer, &partitionEntrySize);
    
    return status;
}
