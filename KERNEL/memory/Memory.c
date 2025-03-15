#include "Memory.h"

UINT64 pageMapCount = 0;
MEMORY_PAGE pageMap[MAX_PAGES];

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap){
    UINTN memMapIndex = 0;

    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            PHYSICAL_ADDRESS pos = desc.physicalStart + FIRST_PAGE_OFFSET;

            PHYSICAL_ADDRESS end = pos + desc.size;

            while (pos + PAGE_SIZE <= end){
                if (pageMapCount >= MAX_PAGES) return BT_MEMORY_OVERFLOW;
                
                pageMap[memMapIndex].allocationStatus = PAGE_FREE;
                pageMap[memMapIndex].protectionLevel = BT_PROTECTION_NONE;
                pageMap[memMapIndex].attributes = 0;
                pageMap[memMapIndex].physicalAddress = pos;
                
                memMapIndex++;
                pageMapCount++;

                pos += PAGE_SIZE;
            }
        }
    }

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPages(IN OUT VOID **buffer, IN OUT UINTN *count, IN BT_MEMORY_PROTECTION_LEVEL protectionLevel){
    UINTN allocPageCount = (*count + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;
    UINTN i = 0;
    
    if (allocPageCount > pageMapCount) return BT_NOT_ENOUGH_MEMORY;
    
    while (i < pageMapCount){
        if (pageMap[i].allocationStatus == PAGE_FREE){
            pageMap[i].allocationStatus = PAGE_ALLOCATED;
            pageMap[i].protectionLevel = protectionLevel;

            allocated++;

            if (allocated == allocPageCount){
                *buffer = (VOID*)pageMap[i - (allocPageCount - 1)].physicalAddress;
                *count = allocated;

                return BT_SUCCESS;
            }
        }

        i++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}
BT_STATUS ByteAPI FreePages(IN VOID *buffer, IN OUT UINTN *count, IN BT_MEMORY_ACCESS_LEVEL accessLevel){
    UINTN freePageCount = (*count + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN freed = 0;
    
    UINTN pageIndex = 0;
    PHYSICAL_ADDRESS pageAddress = (PHYSICAL_ADDRESS)buffer;
    pageAddress &= ~(PAGE_SIZE - 1);
    while (pageMap[pageIndex].physicalAddress != pageAddress){
        if (pageIndex >= pageMapCount) return BT_UNKNOWN_MEMORY;
        pageIndex++;
    }

    if (freePageCount > pageMapCount) return BT_NOT_ENOUGH_MEMORY;

    while (pageIndex < pageMapCount){
        if (pageMap[pageIndex].allocationStatus == PAGE_ALLOCATED){
            if (pageMap[pageIndex].protectionLevel > accessLevel){
                return BT_ACCESS_VIOLATION;
            }

            pageMap[pageIndex].allocationStatus = PAGE_FREE;

            freed++;

            if (freed == freePageCount){
                *count = freed;
                return BT_SUCCESS;
            }
        }

        pageIndex++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}
BT_STATUS ByteAPI ClearPages(IN VOID *ptr, IN UINTN count, IN BT_MEMORY_ACCESS_LEVEL accessLevel){
    PHYSICAL_ADDRESS pageAddress = (PHYSICAL_ADDRESS)ptr;
    pageAddress &= ~(PAGE_SIZE - 1);

    UINTN pageIndex = 0;
    while (pageMap[pageIndex].physicalAddress != pageAddress){
        if (pageIndex >= pageMapCount) return BT_UNKNOWN_MEMORY;
        pageIndex++;
    }

    for (UINTN i = 0; i < count; i++){
        if (pageMap[pageIndex + i].protectionLevel > accessLevel) return BT_ACCESS_VIOLATION;        

        for (PHYSICAL_ADDRESS pb = pageMap[pageIndex + i].physicalAddress; pb < pageMap[pageIndex + i].physicalAddress + PAGE_SIZE; pb += sizeof(UINTN)){
            *((UINTN*)pb) = 0;
        }
    }

    return BT_SUCCESS;
}

MEMORY_PAGE ByteAPI GetPage(UINTN index){
    return pageMap[index];
}