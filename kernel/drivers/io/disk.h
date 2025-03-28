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
    UINT32 bus[IO_MAX_BUS];
    FS_TYPE fileSystem;
} IO_DISK;

BT_STATUS RecognizeDisk(IN UINT32 bus[IO_MAX_BUS], IN CHAR8 *symbol, OUT IO_DISK *disk);
BT_STATUS SetupFileSystem(IO_DISK *disk, FS_TYPE type);