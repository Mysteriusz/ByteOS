#pragma once

#include "byteos.h"
#include "disk.h"

#define GPT_SIZE 0x200

#define GPT_SIGNATURE_BIG 0x4546492050415254
#define GPT_SIGNATURE_LITTLE 0x5452415020494645
#define GPT_MAX_PARTITIONS 0x80

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
    GUID typeGuid;
    GUID uniqueGuid;
    UINT64 firstLba;
    UINT64 lastLba;
    UINT64 attributeFlags;
    CHAR16 partitionName[36];
} GPT_PARTITON_ENTRY;

BOOLEAN ByteAPI IsGpt(IN IO_DISK *disk);

BT_STATUS ByteAPI GptIdentifyPartitions(IN IO_DISK *disk);

BT_STATUS ByteAPI GptWritePartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN UINT32 count, IN GPT_PARTITON_ENTRY *buffer);
BT_STATUS ByteAPI GptReadPartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN UINT32 count, IN GPT_PARTITON_ENTRY *buffer);

BT_STATUS ByteAPI GptSafeReadPartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN GPT_PARTITON_ENTRY *buffer);
BT_STATUS ByteAPI GptSafeWritePartitonEntry(IN IO_DISK *disk, IN UINT32 partitionIndex, IN GPT_PARTITON_ENTRY *buffer);

BT_STATUS ByteAPI GptFreeSize(IN IO_DISK *disk, OUT UINTN *freeSize);

BT_STATUS ByteAPI GptUpdateHeaderCrc(IN IO_DISK *disk);
