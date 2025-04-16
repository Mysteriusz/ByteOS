#pragma once

#include "byteos.h"
#include "tasking/process.h"

#define PAGE_SIZE 0x1000 // 4 KiB RAM
#define MAX_PAGES 0x400000 // 16 GiB RAM
#define PAGES_PER_GROUP 0x08 // 8 Pages
#define MAX_PAGE_GROUPS (MAX_PAGES / PAGES_PER_GROUP) // 524,288 Groups

typedef BOOLEAN BT_MEMORY_PAGE_ALLOCATION;
#define PAGE_FREE 0
#define PAGE_ALLOCATED 1
#define BLOCK_FREE 0
#define BLOCK_ALLOCATED 1

typedef UINT8 BT_MEMORY_PAGE_FLAGS;
#define BT_MEMORY_READ 0x01
#define BT_MEMORY_WRITE 0x02
#define BT_MEMORY_EXECUTE 0x04
#define BT_MEMORY_USER 0x08
#define BT_MEMORY_KERNEL 0x10

#define BT_MEMORY_RW (BT_MEMORY_READ | BT_MEMORY_WRITE)
#define BT_MEMORY_RWX (BT_MEMORY_READ | BT_MEMORY_WRITE | BT_MEMORY_EXECUTE)

#define BT_MEMORY_USER_RW (BT_MEMORY_USER | BT_MEMORY_RW)
#define BT_MEMORY_KERNEL_RW (BT_MEMORY_KERNEL | BT_MEMORY_RW)

#define BT_MEMORY_USER_RWX (BT_MEMORY_USER | BT_MEMORY_RWX)
#define BT_MEMORY_KERNEL_RWX (BT_MEMORY_KERNEL | BT_MEMORY_RWX)

typedef struct MEMORY_PAGE{
    BT_MEMORY_PAGE_ALLOCATION allocation;
    BT_MEMORY_PAGE_FLAGS flags;
    UINT16 attributes;
    PHYSICAL_ADDRESS physicalAddress;
    UINT32 index;
} MEMORY_PAGE;

#define POOL_BLOCKS_PER_MAP 0x08 // 8 Bits
#define MAX_MAP_SIZE 32 // 32 Maps

typedef struct MEMORY_PAGE_POOL_HEADER MEMORY_PAGE_POOL_HEADER;
typedef struct MEMORY_PAGE_POOL_HEADER{
    MEMORY_PAGE_POOL_HEADER *next;
    UINT16 blockSize;
    UINT8 used;
    UINT8 poolMap[MAX_MAP_SIZE];
} MEMORY_PAGE_POOL_HEADER;

#define POOL_TINY_BLOCK_SIZE 0x10 // 16 Bytes
#define POOL_SMALL_BLOCK_SIZE 0x20 // 32 Bytes
#define POOL_MEDIUM_BLOCK_SIZE 0x40 // 64 Bytes
#define POOL_BIG_BLOCK_SIZE 0x80 // 128 Bytes
#define POOL_HUGE_BLOCK_SIZE 0x100 // 256 Bytes

#define POOL_BLOCK_SIZE(bytes) \
( \
    ((UINT32)bytes) <= POOL_TINY_BLOCK_SIZE ? POOL_TINY_BLOCK_SIZE : \
    ((UINT32)bytes) <= POOL_SMALL_BLOCK_SIZE ? POOL_SMALL_BLOCK_SIZE : \
    ((UINT32)bytes) <= POOL_MEDIUM_BLOCK_SIZE ? POOL_MEDIUM_BLOCK_SIZE : \
    ((UINT32)bytes) <= POOL_BIG_BLOCK_SIZE ? POOL_BIG_BLOCK_SIZE : \
    ((UINT32)bytes) <= POOL_HUGE_BLOCK_SIZE ? POOL_HUGE_BLOCK_SIZE : \
    ((UINT32)bytes) \
)
#define POOL_BLOCK_COUNT(blockSize) ((PAGE_SIZE - sizeof(MEMORY_PAGE_POOL_HEADER)) / (UINT32)blockSize)

#define POOL_TINY_BLOCK_COUNT (POOL_BLOCK_COUNT(POOL_TINY_BLOCK_SIZE))
#define POOL_SMALL_BLOCK_COUNT (POOL_BLOCK_COUNT(POOL_SMALL_BLOCK_SIZE))
#define POOL_MEDIUM_BLOCK_COUNT (POOL_BLOCK_COUNT(POOL_MEDIUM_BLOCK_SIZE))
#define POOL_BIG_BLOCK_COUNT (POOL_BLOCK_COUNT(POOL_BIG_BLOCK_SIZE))
#define POOL_HUGE_BLOCK_COUNT (POOL_BLOCK_COUNT(POOL_HUGE_BLOCK_SIZE))

// ==================================== |
//               PHYSICAL               |
// ==================================== |

#define PAGE_PAD_ADDRESS(address)(((PHYSICAL_ADDRESS)address & ~(PAGE_SIZE - 1)))

BT_STATUS ByteAPI InitializePhysicalMemory(KERNEL_MEMORY_MAP *memMap);
BT_STATUS ByteAPI InitializePhysicalPages(KERNEL_MEMORY_MAP *memMap);
BT_STATUS ByteAPI InitializePhysicalPool();

BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags);
BT_STATUS ByteAPI FreePhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size);
MEMORY_PAGE ByteAPI GetPhysicalPage(IN UINT32 index);

BT_STATUS ByteAPI AllocPhysicalPool(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags);
BT_STATUS ByteAPI FreePhysicalPool(IN OUT VOID **buffer, IN OUT UINTN *size);
MEMORY_PAGE_POOL_HEADER ByteAPI GetPhysicalPool(IN UINT32 index, IN UINT32 poolSize);

BT_STATUS ByteAPI ForceSetPhysicalMemory(IN VOID *buffer, IN BYTE value, IN UINTN size);
BT_STATUS ByteAPI SetPhysicalMemory(IN VOID *buffer, IN BYTE value, IN UINTN size);
BT_STATUS ByteAPI GetPhysicalFlags(IN VOID *buffer, OUT UINT8 *flags);

BT_STATUS ByteAPI CopyPhysicalMemory(IN VOID *from, IN UINTN size, IN OUT VOID *to);

// ==================================== |
//           PHYSICAL HELPERS           |
// ==================================== |

BT_STATUS ByteAPI PhysicalIndexToPage(IN UINT32 pageIndex, OUT PHYSICAL_ADDRESS *address);
BT_STATUS ByteAPI PhysicalPageToIndex(IN PHYSICAL_ADDRESS pageAddress, OUT UINT32 *index);
BT_STATUS ByteAPI PhysicalGetClosest(IN UINT32 fromIndex, OUT UINT32 *index, OUT PHYSICAL_ADDRESS *address);

// ==================================== |
//            PHYSICAL DEBUG            |
// ==================================== |

VOID DEBUG_ALLOC(UINT32 index);
VOID DEBUG_FREE(UINT32 index);
PHYSICAL_ADDRESS DEBUG_CLOSEST();

// ==================================== |
//                VIRTUAL               |
// ==================================== |

// BT_STATUS ByteAPI AllocVirtualPages(IN PROCESS_HANDLE handle, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags);