#include "Memory.h"

UINT64 pageCount = 0;
UINT64 pageGroupCount = 0;
PHYSICAL_ADDRESS closestAddress = 0;

UINT8 pageGroups[MAX_PAGE_GROUPS];
UINT8 flagMap[MAX_PAGES];

#define PAGE_PAD_ADDRESS(address)(((PHYSICAL_ADDRESS)address & ~(PAGE_SIZE - 1)))

#define PAGE_ALLOC(index)(pageGroups[((UINT64)index / PAGES_PER_GROUP)] |= (PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP)))
#define PAGE_DEALLOC(index)(pageGroups[((UINT64)index / PAGES_PER_GROUP)] &= ~(PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP)))
#define PAGE_CHECK(index)((pageGroups[((UINT64)index / PAGES_PER_GROUP)] & (PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP))) != 0)

PHYSICAL_ADDRESS PAGE_ADDRESS_FROM_INDEX(UINT64 index);
UINT64 PAGE_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS address);

PHYSICAL_ADDRESS PAGE_ADDRESS_FROM_INDEX(UINT64 index){
    return FIRST_PAGE_OFFSET + (index * PAGE_SIZE);
}
UINT64 PAGE_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS address){
    UINT64 adr = address - FIRST_PAGE_OFFSET;

    if (adr == 0){
        return 0;
    }
    else{
        return adr / PAGE_SIZE;
    }
}

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap){
    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            PHYSICAL_ADDRESS pos = desc.physicalStart + FIRST_PAGE_OFFSET;
            PHYSICAL_ADDRESS end = pos + desc.size;

            while (pos + PAGE_SIZE <= end){
                if (pageGroupCount >= MAX_PAGES) return BT_MEMORY_OVERFLOW;
                
                pageGroups[pageCount] = PAGE_FREE;
                pageCount++;

                pos += PAGE_SIZE;
            }
        }
    }

    pageGroupCount = pageCount / PAGES_PER_GROUP;
    closestAddress = FIRST_PAGE_OFFSET;

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPages(IN OUT VOID **buffer, IN OUT UINTN *count, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN allocPageCount = (*count + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;

    UINTN i = PAGE_INDEX_FROM_ADDRESS(closestAddress);
    UINTN fi = i;

    if (allocPageCount > pageCount) return BT_NOT_ENOUGH_MEMORY;
    
    PHYSICAL_ADDRESS firstPageAddress = 0;
    PHYSICAL_ADDRESS lastPageAddress = 0;

    BOOLEAN safe = FALSE;

    while (i < pageCount){
        ALLOCATE:
        if (safe){
            PAGE_ALLOC(i);
            flagMap[i] = flags;
            
            if (allocated == 0){
                firstPageAddress = PAGE_ADDRESS_FROM_INDEX(i);
            }
            allocated++;
            
            if (allocated == allocPageCount){
                lastPageAddress = PAGE_ADDRESS_FROM_INDEX(i);
                *buffer = (VOID*)firstPageAddress;
                *count = allocated * PAGE_SIZE;
                
                if (firstPageAddress == closestAddress){
                    closestAddress = lastPageAddress;
                }
                
                return BT_SUCCESS;
            }
        }
        // SAFETY CHECK
        else{
            UINTN sfci = i;
            UINTN sfcallocated = 0;
            while (sfcallocated < allocPageCount){
                if (PAGE_CHECK(sfci) == PAGE_ALLOCATED){
                    goto SKIP_PAGE;
                }  
                sfci++;
                sfcallocated++;
                
                if (sfcallocated == allocPageCount){
                    safe = TRUE;
                    goto ALLOCATE;
                }
            }
        }
        SKIP_PAGE:
        
        i++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}
BT_STATUS ByteAPI FreePages(IN VOID *buffer, IN OUT UINTN *count, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN freePageCount = (*count + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN freed = 0;

    PHYSICAL_ADDRESS pageAddress = PAGE_PAD_ADDRESS(buffer);
    UINTN i = PAGE_INDEX_FROM_ADDRESS(pageAddress);

    if (freePageCount > pageCount) return BT_NOT_ENOUGH_MEMORY;
    
    while (i < pageCount){
        if (PAGE_CHECK(i) == PAGE_ALLOCATED){   
            PAGE_DEALLOC(i);
            
            freed++;
            
            if (freed == freePageCount){
                *count = freed * PAGE_SIZE;

                if (closestAddress > pageAddress){
                    closestAddress = pageAddress;
                }

                return BT_SUCCESS;
            }
        }

        i++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}

VOID DEBUG_ALLOC(UINT64 index){
    PAGE_ALLOC(index);
}
VOID DEBUG_FREE(UINT64 index){
    PAGE_DEALLOC(index);
}
PHYSICAL_ADDRESS DEBUG_CLOSEST(){
    return closestAddress;
}

MEMORY_PAGE ByteAPI GetPage(UINT64 index){
    MEMORY_PAGE page;
    page.allocation = PAGE_CHECK(index);
    page.flags = flagMap[index];
    page.physicalAddress = PAGE_ADDRESS_FROM_INDEX(index);
    page.attributes = 0;
    page.index = PAGE_INDEX_FROM_ADDRESS(page.physicalAddress);

    return page;
}