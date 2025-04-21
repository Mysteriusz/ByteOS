#pragma once

typedef struct IO_DISK_PARTITION IO_DISK_PARTITION;
typedef struct IO_DISK IO_DISK;

#include "byteos.h"
#include "filesystem.h"
#include "pci.h"

#define IO_MAX_DISKS 26

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
typedef struct IO_DISK_INFO_DATA{
    UINT32 logicalBlockCount;
    UINT32 logicalBlockSize;
} IO_DISK_INFO_DATA;
typedef struct IO_DISK{
    UINT8 scheme;
    VOID *mbr;
    UINT8 index;
    IO_DISK_FUNCTIONS io;
    IO_DISK_INFO_DATA info;
    UINT32 initializedPartitions[4];
    PCI *pci;
} IO_DISK;
typedef struct IO_DISK_PARTITION{
    CHAR8 name[16];
    UINT8 partitionIndex;
    IO_DISK* disk;
    FILE_SYSTEM filesystem;
} IO_DISK_PARTITION;

#define IO_DISK_PARTITION_BIT_SET(diskptr, partitionIndex)((diskptr)->initializedPartitions[(partitionIndex) / 32] |= (1 << ((partitionIndex) % 32)))
#define IO_DISK_PARTITION_BIT_UNSET(diskptr, partitionIndex)((diskptr)->initializedPartitions[(partitionIndex) / 32] &= ~(1 << ((partitionIndex) % 32)))
#define IO_DISK_PARTITION_BIT_CHECK(diskptr, partitionIndex)((diskptr)->initializedPartitions[(partitionIndex) / 32] & (1 << ((partitionIndex) % 32)))

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS ByteAPI InjectDisk(IN PCI *pci, OUT IO_DISK **disk);
BT_STATUS ByteAPI EjectDisk(IN IO_DISK **disk);

// ==================================== |
//         DISK METHODS INTEFACE        |
// ==================================== |

#define IO_DISK_COMMAND_INFO 0x00
#define IO_DISK_COMMAND_READ 0x01
#define IO_DISK_COMMAND_WRITE 0x02
#define IO_DISK_COMMAND_FLUSH 0x03

BT_STATUS ByteAPI DiskIfInfo(IN IO_DISK *disk, IN OUT VOID *buffer);
BT_STATUS ByteAPI DiskIfRead(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer);
BT_STATUS ByteAPI DiskIfWrite(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer);
BT_STATUS ByteAPI DiskIfFlush(IN IO_DISK *disk);

BT_STATUS ByteAPI DiskIfSata(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID *buffer, IN UINT16 commandType);