#include "memory.h"

UINT32 closestPageIndex = 0;
PHYSICAL_ADDRESS closestPageAddress = 0;

UINT8 pageGroups[MAX_PAGE_GROUPS]; // BIT PER PAGE
UINT32 pageGroupCount = 0;
UINT8 flagMap[MAX_PAGES]; // BYTE PER PAGE
UINT32 pageCount = 0;

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

#define POOL_BLOCK_ALLOC(poolMap, index)(((UINT8*)poolMap)[(UINT32)index / POOL_BLOCKS_PER_MAP] |= (BLOCK_ALLOCATED << ((UINT32)index % POOL_BLOCKS_PER_MAP)))
#define POOL_BLOCK_DEALLOC(poolMap, index)(((UINT8*)poolMap)[(UINT32)index / POOL_BLOCKS_PER_MAP] &= ~(BLOCK_ALLOCATED << ((UINT32)index % POOL_BLOCKS_PER_MAP)))
#define POOL_BLOCK_CHECK(poolMap, index)(((((UINT8*)poolMap)[(UINT32)index / POOL_BLOCKS_PER_MAP] & (BLOCK_ALLOCATED << ((UINT32)index % POOL_BLOCKS_PER_MAP))) != 0))

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
    BOOLEAN first = TRUE;
    pageCount = 0;

    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        PHYSICAL_ADDRESS pos = desc.physicalStart;
        PHYSICAL_ADDRESS end = pos + desc.size;

        while (pos <= end){
            if (pageCount >= MAX_PAGES) return BT_MEMORY_OVERFLOW;
            
            // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
            if (desc.type == 7 || desc.type == 3 || desc.type == 4){
                if (first){
                    PhysicalGetClosest(pageCount, &closestPageIndex, &closestPageAddress);
                    first = FALSE;
                }
                PAGE_DEALLOC(pageCount);
            }
            else{
                flagMap[pageCount] = BT_MEMORY_KERNEL_RW;
                PAGE_ALLOC(pageCount);
            }
            
            pageCount++;
            pos += PAGE_SIZE;
        }
    }

    pageGroupCount = pageCount / PAGES_PER_GROUP;

    return BT_SUCCESS;
}
BT_STATUS ByteAPI InitializePhysicalPool(){
    tinyPool = NULL;
    smallPool = NULL;
    mediumPool = NULL;
    bigPool = NULL;
    hugePool = NULL;
    poolCount = 0;

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN allocPageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN allocated = 0;
    UINT32 firstPageIndex = 0;
    UINTN i = closestPageIndex;
    
    if (allocPageCount > pageCount) return BT_NOT_ENOUGH_MEMORY;
    
    // SAFETY CHECK
    UINTN sfci = i;
    UINTN sfcallocated = 0;
    while (sfci < pageCount){
        if (PAGE_CHECK(sfci) == PAGE_ALLOCATED){
            i++;
            sfci = i;
            sfcallocated = 0;
            continue;
        }  
        sfcallocated++;
        
        if (sfcallocated == allocPageCount){
            break;
        }
        sfci++;
    }
    
    while (allocated < allocPageCount){
        PAGE_ALLOC(i);
        flagMap[i] = flags;
        
        if (allocated == 0){
            firstPageIndex = i;
        }
        i++;
        allocated++;
    }

    PHYSICAL_ADDRESS adr = 0;
    PhysicalIndexToPage(firstPageIndex, &adr);
    *buffer = (VOID*)adr;

    *size = allocated * PAGE_SIZE;
    
    if (firstPageIndex == closestPageIndex){
        PhysicalGetClosest(closestPageIndex, &closestPageIndex, &closestPageAddress);
    }
    
    return BT_SUCCESS;
}
BT_STATUS ByteAPI FreePhysicalPages(IN VOID *buffer, IN OUT UINTN *size){
    UINTN freePageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE;
    UINTN freed = 0;

    PHYSICAL_ADDRESS pageAddress = PAGE_PAD_ADDRESS(buffer);
    UINT32 pageIndex = 0;
    PhysicalPageToIndex(pageAddress, &pageIndex);
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
                SetPhysicalMemory((VOID*)pageAddress, 0, *size);

                if (closestPageIndex > pageIndex){
                    closestPageIndex = pageIndex;
                    closestPageAddress = pageAddress;
                }
                
                return BT_SUCCESS;
            }
        }

        i++;
    }

    return BT_INVALID_BUFFER;
}
MEMORY_PAGE ByteAPI GetPhysicalPage(IN UINT32 index){
    MEMORY_PAGE page;
    page.allocation = PAGE_CHECK(index);
    page.flags = flagMap[index];
    PhysicalIndexToPage(index, &page.physicalAddress);
    page.attributes = 0;
    page.index = index;

    return page;
}

