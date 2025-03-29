#pragma once

#include "byteos.h"

#define IO_MAX_DISKS 26

typedef enum FS_TYPE{
    NONE,
    FAT32,
} FS_TYPE;

typedef struct IO_DISK{
    BOOLEAN free;
    UINTN size;
    CHAR8 symbol;
    FS_TYPE fileSystem;
    VOID *pcieAddress;
} IO_DISK;

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS RecognizeDisk(IN VOID *pcieAddress, IN CHAR8 *symbol, OUT IO_DISK *disk);
BT_STATUS SetupFileSystem(IN FS_TYPE type, IN OUT IO_DISK *disk);