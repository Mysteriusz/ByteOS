#include "Memory.h"

UINT32 pageCount = 0;

UINT32 closestPageIndex = 0;
PHYSICAL_ADDRESS closestPageAddress = 0;

MEMORY_SECTION pageSections[MAX_PAGE_SECTIONS]; // MEMORY_SECTION PER SECTION  
UINT32 pageSectionCount = 0;

UINT8 pageGroups[MAX_PAGE_GROUPS]; // BIT PER PAGE
UINT8 flagMap[MAX_PAGES]; // BYTE PER PAGE
UINT32 pageGroupCount = 0;

BOOLEAN freePoolsInitialized;
MEMORY_PAGE_POOL freePools;
MEMORY_PAGE_POOL allocPools;
UINT32 poolCount = 0;

#define PAGE_PAD_ADDRESS(address)(((PHYSICAL_ADDRESS)address & ~(PAGE_SIZE - 1)))
#define PAGE_ALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] |= (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
#define PAGE_DEALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] &= ~(PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
#define PAGE_CHECK(index)((pageGroups[((UINT32)index / PAGES_PER_GROUP)] & (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP))) != 0)

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

    freePools.address = 0;
    freePools.blocks = NULL;
    freePools.blockCount = 0;
    freePools.next = NULL;
    freePoolsInitialized = FALSE;

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
    MEMORY_PAGE_POOL *curr = &freePools;
    MEMORY_PAGE_POOL *prev = NULL;

    if (*size > PAGE_SIZE){
        return BT_NOT_ENOUGH_MEMORY;
    }

    while (curr != NULL){
        UINTN blockIndex = 0;
        MEMORY_PAGE_POOL_BLOCK *block = curr->blocks;
        MEMORY_PAGE_POOL_BLOCK *prevBlock = NULL;

        while (blockIndex < curr->blockCount){
            PHYSICAL_ADDRESS blockStartAddress = curr->address + block->rva;
            // CHECK IF THERE IS ENOUGH MEMORY BETWEEN CURRENT AND PREVIOUS BLOCK
            if (prevBlock != NULL && blockStartAddress - (curr->address + prevBlock->rva + prevBlock->size) >= *size){
                MEMORY_PAGE_POOL_BLOCK newBlock = (MEMORY_PAGE_POOL_BLOCK){
                    .next = block,
                    .rva = prevBlock->rva + prevBlock->size,
                    .size = *size
                };

                *buffer = (VOID*)(curr->address + block->next->rva); 
                curr->blockCount++;
                prevBlock->next = &newBlock;
                return 1;
            }

            // CHECK IF THE BLOCK IS LAST AND THERE IS ENOUGH MEMORY 
            if (block->next == NULL && blockStartAddress + block->size - curr->address <= *size){
                block->next = &(MEMORY_PAGE_POOL_BLOCK){
                    .next = NULL,
                    .rva = block->rva + block->size,
                    .size = *size
                };

                *buffer = (VOID*)(curr->address + block->next->rva); 
                curr->blockCount++;
                return 2;
            }
            else if (blockStartAddress + block->size - curr->address <= *size) {
                goto NEW_POOL;
            }
            
            prevBlock = block;
            block = block->next;
            blockIndex++;
        }

        prev = curr;    
        curr = curr->next;
        continue;    
    }
    
    NEW_POOL:
    // CREATE NEW POOL IF NONE WITH ENOUGH SPACE FOUND
    PAGE_ALLOC(closestPageIndex);
    PHYSICAL_ADDRESS poolAddress = closestPageAddress;
    PAGE_UPDATE_CLOSEST();

    MEMORY_PAGE_POOL_BLOCK block = (MEMORY_PAGE_POOL_BLOCK){
        .rva = 0,
        .size = *size,
        .next = NULL,
    };

    MEMORY_PAGE_POOL newPool = (MEMORY_PAGE_POOL){
        .blockCount = 1,
        .blocks = &block,
        .next = NULL,
        .address = poolAddress
    };

    if (freePoolsInitialized == FALSE){
        freePools = newPool;
        freePoolsInitialized = TRUE;
    }

    MEMORY_PAGE_POOL *last = &newPool;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = &newPool;
    
    *buffer = (VOID*)poolAddress;
    return BT_SUCCESS;
}
BT_STATUS ByteAPI FreePhysicalPool(IN VOID *buffer, IN OUT UINTN *size){
    MEMORY_PAGE_POOL *curr = &freePools;
    MEMORY_PAGE_POOL *prev = NULL;

    if (*size > PAGE_SIZE){
        return BT_NOT_ENOUGH_MEMORY;
    }

    while (curr != NULL){
        UINTN blockIndex = 0;
        MEMORY_PAGE_POOL_BLOCK *block = curr->blocks;
        MEMORY_PAGE_POOL_BLOCK *prevBlock = NULL;

        while (blockIndex < curr->blockCount){
            PHYSICAL_ADDRESS blockStartAddress = curr->address + block->rva;

            if (blockStartAddress == (PHYSICAL_ADDRESS)buffer){
                // CHECK IF THERE IS A PREVIOUS BLOCK
                if (prevBlock != NULL){
                    prevBlock->next = block->next;
                    curr->blockCount--;
                    return 1;
                }
                // CHECK IF CURRENT BLOCK IS THE FIRST ON IN THE POOL
                else if (block->next != NULL){
                    curr->blocks = block->next;
                    curr->blockCount--;
                    return 2;
                }
                // CHECK IF CURRENT BLOCK IS THE ONLY ONE IN THE POOL AND FREE THE ENTIRE POOL
                else{
                    if (prev != NULL){
                        prev->next = curr->next;
                    }
                    goto FREE_POOL;
                }

                prevBlock = block;
                block = block->next;
                blockIndex++;
            }

            prevBlock = block;
            block = block->next;
        }

        prev = curr;
        curr = curr->next;
    }

    FREE_POOL:

    return 3;
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
MEMORY_PAGE_POOL *DEBUG_GET_FREE_POOLS(){
    return &freePools;
}
PHYSICAL_ADDRESS DEBUG_CLOSEST(){
    return closestPageAddress;
}
