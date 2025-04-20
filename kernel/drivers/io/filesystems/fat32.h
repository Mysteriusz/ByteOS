#pragma once

#include "filesystem.h"
#include "mbr.h"
#include "gpt.h"

#define FAT32_BASE_OEM "BOSS1.0"
#define FAT32_BASE_JUMP_CODE 0xEB, 0x58, 0x90
#define FAT32_BASE_BYTES_PER_SECTOR 0x200 
#define FAT32_BASE_SECTORS_PER_CLUSTER 0x10 
#define FAT32_BASE_RESERVED_SECTORS 0x20 
#define FAT32_BASE_NUMBER_OF_COPIES 0x02 
#define FAT32_BASE_MAX_ROOT_ENTRIES 0x00 
#define FAT32_BASE_NUMBER_OF_SMALLER_SECTORS 0x00 
#define FAT32_BASE_MEDIA_DESCRIPTOR 0xf8 
#define FAT32_BASE_SECTORS_PER_FAT 0x00 
#define FAT32_BASE_SECTORS_PER_TRACK 0x3f 
#define FAT32_BASE_NUMBER_OF_HEADS 0xff 
#define FAT32_BASE_FLAGS 0x00 
#define FAT32_BASE_VERSION 0x00 
#define FAT32_BASE_FILESYSTEM_INFO_SECTOR 0x01 
#define FAT32_BASE_BACKUP_BOOT_SECTOR 0x06 
#define FAT32_BASE_DRIVE_NUMBER 0x80 
#define FAT32_BASE_SERIAL_NUMBER 0x29 
#define FAT32_BASE_VOLUME_NAME "UNK" 
#define FAT32_BASE_FAT_NAME "FAT32" 
#define FAT32_BASE_BOOT_SIGNATURE 0xaa55     

#define FAT32_FAT_TABLES_LBA(fat32ptr, fat32lba)((UINT64)fat32lba + ((FAT32_BOOT_SECTOR*)fat32ptr)->reservedSectors)
#define FAT32_DATA_AREA_LBA(fat32ptr, fat32lba)((UINT64)FAT32_FAT_TABLES_LBA(fat32ptr, fat32lba) + (((FAT32_BOOT_SECTOR*)fat32ptr)->sectorsPerFat * 2))

#pragma pack(1)

typedef struct FAT32_BOOT_SECTOR{
    UINT8 jumpCode[3];
    CHAR8 oemName[8];
    UINT16 bytesPerSector;
    UINT8 sectorsPerCluster;
    UINT16 reservedSectors;
    UINT8 numberOfCopies;
    UINT16 maxRootEntries;
    UINT16 numberOfSmallerThan32;
    UINT8 mediaDescriptor;
    UINT16 sectorsPerFat;
    UINT16 sectorsPerTrack;
    UINT16 numberOfHeads;
    UINT32 numberOfHiddenSectorsInPartition;
    UINT32 numberOfSectorsInPartition;
    UINT32 numberOfSectorsPerFatInPartition;
    UINT16 flags;
    UINT16 versionOfFatDrive;
    UINT32 rootCluster;
    UINT16 sectorNumberOfFilesystemInfo;
    UINT16 sectorNumberOfBackupBoot;
    UINT8 reserved0[12];
    UINT8 logicalDriveNumberOfPartition;
    UINT8 unused;
    UINT8 extendedSignature;
    UINT32 serialNumberOfPartition;
    CHAR8 volumeNameOfPartition[11];
    CHAR8 fatName[8];
    UINT8 executableCode[420];
    UINT16 bootRecordSignature;
} FAT32_BOOT_SECTOR;
typedef struct FAT32_INFORMATION_SECTOR{
    UINT32 signature0;
    BYTE reserved0[480];
    UINT32 signature1;
    UINT32 freeClusters;
    UINT32 recentClusters;
    BYTE reserved1[12];
    UINT32 signature2;
} FAT32_INFORMATION_SECTOR;

#pragma pack()

#define FAT32_SFN_DELETED 0xe5

#define FAT32_SFN_READ_ONLY 0x01
#define FAT32_SFN_HIDDEN 0x02
#define FAT32_SFN_SYSTEM 0x04
#define FAT32_SFN_VOLUME 0x08
#define FAT32_SFN_DIRECTORY 0x10
#define FAT32_SFN_ARCHIVE 0x20
typedef struct FAT32_SFN_ENTRY{
    CHAR8 fileNameMain[8];
    CHAR8 fileNameExt[3];
    UINT8 fileAttributes;
    UINT8 reserved0;
    UINT8 creationTimeHndr;
    UINT16 creationTime;
    UINT16 creationDate;
    UINT16 lastAccessDate;
    UINT16 firstClusterHigh;
    UINT16 writeTime;
    UINT16 writeDate;
    UINT16 firstClusterLow;
    UINT32 fileSize;
} FAT32_SFN_ENTRY;
typedef struct FAT32_LFN_ENTRY{
    UINT8 entryOrder;
    CHAR16 fileName0[5];
    UINT8 fileAttributes;
    UINT8 type;
    UINT8 checkSum;
    CHAR16 fileName1[6];
    UINT16 reserved0;
    CHAR16 fileName2[2];
} FAT32_LFN_ENTRY;

BT_STATUS ByteAPI Fat32Setup(IN IO_DISK_PARTITION *partition);
BT_STATUS ByteAPI Fat32CreateBootSector(IN FAT32_BOOT_SECTOR *buffer);
BT_STATUS ByteAPI Fat32GetBootSector(IN IO_DISK_PARTITION *partition, OUT UINT64 *lba, IN OUT VOID *buffer);

BT_STATUS ByteAPI Fat32Create(IN IO_DISK_PARTITION *partition, IN CHAR8 *filename, IN CHAR16 *directory);
BT_STATUS ByteAPI Fat32Ls(IN IO_DISK_PARTITION *partition, IN CHAR16 *directory);
