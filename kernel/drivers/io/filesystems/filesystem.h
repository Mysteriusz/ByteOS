#pragma once

#include "byteos.h"
#include "pci.h"

typedef enum FILE_SYSTEM_TYPE{
    NONE = 0x00,
    FAT12 = 0x0c,
    FAT16 = 0x10,
    FAT32 = 0x20,
    EXFAT = 0x40,
} FILE_SYSTEM_TYPE;

typedef struct FILE_SYSTEM{
    UINT8 type;
    UINTN bootSectorLba;
    UINT32 bootSectorDataSize;
    BYTE *bootSectorData;
} FILE_SYSTEM;

#include "disk.h"

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI FilesystemSetup(IN IO_DISK_PARTITION *partition, IN FILE_SYSTEM_TYPE type);
