#include "fat32.h"

BT_STATUS FAT32_GET_BOOT_SECTOR(IN FAT32_BOOT_SECTOR *buffer){
    BT_STATUS status;

    status = CopyPhysicalMemory((UINT8[]){FAT32_BASE_JUMP_CODE}, 3, buffer->jumpCode);
    if (BT_ERROR(status)){
        return status;
    }
    status = CopyPhysicalMemory((CHAR8[]){FAT32_BASE_OEM}, 8, buffer->oemName);
    if (BT_ERROR(status)){
        return status;
    }
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
    if (BT_ERROR(status)){
        return status;
    }
    status = CopyPhysicalMemory((CHAR8[]){FAT32_BASE_FAT_NAME}, 8, buffer->fatName);
    if (BT_ERROR(status)){
        return status;
    }
    buffer->bootRecordSignature = FAT32_BASE_BOOT_SIGNATURE;

    return BT_SUCCESS;
}

BT_STATUS FAT32_Setup(IN IO_DISK *disk){
    BT_STATUS status;

    VOID *firstSector = NULL;
    UINTN firstSectorSize = MBR_SIZE;
    status = AllocPhysicalPool(&firstSector, &firstSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;    
    VOID* partitionBuffer = NULL;
    UINTN partitionBufferSize = MBR_SIZE;
    status = AllocPhysicalPool((VOID**)&partitionBuffer, &partitionBufferSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;    
    VOID *fsBootSector = NULL;
    UINTN fsBootSectorSize = MBR_SIZE;
    status = AllocPhysicalPool(&fsBootSector, &fsBootSectorSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;

    status = disk->functions.read(disk, 0, 1, (VOID**)&firstSector);
    if (BT_ERROR(status)) goto CLEANUP;    
    
    MBR_MODERN* fmbr = (MBR_MODERN*)firstSector;
    if (fmbr->signature != MBR_SIGNATURE){
        status = BT_IO_INVALID_SYMBOL;
        goto CLEANUP;
    }

    status = disk->functions.read(disk, fmbr->partitionEntry0.firstLba, 1, (VOID**)&firstSector);
    if (BT_ERROR(status)) goto CLEANUP;    

    MBR_PARTITION_ENTRY **partition = NULL;

    GPT_HEADER *gptHeader = (GPT_HEADER*)firstSector;
    if (gptHeader->signature == GPT_SIGNATURE_LITTLE || gptHeader->signature == GPT_SIGNATURE_BIG){
        UINT32 entryLba = gptHeader->startingLba + (disk->pciPartition * gptHeader->sizeOfEntry);

        status = FAT32_GET_BOOT_SECTOR((FAT32_BOOT_SECTOR*)fsBootSector);
        if (BT_ERROR(status)) goto CLEANUP;
        status = disk->functions.write(disk, entryLba, 1, fsBootSector);
        if (BT_ERROR(status)) goto CLEANUP;
    }
    else{
        switch (disk->pciPartition)
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
                break;
        }
    }

    CLEANUP:
    if (fsBootSector) FreePhysicalPool((VOID**)&fsBootSector, &fsBootSectorSize);
    if (partitionBuffer) FreePhysicalPool((VOID**)&partitionBuffer, &partitionBufferSize);
    if (firstSector) FreePhysicalPool((VOID**)&firstSector, &firstSectorSize);

    return status;
}