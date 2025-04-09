#pragma once

#include "byteos.h"

#define GPT_SIGNATURE_BIG 0x4546492050415254
#define GPT_SIGNATURE_LITTLE 0x5452415020494645

typedef struct GPT_HEADER{
    UINT64 signature;
    UINT32 revisionNumber;
    UINT32 headerSize;
    UINT32 crc32;
    UINT32 reserved0;
    UINT64 currentLba;
    UINT64 backupLba;
    UINT64 firstUsableLba;
    UINT64 lastUsableLba;
    UINT8 diskGuid[16];
    UINT64 startingLba;
    UINT32 numberOfEntries;
    UINT32 sizeOfEntry;
    UINT32 crc32Entry;
} GPT_HEADER;
typedef struct GPT_PARTITON_ENTRY{
    UINT8 partitionTypeGuid[16];
    UINT8 uniquePartitionGuid[16];
    UINT64 firstLba;
    UINT64 lastLba;
    UINT64 attributeFlags;
    CHAR16 partitionName[72];
} GPT_PARTITON_ENTRY;