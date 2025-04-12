#pragma once

#include "byteos.h"
#include "pci.h"

#define IO_MAX_DISKS 26

typedef struct IO_DISK IO_DISK;

typedef BT_STATUS (ByteAPI *IO_DISK_READ)(
    IN IO_DISK *disk,
    IN UINT64 lba,
    IN UINT32 count,
    OUT VOID **buffer
);
typedef BT_STATUS (ByteAPI *IO_DISK_WRITE)(
    IN IO_DISK *disk,
    IN UINT64 lba,
    IN UINT32 count,
    IN VOID *buffer
);
typedef BT_STATUS (ByteAPI *IO_DISK_RESET)(
    IN IO_DISK *disk
);

typedef struct IO_DISK_FUNCTIONS{
    IO_DISK_READ read;
    IO_DISK_WRITE write;
    IO_DISK_RESET reset;
} IO_DISK_FUNCTIONS;
typedef struct IO_DISK{
    UINTN size;
    CHAR8 symbol;
    UINT8 filesystem;
    PCI *pci;
    UINT16 pciPartition;
    IO_DISK_FUNCTIONS functions;
} IO_DISK;

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS ByteAPI RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count);
BT_STATUS ByteAPI RegisterDisk(IN PCI *pci, IN CHAR8 *symbol, OUT IO_DISK *disk);

BT_STATUS ByteAPI GetDisk(IN CHAR8 symbol, OUT IO_DISK *disk);

// ==================================== |
//         STATIC DISK INTERFACE        |
// ==================================== |

#define IO_DISK_COMMAND_READ 0x00
#define IO_DISK_COMMAND_WRITE 0x01

BT_STATUS ByteAPI IO_DISK_IF_READ(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, OUT VOID **buffer);
BT_STATUS ByteAPI IO_DISK_IF_WRITE(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer);

BT_STATUS ByteAPI IO_DISK_IF_SATA(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID **buffer, IN UINT16 commandType);