#pragma once

#include "byteos.h"

typedef struct FAT32_INFORMATION_SECTOR{
    UINT32 signature0;
    BYTE reserved0[480];
    UINT32 signature1;
    UINT32 freeClusters;
    UINT32 recentClusters;
    BYTE reserved1[12];
    UINT32 signature2;
} FAT32_INFORMATION_SECTOR;

typedef struct LFN_ENTRY{
    BYTE sequenceNumber;
    CHAR16 nameCharacters0[5];
    BYTE attributes;
    BYTE type;
    BYTE checksum;
    CHAR16 nameCharacters1[6];
    UINT16 firstCluster;
    CHAR16 nameCharacters2[2];
} LFN_ENTRY;