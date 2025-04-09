#pragma once

#include "byteos.h"

#define FAT32_BASE_OEM "BOSS1.0"
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
#define FAT32_BASE_BOOT_SIGNATURE 0x29 
#define FAT32_BASE_VOLUME_NAME "UNK" 
#define FAT32_BASE_FAT_NAME "FAT32" 
#define FAT32_BASE_BOOT_SIGNATURE 0x55aa     

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
    UINT32 numberOfSectorsPerFat;
    UINT16 flags;
    UINT16 versionOfFatDrive;
    UINT32 clusterNumberOfStartof;
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
typedef struct FAT32_LFN_ENTRY{
    BYTE sequenceNumber;
    CHAR16 nameCharacters0[5];
    BYTE attributes;
    BYTE type;
    BYTE checksum;
    CHAR16 nameCharacters1[6];
    UINT16 firstCluster;
    CHAR16 nameCharacters2[2];
} FAT32_LFN_ENTRY;

BT_STATUS FAT32_GET_BOOT_SECTOR(IN OUT FAT32_BOOT_SECTOR *buffer){
    // buffer->oemName;
}