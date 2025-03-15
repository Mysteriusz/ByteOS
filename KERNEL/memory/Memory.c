#include "Memory.h"

#define PAGE_SIZE 0x1000 // 4 KiB RAM
#define MAX_PAGES 0x2000000 // 128 GiB RAM

#define PAGE_FREE 0
#define PAGE_ALLOCATED 1

typedef struct MEMORY_PAGE{
    UINT64 protectionLevel;
    UINT64 attributes;
    UINT32 size;
    PHYSICAL_ADDRESS physicalAddress;
} MEMORY_PAGE;

UINT64 pageMapCount = 0;
PHYSICAL_ADDRESS pageMap[MAX_PAGES];

UINT8 allocationMap[MAX_PAGES];
UINT64 attributeMap[MAX_PAGES];
UINT64 protectionMap[MAX_PAGES];

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap){
    UINTN memMapIndex = 0;

    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            PHYSICAL_ADDRESS pos = desc.physicalStart;
            PHYSICAL_ADDRESS end = desc.physicalStart + desc.size;

            while (pos + PAGE_SIZE <= end){
                if (memMapIndex >= PAGE_SIZE) return BT_MEMORY_OVERFLOW_ERROR;

                attributeMap[memMapIndex] = 0;
                protectionMap[memMapIndex] = 0;
                allocationMap[memMapIndex] = PAGE_FREE;
                pageMap[memMapIndex] = pos;

                memMapIndex++;
                pageMapCount++;

                pos += PAGE_SIZE;
            }
        }
    }

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPages(IN OUT VOID **buffer, IN OUT UINTN size){
    UINTN allocPageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;
    UINTN i = 0;
    
    if (allocPageCount > pageMapCount) return BT_NOT_ENOUGH_MEMORY;
    
    while (i < pageMapCount){
        if (allocationMap[i] == PAGE_FREE){
            allocationMap[i] = PAGE_ALLOCATED;
            allocated++;

            if (allocated == allocPageCount){
                *buffer = (VOID*)pageMap[i - (allocPageCount - 1)];
                return BT_SUCCESS;
            }
        }

        i++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}