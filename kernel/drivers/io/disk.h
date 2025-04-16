#pragma once

#include "byteos.h"
#include "pci.h"

#define IO_MAX_DISKS 26

typedef struct IO_DISK IO_DISK;

typedef struct IO_DISK_INFO_DATA{
    UINT32 logicalBlockCount;
    UINT32 logicalBlockSize;
} IO_DISK_INFO_DATA;

typedef BT_STATUS (ByteAPI *IO_DISK_INFO)(
    IN IO_DISK *disk,
    IN OUT VOID *buffer
);
typedef BT_STATUS (ByteAPI *IO_DISK_READ)(
    IN IO_DISK *disk,
    IN UINT64 lba,
    IN UINT32 count,
    IN OUT VOID *buffer
);
typedef BT_STATUS (ByteAPI *IO_DISK_WRITE)(
    IN IO_DISK *disk,
    IN UINT64 lba,
    IN UINT32 count,
    IN VOID *buffer
);
typedef BT_STATUS (ByteAPI *IO_DISK_FLUSH)(
    IN IO_DISK *disk
);
typedef BT_STATUS (ByteAPI *IO_DISK_RESET)(
    IN IO_DISK *disk
);

#define IO_DISK_SCHEME_UNK 0x00
#define IO_DISK_SCHEME_MBR 0x01
#define IO_DISK_SCHEME_GPT 0x02

typedef struct IO_DISK_FUNCTIONS{
    IO_DISK_INFO info;
    IO_DISK_READ read;
    IO_DISK_WRITE write;
    IO_DISK_FLUSH flush;
    IO_DISK_RESET reset;
} IO_DISK_FUNCTIONS;
typedef struct IO_DISK{
    UINT8 filesystem;
    UINT8 partitionScheme;
    UINT8 partitionIndex;
    PCI *pci;
    IO_DISK_FUNCTIONS functions;
    IO_DISK_INFO_DATA info;
} IO_DISK;

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS ByteAPI RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count);
BT_STATUS ByteAPI RegisterDisk(IN PCI *pci, OUT IO_DISK **disk);

BT_STATUS ByteAPI GetDisk(IN UINT32 index, OUT IO_DISK **disk);

// ==================================== |
//         DISK METHODS INTEFACE        |
// ==================================== |

#define IO_DISK_COMMAND_INFO 0x00
#define IO_DISK_COMMAND_READ 0x01
#define IO_DISK_COMMAND_WRITE 0x02
#define IO_DISK_COMMAND_FLUSH 0x03

BT_STATUS ByteAPI IO_DISK_IF_INFO(IN IO_DISK *disk, IN OUT VOID *buffer);
BT_STATUS ByteAPI IO_DISK_IF_READ(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer);
BT_STATUS ByteAPI IO_DISK_IF_WRITE(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer);
BT_STATUS ByteAPI IO_DISK_IF_FLUSH(IN IO_DISK *disk);

BT_STATUS ByteAPI IO_DISK_IF_SATA(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer, IN UINT16 commandType);