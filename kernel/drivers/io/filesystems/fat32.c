#include "fat32.h"
#include "x86.h"

BT_STATUS FAT32_Setup(IN IO_DISK *disk){
    if (disk == NULL){
        return BT_INVALID_ARGUMENT;
    }
    if (disk->filesystem == FAT32){
        return BT_IO_INVALID_DISK;
    }

    VOID *bootSectorBlock = NULL;
    UINTN bootSectorBlockSize = sizeof(FAT32_BOOT_SECTOR);
    UINT64 lba = 0;
    // FAT32_GetBootSectorBlock(disk, );
    
    return BT_SUCCESS;
}
BT_STATUS FAT32_CreateBootSectorBlock(IN FAT32_BOOT_SECTOR *buffer){
    BT_STATUS status;

    status = CopyPhysicalMemory((UINT8[]){FAT32_BASE_JUMP_CODE}, 3, buffer->jumpCode);
    if (BT_ERROR(status)) return status;
    status = CopyPhysicalMemory((CHAR8[]){FAT32_BASE_OEM}, 8, buffer->oemName);
    if (BT_ERROR(status)) return status;

    buffer->bytesPerSector = FAT32_BASE_BYTES_PER_SECTOR;
    buffer->sectorsPerCluster = FAT32_BASE_SECTORS_PER_CLUSTER;
    buffer->reservedSectors = FAT32_BASE_RESERVED_SECTORS;
    buffer->numberOfCopies = FAT32_BASE_NUMBER_OF_COPIES;
    buffer->maxRootEntries = FAT32_BASE_MAX_ROOT_ENTRIES;
    buffer->numberOfSmallerThan32 = FAT32_BASE_NUMBER_OF_SMALLER_SECTORS;
    buffer->mediaDescriptor = FAT32_BASE_MEDIA_DESCRIPTOR;
    buffer->sectorsPerFat = FAT32_BASE_SECTORS_PER_FAT;
    buffer->sectorsPerTrack = FAT32_BASE_SECTORS_PER_TRACK;
    buffer->numberOfHeads = FAT32_BASE_NUMBER_OF_HEADS;
    buffer->numberOfHiddenSectorsInPartition = 0x00;
    buffer->numberOfSectorsInPartition = 0x00;
    buffer->numberOfSectorsPerFatInPartition = 0x00;
    buffer->flags = FAT32_BASE_FLAGS;
    buffer->versionOfFatDrive = FAT32_BASE_VERSION;
    buffer->clusterNumberOfStartof = 0x00;
    buffer->sectorNumberOfFilesystemInfo = FAT32_BASE_FILESYSTEM_INFO_SECTOR;
    buffer->sectorNumberOfBackupBoot = FAT32_BASE_BACKUP_BOOT_SECTOR;
    buffer->logicalDriveNumberOfPartition = FAT32_BASE_DRIVE_NUMBER;
    buffer->extendedSignature = 0x00;
    buffer->serialNumberOfPartition = FAT32_BASE_SERIAL_NUMBER;

    status = CopyPhysicalMemory((CHAR8[]){FAT32_BASE_VOLUME_NAME}, 11, buffer->volumeNameOfPartition);
    if (BT_ERROR(status)) return status;
    status = CopyPhysicalMemory((CHAR8[]){FAT32_BASE_FAT_NAME}, 8, buffer->fatName);
    if (BT_ERROR(status)) return status;

    buffer->bootRecordSignature = FAT32_BASE_BOOT_SIGNATURE;

    return BT_SUCCESS;
}
BT_STATUS FAT32_GetBootSectorBlock(IN IO_DISK *disk, OUT UINT64 *lba, IN OUT FAT32_BOOT_SECTOR *buffer){
    BT_STATUS status = 0;
    
    if (buffer == NULL){
        return BT_INVALID_ARGUMENT;
    }
    if (disk == NULL){
        return BT_INVALID_ARGUMENT;
    }

    VOID* firstSector = NULL;
    UINTN firstSectorSize = MBR_SIZE;
    status = AllocPhysicalPool(&firstSector, &firstSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    status = disk->functions.read(disk, 0, 1, firstSector);
    if (BT_ERROR(status)) goto CLEANUP;

    MBR_CLASSIC *protectiveMbr = (MBR_CLASSIC*)firstSector;
    
    if (protectiveMbr->signature != MBR_SIGNATURE_LITTLE && protectiveMbr->signature != MBR_SIGNATURE_BIG){
        status = BT_IO_INVALID_DISK;
        goto CLEANUP;
    }
    
    VOID* firstEntry = NULL;
    UINTN firstEntrySize = MBR_SIZE;
    status = AllocPhysicalPool(&firstEntry, &firstEntrySize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    status = disk->functions.read(disk, protectiveMbr->partitionEntry0.firstLba, 1, firstEntry);
    if (BT_ERROR(status)) goto CLEANUP;

    VOID* partitonEntry = NULL;
    UINTN partitonEntrySize = sizeof(GPT_PARTITON_ENTRY);

    GPT_HEADER *gpt = (GPT_HEADER*)firstEntry;
    UINT64 sectorLba = 0;

    if (gpt->signature == GPT_SIGNATURE_LITTLE || gpt->signature == GPT_SIGNATURE_BIG){
        UINT64 partitonArrayLba = gpt->startingLba + disk->partitionIndex;
        UINT64 lbaPerEntry = 0;
        
        if (gpt->sizeOfEntry <= GPT_SIZE){
            lbaPerEntry = 1;
        }
        else{
            lbaPerEntry = gpt->sizeOfEntry / GPT_SIZE;
        }

        status = AllocPhysicalPool(&partitonEntry, &partitonEntrySize, BT_MEMORY_KERNEL_RW);
        if (BT_ERROR(status)) goto CLEANUP;
        status = disk->functions.read(disk, partitonArrayLba * lbaPerEntry, lbaPerEntry, partitonEntry);
        if (BT_ERROR(status)) goto CLEANUP;

        GPT_PARTITON_ENTRY *entry = (GPT_PARTITON_ENTRY*)partitonEntry;
        sectorLba = entry->firstLba;        
    }
    else{
        MBR_PARTITION_ENTRY *entry = NULL;
        switch (disk->partitionIndex)
        {
            case 0:
                entry = &protectiveMbr->partitionEntry0;
                break;
            case 1:
                entry = &protectiveMbr->partitionEntry1;
                break;
            case 2:
                entry = &protectiveMbr->partitionEntry2;
                break;
            case 3:
                entry = &protectiveMbr->partitionEntry3;
                break;
            default:
                status = BT_IO_INVALID_DISK;
                goto CLEANUP;
        }
        sectorLba = entry->firstLba;
    }
    
    status = disk->functions.read(disk, sectorLba, 1, (VOID*)buffer);
    if (BT_ERROR(status)) goto CLEANUP;

    *lba = sectorLba;

    CLEANUP:
    if (firstSector) FreePhysicalPool(&firstSector, &firstSectorSize);
    if (firstEntry) FreePhysicalPool(&firstEntry, &firstEntrySize);
    if (partitonEntry) FreePhysicalPool(&partitonEntry, &partitonEntrySize);
    
    return status;
}