BT_STATUS ByteAPI AllocPhysicalPool(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags){
    MEMORY_PAGE_POOL_HEADER *curr = NULL;
    MEMORY_PAGE_POOL_HEADER **pPool = NULL;
    UINT32 blockSize = POOL_BLOCK_SIZE(*size);
    UINT32 blockCount = POOL_BLOCK_COUNT(blockSize);
    
    switch (blockSize)
    {
        case POOL_TINY_BLOCK_SIZE:
            pPool = &tinyPool; break;
        case POOL_SMALL_BLOCK_SIZE:
            pPool = &smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = &mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = &bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = &hugePool; break;
        default:
            return BT_INVALID_POOL_BLOCK_SIZE;
    }

    curr = *pPool;

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

            curr->used++;

            return BT_SUCCESS;
        }
        *pPool = curr;
        curr = curr->next;
    }

    UINTN poolSize = PAGE_SIZE;
    BT_STATUS status = AllocPhysicalPages((VOID**)pPool, &poolSize, BT_MEMORY_KERNEL_RWX);
    if (BT_ERROR(status)){
        return status;
    }

    curr = *pPool;
    curr->blockSize = blockSize;
    curr->next = NULL;

    if (*pPool != NULL){  
        MEMORY_PAGE_POOL_HEADER *prev = *pPool;
        while (prev->next) prev = prev->next;
        prev->next = curr;
    } else {
        *pPool = curr;
    }

    goto ALLOC;
}
BT_STATUS ByteAPI FreePhysicalPool(IN VOID *buffer, IN OUT UINTN *size){
    MEMORY_PAGE_POOL_HEADER *curr = NULL;
    MEMORY_PAGE_POOL_HEADER *prev = NULL;
    MEMORY_PAGE_POOL_HEADER **pPool = NULL;
    UINT32 blockSize = POOL_BLOCK_SIZE(*size);
    UINT32 blockCount = POOL_BLOCK_COUNT(blockSize);
    
    switch (blockSize)
    {
        case POOL_TINY_BLOCK_SIZE:
            pPool = &tinyPool; break;
        case POOL_SMALL_BLOCK_SIZE:
            pPool = &smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = &mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = &bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = &hugePool; break;
        default:
            return BT_INVALID_POOL_BLOCK_SIZE;
    }

    curr = *pPool;

    while (curr != NULL && ((PHYSICAL_ADDRESS)curr + PAGE_SIZE) <= (PHYSICAL_ADDRESS)buffer){
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL){
        return BT_INVALID_BUFFER;
    }

    UINT32 blockIndex = 0;
    UINT32 blockRva = sizeof(MEMORY_PAGE_POOL_HEADER);
    while (blockIndex < blockCount){
        if ((PHYSICAL_ADDRESS)buffer == (PHYSICAL_ADDRESS)curr + blockRva){
            POOL_BLOCK_DEALLOC(curr->poolMap, blockIndex);
            *size = blockSize;
            curr->used--;

            if (curr->used == 0){
                if (prev != NULL){
                    *prev->next = *curr->next; 
                } else {
                    *pPool = curr->next; 
                }

                UINTN s = PAGE_SIZE;
                BT_STATUS status = FreePhysicalPages(*pPool, &s);
                if (BT_ERROR(status)){
                    return status;
                }

                *pPool = NULL;
                return BT_SUCCESS;
            }
            else{
                SetPhysicalMemory(buffer, 0, blockSize);
            }

            return BT_SUCCESS;
        }

        blockIndex++;
        blockRva += blockSize;
    }

    return BT_INVALID_BUFFER;
}
MEMORY_PAGE_POOL_HEADER ByteAPI GetPhysicalPool(IN UINT32 index, IN UINT32 poolSize){
    MEMORY_PAGE_POOL_HEADER *pPool = NULL;
    UINT32 blockSize = POOL_BLOCK_SIZE(poolSize);
    
    switch (blockSize)
    {
        case POOL_TINY_BLOCK_SIZE:
            pPool = tinyPool; break;
        case POOL_SMALL_BLOCK_SIZE:
            pPool = smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = hugePool; break;
        default:
            return (MEMORY_PAGE_POOL_HEADER){0};
    }   

    UINT32 i = 0;
    while (i < index){
        pPool = pPool->next;
        i++;

        if (pPool == NULL){
            return (MEMORY_PAGE_POOL_HEADER){0};
        }
    }

    return *pPool;
}

BT_STATUS ByteAPI SetPhysicalMemory(IN VOID *buffer, IN BYTE value, IN UINTN size){
    BYTE *ptr = (BYTE*)buffer;

    UINT32 index = 0;
    PhysicalPageToIndex((PHYSICAL_ADDRESS)ptr, &index);

    // PERMISSION CHECK
    if ((flagMap[index] & BT_MEMORY_WRITE) == FALSE){
        return BT_MEMORY_NOT_WRITABLE;
    }
    
    for (UINTN i = 0; i < size; i++) {
        ptr[i] = value;
    }
    return BT_SUCCESS;
}

// ==================================== |
//           PHYSICAL HELPERS           |
// ==================================== |

BT_STATUS ByteAPI PhysicalIndexToPage(IN UINT32 pageIndex, OUT PHYSICAL_ADDRESS *address){
    if (pageIndex > pageCount){
        return BT_INVALID_PAGE_INDEX;
    }

    if (pageIndex == 0){
        return 0;
    }

    *address = PAGE_SIZE * pageIndex;  
    return BT_SUCCESS;
}
BT_STATUS ByteAPI PhysicalPageToIndex(IN PHYSICAL_ADDRESS pageAddress, OUT UINT32 *index){
    if (*index > pageCount){
        return BT_INVALID_PAGE_INDEX;
    }

    *index = PAGE_PAD_ADDRESS(pageAddress) / PAGE_SIZE;  
    return BT_SUCCESS;
}
BT_STATUS ByteAPI PhysicalGetClosest(IN UINT32 fromIndex, OUT UINT32 *index, OUT PHYSICAL_ADDRESS *address){
    UINT32 i = fromIndex;
    while (PAGE_CHECK(i) == PAGE_ALLOCATED){
        i++;
        if (i >= pageCount){
            return BT_INVALID_PAGE_INDEX;
        }
    }
    *index = i;
    *address = PAGE_SIZE * i;

    return BT_SUCCESS;
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
PHYSICAL_ADDRESS DEBUG_CLOSEST(){
    return closestPageAddress;
}

// ==================================== |
//                VIRTUAL               |
// ==================================== |

