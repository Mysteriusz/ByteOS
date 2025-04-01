#pragma once

#include "byteos.h"
#include "drivers/pci.h"

#define IO_MAX_DISKS 26

typedef enum FS_TYPE{
    NONE,
    FAT32,
} FS_TYPE;

typedef struct IO_DISK{
    UINTN size;
    CHAR8 symbol;
    FS_TYPE fileSystem;
    PCI *pci;
} IO_DISK;

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count);
BT_STATUS RegisterDisk(IN PCI *pci, IN CHAR8 *symbol, OUT IO_DISK *disk);