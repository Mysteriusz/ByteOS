#pragma once

#include "byteos.h"

#define MBR_SIZE 0x200

#define MBR_SIGNATURE_LITTLE 0xaa55
#define MBR_SIGNATURE_BIG 0x55aa
#define MBR_MAX_PARTITIONS 0x04

#define MBR_COPY_PROTECTED 0x5a5a

typedef enum MBR_PARTITION_TYPES{
    MBR_EMPTY_PARTITION = 0x00,
    MBR_NTFS = 0x07,
    MBR_FAT32_CHS = 0x0b,
    MBR_FAT32_LBA = 0x0c,
    MBR_EXT_LBA = 0x0f,
    MBR_LINUX_SS = 0x82,
    MBR_LINUX_FS = 0x83,
} MBR_PARTITION_TYPES;

// Prevent compiler from padding MBR_CHS_ADDRESS to 4 bytes 
#pragma pack(1)

typedef struct MBR_CHS_ADDRESS{
    UINT8 chsHead;
    UINT8 sector : 6;
    UINT8 cylinderHigh : 2;
    UINT8 cylinderLow;
} MBR_CHS_ADDRESS;

typedef struct MBR_PARTITION_ENTRY{
    UINT8 status;
    MBR_CHS_ADDRESS chsFirst;
    UINT8 partitionType;
    MBR_CHS_ADDRESS chsLast;
    UINT32 firstLba;
    UINT32 numberOfSectors;
} MBR_PARTITION_ENTRY;

typedef struct MBR_CLASSIC{
    UINT8 bootstrapCode[446];
    MBR_PARTITION_ENTRY partitionEntry0;
    MBR_PARTITION_ENTRY partitionEntry1;
    MBR_PARTITION_ENTRY partitionEntry2;
    MBR_PARTITION_ENTRY partitionEntry3;
    UINT16 signature;
} MBR_CLASSIC;

typedef struct MBR_MODERN{
    UINT8 bootstrapCode0[218];
    UINT16 reserved0;
    UINT8 originalPhysicalDrive;
    UINT8 seconds;
    UINT8 minutes;
    UINT8 hours;
    UINT8 bootstrapCode1[216];
    UINT32 diskSignature;
    UINT16 copyProtected;
    MBR_PARTITION_ENTRY partitionEntry0;
    MBR_PARTITION_ENTRY partitionEntry1;
    MBR_PARTITION_ENTRY partitionEntry2;
    MBR_PARTITION_ENTRY partitionEntry3;
    UINT16 signature;
} MBR_MODERN;

#pragma pack()