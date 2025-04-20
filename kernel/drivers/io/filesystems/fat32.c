#include "fat32.h"

BT_STATUS ByteAPI Fat32Setup(IN IO_DISK_PARTITION *partition){
    if (partition == NULL) return BT_INVALID_ARGUMENT;
    if (partition->filesystem.type == FAT32) return BT_IO_INVALID_DISK;

    BT_STATUS status = 0;

    VOID *bootSectorBlock = NULL;
    UINTN bootSectorBlockSize = FIT_IN_SIZE(sizeof(FAT32_BOOT_SECTOR), partition->disk->info.logicalBlockSize);
    UINT64 lba = 0;
    status = AllocPhysicalPool(&bootSectorBlock, &bootSectorBlockSize, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)) goto CLEANUP;
    
    // status = Fat32GetBootSector(partition, &lba, bootSectorBlock);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = Fat32CreateBootSector((FAT32_BOOT_SECTOR*)bootSectorBlock);
    if (BT_ERROR(status)) goto CLEANUP;
    
    status = partition->disk->io.write(partition->disk, lba, FIT_IN(bootSectorBlockSize, partition->disk->info.logicalBlockSize), bootSectorBlock);
    if (BT_ERROR(status)) goto CLEANUP;
        
    CLEANUP:
    if (BT_ERROR(status) && bootSectorBlock){
        FreePhysicalPool(&bootSectorBlock, &bootSectorBlockSize);
    }
    else{
        partition->filesystem.bootSectorData = bootSectorBlock;
        partition->filesystem.bootSectorDataSize = bootSectorBlockSize;
        partition->filesystem.bootSectorLba = lba;
        partition->filesystem.type = FAT32;
    }
    
    return status;
}
BT_STATUS ByteAPI Fat32CreateBootSector(IN FAT32_BOOT_SECTOR *buffer){
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
    buffer->rootCluster = 0x00;
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

BT_STATUS ByteAPI Fat32Create(IN IO_DISK_PARTITION *partition, IN CHAR8 *filename, IN CHAR16 *directory){
    if (partition == NULL) return BT_INVALID_ARGUMENT;
    if (directory == NULL) return BT_INVALID_ARGUMENT;
    if (filename == NULL) return BT_INVALID_ARGUMENT;
    if (partition->filesystem.type != FAT32) return BT_IO_INVALID_DISK;
    
    // return FAT32_DATA_AREA_LBA(partition->filesystem.bootSectorData, partition->filesystem.bootSectorLba);
    return partition->filesystem.bootSectorLba;
}
BT_STATUS ByteAPI Fat32Ls(IN IO_DISK_PARTITION *partition, IN CHAR16 *directory){
    if (partition == NULL) return BT_INVALID_ARGUMENT;
    if (directory == NULL) return BT_INVALID_ARGUMENT;
    if (partition->filesystem.type != FAT32) return BT_IO_INVALID_DISK;
    
    return ((FAT32_BOOT_SECTOR*)partition->filesystem.bootSectorData)->rootCluster;
}
