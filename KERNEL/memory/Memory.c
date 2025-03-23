#include "Memory.h"

UINT32 pageCount = 0;

UINT32 closestPageIndex = 0;
PHYSICAL_ADDRESS closestPageAddress = 0;

MEMORY_SECTION pageSections[MAX_PAGE_SECTIONS]; // MEMORY_SECTION PER SECTION  
UINT32 pageSectionCount = 0;

UINT8 pageGroups[MAX_PAGE_GROUPS]; // BIT PER PAGE
UINT8 flagMap[MAX_PAGES]; // BYTE PER PAGE
UINT32 pageGroupCount = 0;

MEMORY_PAGE_POOL_HEADER *tinyPool;
MEMORY_PAGE_POOL_HEADER *smallPool;
MEMORY_PAGE_POOL_HEADER *mediumPool;
MEMORY_PAGE_POOL_HEADER *bigPool;
MEMORY_PAGE_POOL_HEADER *hugePool;
UINT32 poolCount = 0;

#define PAGE_PAD_ADDRESS(address)(((PHYSICAL_ADDRESS)address & ~(PAGE_SIZE - 1)))
#define PAGE_ALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] |= (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
#define PAGE_DEALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] &= ~(PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
#define PAGE_CHECK(index)((pageGroups[((UINT32)index / PAGES_PER_GROUP)] & (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP))) != 0)

#define POOL_BLOCK_ALLOC(poolMap, index)(((UINT8*)poolMap)[(UINT32)(index) / POOL_BLOCKS_PER_MAP] |= (BLOCK_ALLOCATED << ((UINT32)(index) % POOL_BLOCKS_PER_MAP)))
// #define POOL_BLOCK_DEALLOC(poolMap, index)()
#define POOL_BLOCK_CHECK(poolMap, index)(((((UINT8*)poolMap)[(UINT32)(index) / POOL_BLOCKS_PER_MAP] & (BLOCK_ALLOCATED << ((UINT32)(index) % POOL_BLOCKS_PER_MAP))) != 0))
// ==================================== |
//               PHYSICAL               |
// ==================================== |

