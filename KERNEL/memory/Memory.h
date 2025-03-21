#include "ByteOS.h"

#define MAX_PAGE_SECTIONS 0x40 // 64 Sections
#define PAGE_SIZE 0x1000 // 4 KiB RAM
#define MAX_PAGES 0x400000 // 16 GiB RAM
#define PAGES_PER_GROUP 0x08 // 8 Pages
#define MAX_PAGE_GROUPS MAX_PAGES / PAGES_PER_GROUP // 524,288 Groups

typedef BOOLEAN BT_MEMORY_PAGE_ALLOCATION;
#define PAGE_FREE 0
#define PAGE_ALLOCATED 1

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

typedef struct MEMORY_SECTION{
    PHYSICAL_ADDRESS start;
    PHYSICAL_ADDRESS end;
} MEMORY_SECTION;

typedef struct MEMORY_PAGE{
    BT_MEMORY_PAGE_ALLOCATION allocation;
    BT_MEMORY_PAGE_FLAGS flags;
    UINT8 attributes;
    PHYSICAL_ADDRESS physicalAddress;
    UINT64 index;
} MEMORY_PAGE;

// ==================================== |
//               PHYSICAL               |
// ==================================== |

BT_STATUS ByteAPI InitializePhysicalMemory(KERNEL_MEMORY_MAP *memMap);
BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *count, IN BT_MEMORY_PAGE_FLAGS flags);
BT_STATUS ByteAPI FreePhysicalPages(IN VOID *buffer, IN OUT UINTN *count);
BT_STATUS ByteAPI ClearPhysicalPages(IN VOID *address, IN UINTN count);
MEMORY_PAGE ByteAPI GetPhysicalPage(UINT64 index);

// ==================================== |
//                HELPERS               |
// ==================================== |

PHYSICAL_ADDRESS PAGE_ADDRESS_FROM_INDEX(UINT64 index);
PHYSICAL_ADDRESS PAGE_SECTION_ADDRESS_FROM_INDEX(UINT64 pageIndex);
UINT64 PAGE_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS address);
UINT64 PAGE_SECTION_OFFSET(PHYSICAL_ADDRESS pageAddress, UINT64 sectionIndex);
UINT64 PAGE_SECTION_INDEX(PHYSICAL_ADDRESS pageAddress);

// ==================================== |
//                 DEBUG                |
// ==================================== |

VOID DEBUG_ALLOC(UINT64 index);
VOID DEBUG_FREE(UINT64 index);
MEMORY_SECTION DEBUG_SECTION(UINT64 index);
UINT64 DEBUG_SECTION_INDEX(PHYSICAL_ADDRESS address);
UINT64 DEBUG_SECTION_OFFSET(PHYSICAL_ADDRESS address, UINT64 index);
UINT64 DEBUG_ADDRESS_FROM_INDEX(UINT64 index);