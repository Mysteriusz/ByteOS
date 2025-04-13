#pragma once

#include "byteos.h"
#include "fat32.h"
#include "disk.h"
#include "mbr.h"
#include "gpt.h"
#include "pci.h"

typedef enum FS_TYPE{
    NONE = 0x00,
    FAT12 = 0x0c,
    FAT16 = 0x10,
    FAT32 = 0x20,
    EXFAT = 0x40,
} FS_TYPE;

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI SetupFilesystem(IN IO_DISK *disk, IN FS_TYPE type);