BT_STATUS ByteAPI InitializePhysicalMemory(KERNEL_MEMORY_MAP *memMap){
    BT_STATUS status = InitializePhysicalPages(memMap);
    if (BT_ERROR(status)){
        return status;
    }
    
    status = InitializePhysicalPool(memMap);
    if (BT_ERROR(status)){
        return status;
    }

    return BT_SUCCESS;
}
BT_STATUS ByteAPI InitializePhysicalPages(KERNEL_MEMORY_MAP *memMap){
    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            PHYSICAL_ADDRESS pos = desc.physicalStart;
            PHYSICAL_ADDRESS end = pos + desc.size;

            if (pageSectionCount == 0){
                closestPageAddress = pos;
                closestPageIndex = 0;
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
BT_STATUS ByteAPI InitializePhysicalPool(KERNEL_MEMORY_MAP *memMap){
    smallPool = NULL;
    mediumPool = NULL;
    bigPool = NULL;
    hugePool = NULL;

    UINTN poolSize = PAGE_SIZE;

    BT_STATUS status = AllocPhysicalPages((VOID**)&tinyPool, &poolSize, BT_MEMORY_KERNEL_RWX);
    if (BT_ERROR(status)){
        return status;
    }

    poolCount = 1;

    // TODO: Custom user defined initial pool metadata via CONFIG
    tinyPool->next = NULL;
    tinyPool->blockSize = POOL_TINY_BLOCK_SIZE;

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN allocPageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;

    UINTN i = closestPageIndex;

    if (allocPageCount > pageCount) return BT_NOT_ENOUGH_MEMORY;
    
    UINT32 firstPageIndex = 0;

    BOOLEAN safe = FALSE;

    while (i < pageCount){
        ALLOCATE:
        if (safe){
            PAGE_ALLOC(i);
            flagMap[i] = flags;
            
            if (allocated == 0){
                firstPageIndex = i;
            }
            allocated++;
            
            if (allocated == allocPageCount){
                *buffer = (VOID*)PAGE_ADDRESS_FROM_INDEX(firstPageIndex);
                *size = allocated * PAGE_SIZE;
                
                if (firstPageIndex == closestPageIndex){
                    PAGE_UPDATE_CLOSEST();
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
BT_STATUS ByteAPI FreePhysicalPages(IN VOID *buffer, IN OUT UINTN *size){
    UINTN freePageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN freed = 0;

    PHYSICAL_ADDRESS pageAddress = PAGE_PAD_ADDRESS(buffer);
    PHYSICAL_ADDRESS pageIndex = PAGE_INDEX_FROM_ADDRESS(pageAddress);
    UINTN i = pageIndex;

    if (freePageCount > pageCount) return BT_NOT_ENOUGH_MEMORY;
    
    // PERMISSION CHECK
    UINTN pi = i;
    while (pi < freePageCount){
        if ((flagMap[pi++] & BT_MEMORY_WRITE) == FALSE){
            return BT_MEMORY_NOT_READABLE;
        }
    }

    while (i < pageCount){
        if (PAGE_CHECK(i) == PAGE_ALLOCATED){   
            PAGE_DEALLOC(i);
            
            freed++;
            
            if (freed == freePageCount){
                *size = freed * PAGE_SIZE;

                if (closestPageIndex > pageIndex){
                    closestPageIndex = pageIndex;
                    closestPageAddress = pageAddress;
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
BT_STATUS ByteAPI ClearPhysicalPages(IN VOID *buffer, IN UINTN size){
    BYTE *ptr = (BYTE*)PAGE_PAD_ADDRESS((PHYSICAL_ADDRESS)buffer);

    // PERMISSION CHECK
    if ((flagMap[PAGE_INDEX_FROM_ADDRESS((PHYSICAL_ADDRESS)ptr)] & BT_MEMORY_WRITE) == FALSE){
        return BT_MEMORY_NOT_WRITABLE;
    }
    
    for (UINTN i = 0; i < size * PAGE_SIZE; i++) {
        ptr[i] = 0x00;
    }
    return BT_SUCCESS;
}
MEMORY_PAGE ByteAPI GetPhysicalPage(UINT32 index){
    MEMORY_PAGE page;
    page.allocation = PAGE_CHECK(index);
    page.flags = flagMap[index];
    page.physicalAddress = PAGE_ADDRESS_FROM_INDEX(index);
    page.attributes = 0;
    page.index = PAGE_INDEX_FROM_ADDRESS(page.physicalAddress);

    return page;
}

BT_STATUS ByteAPI AllocPhysicalPool(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags){
    MEMORY_PAGE_POOL_HEADER *curr = NULL;
    MEMORY_PAGE_POOL_HEADER *prev = NULL;
    UINT32 blockSize = POOL_BLOCK_SIZE(*size);
    UINT32 blockCount = POOL_BLOCK_COUNT(blockSize);
    
    switch (blockSize)
    {
        case POOL_TINY_BLOCK_SIZE:
            curr = tinyPool; break;
        case POOL_SMALL_BLOCK_SIZE:
            curr = smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            curr = mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            curr = bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            curr = hugePool; break;
        default:
            return BT_INVALID_POOL_BLOCK_SIZE;
    }

    ALLOC:

    while (curr != NULL){
        UINT32 blockIndex = 0;
        UINT32 blockRva = sizeof(MEMORY_PAGE_POOL_HEADER);
         
        while (blockIndex < blockCount){
            if (POOL_BLOCK_CHECK(curr->poolMap, blockIndex) == BLOCK_ALLOCATED){
                blockIndex++;
                blockRva += blockSize;
                continue;
            }
            
            POOL_BLOCK_ALLOC(curr->poolMap, blockIndex);
            *buffer = (VOID*)((PHYSICAL_ADDRESS)curr + blockRva);
            *size = blockSize;

            return BT_SUCCESS;
        }       
        prev = curr;
        curr = curr->next;
    }

    UINTN poolSize = PAGE_SIZE;
    BT_STATUS status = AllocPhysicalPages((VOID**)&curr, &poolSize, BT_MEMORY_KERNEL_RWX);
    if (BT_ERROR(status)){
        return status;
    }

    curr->blockSize = blockSize;
    curr->next = NULL;
    if (prev != NULL){
        prev->next = curr; 
    }

    goto ALLOC;
}

// ==================================== |
//           PHYSICAL HELPERS           |
// ==================================== |

PHYSICAL_ADDRESS PAGE_ADDRESS_FROM_INDEX(UINT32 pageIndex){
    UINTN sectionIndex = 0; 
    UINTN currPageIndex = 0; 
    
    while(sectionIndex < pageSectionCount){
        UINT32 sectionPageCount = (pageSections[sectionIndex].end - pageSections[sectionIndex].start) / PAGE_SIZE; 

        if (pageIndex >= currPageIndex && pageIndex < currPageIndex + sectionPageCount){
            // SECTION WITH PAGE FOUND
            UINT32 offset = (pageIndex - currPageIndex) * PAGE_SIZE;
            return pageSections[sectionIndex].start + offset;
        }
        
        currPageIndex += sectionPageCount;
        sectionIndex++;
    }

    return UINT32_MAX;
}
UINT32 PAGE_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS pageAddress){
    UINTN sectionIndex = 0; 
    UINTN currPageIndex = 0; 
    
    while(sectionIndex < pageSectionCount){
        UINT32 sectionPageCount = (pageSections[sectionIndex].end - pageSections[sectionIndex].start) / PAGE_SIZE; 

        if (pageAddress >= pageSections[sectionIndex].start && pageAddress < pageSections[sectionIndex].end){
            // SECTION WITH PAGE FOUND
            UINT32 offset = pageAddress - pageSections[sectionIndex].start;
            return currPageIndex + offset / PAGE_SIZE;
        }

        currPageIndex += sectionPageCount;
        sectionIndex++;
    }

    return UINT32_MAX;
}
PHYSICAL_ADDRESS PAGE_SECTION_OFFSET(PHYSICAL_ADDRESS pageAddress, UINT32 sectionIndex){
    if (pageAddress >= pageSections[sectionIndex].start && pageAddress < pageSections[sectionIndex].end) {
        return pageAddress - pageSections[sectionIndex].start;
    }

    if (pageAddress < pageSections[sectionIndex].start) {
        return pageSections[sectionIndex].start - pageAddress;
    }
    
    return pageAddress - pageSections[sectionIndex].end;
}
UINT32 PAGE_SECTION_INDEX(PHYSICAL_ADDRESS pageAddress){
    for (UINTN i = 0; i < pageSectionCount; i++){
        if (pageAddress >= pageSections[i].start && pageAddress < pageSections[i].end){
            return i;
        }    
    }
    return UINT32_MAX;
}
VOID PAGE_UPDATE_CLOSEST(){
    while (PAGE_CHECK(closestPageIndex) == PAGE_ALLOCATED){
        closestPageIndex++;
    }
    closestPageAddress = PAGE_ADDRESS_FROM_INDEX(closestPageIndex);
}

// ==================================== |
//            PHYSICAL DEBUG            |
// ==================================== |

VOID DEBUG_ALLOC(UINT32 index){
    PAGE_ALLOC(index);
}
VOID DEBUG_FREE(UINT32 index){
    PAGE_DEALLOC(index);
}
MEMORY_SECTION DEBUG_SECTION(UINT32 index){
    return pageSections[index];
}
UINT32 DEBUG_SECTION_INDEX(PHYSICAL_ADDRESS address){
    return PAGE_SECTION_INDEX(address);
}
PHYSICAL_ADDRESS DEBUG_SECTION_OFFSET(PHYSICAL_ADDRESS address, UINT32 index){
    return PAGE_SECTION_OFFSET(address, index);
}
PHYSICAL_ADDRESS DEBUG_ADDRESS_FROM_INDEX(UINT32 index){
    return PAGE_ADDRESS_FROM_INDEX(index);
}
UINT32 DEBUG_INDEX_FROM_ADDRESS(PHYSICAL_ADDRESS address){
    return PAGE_INDEX_FROM_ADDRESS(address);
}
PHYSICAL_ADDRESS DEBUG_CLOSEST(){
    return closestPageAddress;
}
