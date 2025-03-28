#pragma once

#include "byteos.h"

#define IO_MAX_DISKS 26
#define IO_MAX_BUS 6

typedef enum FS_TYPE{
    NONE,
    FAT32,
} FS_TYPE;

typedef struct IO_DISK{
    BOOLEAN initialized;
    UINTN size;
    CHAR8 symbol;
    PCIE pcie;
    FS_TYPE fileSystem;
} IO_DISK;

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS RecognizeDisk(IN PCIE pcie, IN CHAR8 *symbol, OUT IO_DISK *disk);
BT_STATUS SetupFileSystem(IN FS_TYPE type, IN OUT IO_DISK *disk);