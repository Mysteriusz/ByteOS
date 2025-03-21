#include "Memory.h"

UINT64 pageCount = 0;
UINT64 pageGroupCount = 0;
UINT64 pageSectionCount = 0;
PHYSICAL_ADDRESS closestAddress = 0;

MEMORY_SECTION pageSections[MAX_PAGE_SECTIONS];
UINT8 pageGroups[MAX_PAGE_GROUPS];
UINT8 flagMap[MAX_PAGES];

#define PAGE_PAD_ADDRESS(address)(((PHYSICAL_ADDRESS)address & ~(PAGE_SIZE - 1)))
#define PAGE_ALLOC(index)(pageGroups[((UINT64)index / PAGES_PER_GROUP)] |= (PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP)))
#define PAGE_DEALLOC(index)(pageGroups[((UINT64)index / PAGES_PER_GROUP)] &= ~(PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP)))
#define PAGE_CHECK(index)((pageGroups[((UINT64)index / PAGES_PER_GROUP)] & (PAGE_ALLOCATED << ((UINT64)index % PAGES_PER_GROUP))) != 0)

// ==================================== |
//               PHYSICAL               |
// ==================================== |

BT_STATUS ByteAPI InitializePhysicalMemory(KERNEL_MEMORY_MAP *memMap){
    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            PHYSICAL_ADDRESS pos = desc.physicalStart;
            PHYSICAL_ADDRESS end = pos + desc.size;

            if (pageSectionCount == 0){
                closestAddress = pos;
            }

            pageSections[pageSectionCount++] = (MEMORY_SECTION){.start = pos, .end = end};

            while (pos + PAGE_SIZE <= end){
                if (pageGroupCount >= MAX_PAGES) return BT_MEMORY_OVERFLOW;
                
                PAGE_DEALLOC(pageCount);
                pageCount++;

                pos += PAGE_SIZE;
            }
        }
    }

    pageGroupCount = pageCount / PAGES_PER_GROUP;

    return BT_SUCCESS;
}
BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *count, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN allocPageCount = (*count + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;

    UINTN i = PAGE_INDEX_FROM_ADDRESS(closestAddress);

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
BT_STATUS ByteAPI FreePhysicalPages(IN VOID *buffer, IN OUT UINTN *count){
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

                BT_STATUS clStatus = ClearPhysicalPages((VOID*)pageAddress, freed);
                if (BT_ERROR(clStatus)){
                    return clStatus;
                }
                
                return BT_SUCCESS;
            }
        }

        i++;
    }

    return BT_NOT_ENOUGH_MEMORY;
}
BT_STATUS ByteAPI ClearPhysicalPages(IN VOID *buffer, IN UINTN count){
    BYTE *ptr = (BYTE*)buffer;
    for (UINTN i = 0; i < count * PAGE_SIZE; i++) {
        ptr[i] = 0x00;
    }
    return BT_SUCCESS;
}
MEMORY_PAGE ByteAPI GetPhysicalPage(UINT64 index){
    MEMORY_PAGE page;
    page.allocation = PAGE_CHECK(index);
    page.flags = flagMap[index];
    page.physicalAddress = PAGE_ADDRESS_FROM_INDEX(index);
    page.attributes = 0;
    page.index = PAGE_INDEX_FROM_ADDRESS(page.physicalAddress);

    return page;
}

// ==================================== |
//                HELPERS               |
// ==================================== |

PHYSICAL_ADDRESS PAGE_ADDRESS_FROM_INDEX(UINT64 pageIndex){
    UINTN sectionIndex = 0; 
    UINTN currPageIndex = 0; 
    
    while(sectionIndex < pageSectionCount){
        UINT64 sectionPageCount = (pageSections[sectionIndex].end - pageSections[sectionIndex].start) / PAGE_SIZE; 

        for (UINTN i = 0; i < sectionPageCount; i++){
            // SECTION WITH PAGE FOUND
            if (currPageIndex == pageIndex){
                return pageSections[sectionIndex].start + (PAGE_SIZE * i);
            }

            currPageIndex++;
        }

        sectionIndex++;
    }

    return UINT64_MAX;
}
UINT64 PAGE_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS pageAddress){
    UINT64 adr = pageAddress;

    if (adr == 0){
        return 0;
    }
    else{
        return adr / PAGE_SIZE;
    }
}
UINT64 PAGE_SECTION_OFFSET(PHYSICAL_ADDRESS pageAddress, UINT64 sectionIndex){
    if (pageAddress >= pageSections[sectionIndex].start && pageAddress < pageSections[sectionIndex].end) {
        return pageAddress - pageSections[sectionIndex].start;
    }

    if (pageAddress < pageSections[sectionIndex].start) {
        return pageSections[sectionIndex].start - pageAddress;
    }
    
    return pageAddress - pageSections[sectionIndex].end;
}
UINT64 PAGE_SECTION_INDEX(PHYSICAL_ADDRESS pageAddress){
    for (UINTN i = 0; i < pageSectionCount; i++){
        if (pageAddress >= pageSections[i].start && pageAddress < pageSections[i].end){
            return i;
        }    
    }
    return UINT64_MAX;
}

// ==================================== |
//                 DEBUG                |
// ==================================== |

VOID DEBUG_ALLOC(UINT64 index){
    PAGE_ALLOC(index);
}
VOID DEBUG_FREE(UINT64 index){
    PAGE_DEALLOC(index);
}
MEMORY_SECTION DEBUG_SECTION(UINT64 index){
    return pageSections[index];
}
UINT64 DEBUG_SECTION_INDEX(PHYSICAL_ADDRESS address){
    return PAGE_SECTION_INDEX(address);
}
UINT64 DEBUG_SECTION_OFFSET(PHYSICAL_ADDRESS address, UINT64 index){
    return PAGE_SECTION_OFFSET(address, index);
}
UINT64 DEBUG_ADDRESS_FROM_INDEX(UINT64 index){
    return PAGE_ADDRESS_FROM_INDEX(index);
}
