#include "Memory.h"

UINT64 pageMapCount = 0;

PHYSICAL_ADDRESS furthestAddress = 0;
PHYSICAL_ADDRESS closestAddress = 0;

UINT8 pageMap[MAX_PAGES];
UINT8 protectionMap[MAX_PAGES];
// UINT16 indexMap[MAX_PAGES];

#define PAGE_ADDRESS_FROM_INDEX(index)(FIRST_PAGE_OFFSET + ((UINTN)index * PAGE_SIZE))

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
                
                pageMap[memMapIndex] = PAGE_FREE;
                protectionMap[memMapIndex] = BT_PROTECTION_NONE;
                
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
    
    PHYSICAL_ADDRESS firstPageAddress = 0;

    while (i < pageMapCount){
        if (pageMap[i] == PAGE_FREE){
            pageMap[i] = PAGE_ALLOCATED;
            protectionMap[i] = protectionLevel;

            if (allocated == 0){
                firstPageAddress = PAGE_ADDRESS_FROM_INDEX(i);
            }
            allocated++;

            if (allocated == allocPageCount){
                *buffer = (VOID*)firstPageAddress;
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
    while (PAGE_ADDRESS_FROM_INDEX(pageIndex) != pageAddress){
        if (pageIndex >= pageMapCount) return BT_UNKNOWN_MEMORY;
        pageIndex++;
    }

    if (freePageCount > pageMapCount) return BT_NOT_ENOUGH_MEMORY;

    while (pageIndex < pageMapCount){
        if (pageMap[pageIndex] == PAGE_ALLOCATED){
            if (protectionMap[pageIndex] > accessLevel){
                return BT_ACCESS_VIOLATION;
            }

            pageMap[pageIndex] = PAGE_FREE;

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
    while (PAGE_ADDRESS_FROM_INDEX(pageIndex) != pageAddress){
        if (pageIndex >= pageMapCount) return BT_UNKNOWN_MEMORY;
        pageIndex++;
    }

    for (UINTN i = 0; i < count; i++){
        if (protectionMap[pageIndex + i] > accessLevel) return BT_ACCESS_VIOLATION;        

        PHYSICAL_ADDRESS phys = PAGE_ADDRESS_FROM_INDEX(pageIndex + i);
        for (PHYSICAL_ADDRESS pb = phys; pb < phys + PAGE_SIZE; pb += sizeof(UINTN)){
            *((UINTN*)pb) = 0;
        }
    }

    return BT_SUCCESS;
}

MEMORY_PAGE ByteAPI GetPage(UINTN index){
    MEMORY_PAGE page;
    page.allocationStatus = pageMap[index];
    page.physicalAddress = PAGE_ADDRESS_FROM_INDEX(index);
    page.protectionLevel = 0;
    page.attributes = 0;

    return page;
}