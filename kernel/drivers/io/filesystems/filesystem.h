#pragma once

#include "byteos.h"
#include "fat32.h"
#include "disk.h"
#include "mbr.h"
#include "gpt.h"
#include "../pci.h"

typedef enum FS_TYPE{
    NONE = 0x00,
    FAT32 = 0x32,
} FS_TYPE;

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI SetupFilesystem(IN IO_DISK *disk, IN UINT16 partitionIndex, IN FS_TYPE type);