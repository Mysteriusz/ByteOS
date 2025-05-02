#include "memory.h"

UINT32 closestPageIndex = 0;
PHYSICAL_ADDRESS closestPageAddress = 0;

UINT8 pageGroups[MAX_PAGE_GROUPS]; // BIT PER PAGE
UINT32 pageGroupCount = 0;
UINT8 flagMap[MAX_PAGES]; // BYTE PER PAGE
UINT32 pageCount = 0;

MEMORY_PAGE_POOL_HEADER *smallPool;
MEMORY_PAGE_POOL_HEADER *mediumPool;
MEMORY_PAGE_POOL_HEADER *bigPool;
MEMORY_PAGE_POOL_HEADER *hugePool;
MEMORY_PAGE_POOL_HEADER *customPool;
UINT32 poolCount = 0;

// Used for allocating page using page bit map
#define PAGE_ALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] |= (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
// Used for deallocating page using page bit map
#define PAGE_DEALLOC(index)(pageGroups[((UINT32)index / PAGES_PER_GROUP)] &= ~(PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP)))
// Used for checking page using page bit map (0 - not allocated; 1 - allocated)
#define PAGE_CHECK(index)((pageGroups[((UINT32)index / PAGES_PER_GROUP)] & (PAGE_ALLOCATED << ((UINT32)index % PAGES_PER_GROUP))) != 0)

// Used for allocating pool block using pool`s bit map
#define POOL_BLOCK_ALLOC(poolMap, index)(((UINT8*)poolMap)[(UINT32)index / POOL_BLOCKS_PER_MAP] |= (BLOCK_ALLOCATED << ((UINT32)index % POOL_BLOCKS_PER_MAP)))
// Used for deallocating pool block using pool`s bit map
#define POOL_BLOCK_DEALLOC(poolMap, index)(((UINT8*)poolMap)[(UINT32)index / POOL_BLOCKS_PER_MAP] &= ~(BLOCK_ALLOCATED << ((UINT32)index % POOL_BLOCKS_PER_MAP)))
// Used for checking pool block using pool`s bit map (0 - not allocated; 1 - allocated)
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
    if (memMap == NULL) return BT_INVALID_ARGUMENT;

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
    smallPool = NULL;
    mediumPool = NULL;
    bigPool = NULL;
    hugePool = NULL;
    poolCount = 0;

    return BT_SUCCESS;
}

BT_STATUS ByteAPI AllocPhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size, IN BT_MEMORY_PAGE_FLAGS flags){
    UINTN allocPageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE; // Amount of pages to allocate
    UINTN allocated = 0; // Amount of allocated pages
    UINT32 firstPageIndex = 0; // Index of the first page to allocate
    UINTN i = closestPageIndex; // Index of closest free page
    
    if (allocPageCount > pageCount) return BT_NOT_ENOUGH_MEMORY; // If amount of pages to allocate is higher than overall page count
    
    // SAFETY CHECK
    UINTN sfci = i; // Safety check index
    UINTN sfcallocated = 0;  // Safety check allocated pages
    while (sfci < pageCount){ 
        // If current page is allocated continue to another page and reset sfc
        if (PAGE_CHECK(sfci) == PAGE_ALLOCATED){ 
            i++;
            sfci = i;
            sfcallocated = 0;
            continue;
        }  
        sfcallocated++;
        
        // If there is enough free pages in line
        if (sfcallocated == allocPageCount){
            break;
        }
        sfci++;

        // Return error if sfci failed to found linear page sector
        if (sfci == pageCount){
            return BT_NOT_ENOUGH_MEMORY;
        }
    }
    
    // Allocate in line from index 
    while (allocated < allocPageCount){
        PAGE_ALLOC(i);
        flagMap[i] = flags;
        
        if (allocated == 0){
            firstPageIndex = i;
        }
        i++;
        allocated++;
    }

    // Retrieve and assign new buffer address
    PHYSICAL_ADDRESS adr = 0;
    PhysicalIndexToPage(firstPageIndex, &adr);
    
    *buffer = (VOID*)adr;
    *size = allocated * PAGE_SIZE;
    
    // Find new closest page
    if (firstPageIndex == closestPageIndex){
        PhysicalGetClosest(closestPageIndex, &closestPageIndex, &closestPageAddress);
    }
    
    return BT_SUCCESS;
}
BT_STATUS ByteAPI FreePhysicalPages(IN OUT VOID **buffer, IN OUT UINTN *size){
    UINTN freePageCount = (*size + PAGE_SIZE - 1) / PAGE_SIZE; // Amount of pages to free
    UINTN freed = 0; // Amount of freed pages

    PHYSICAL_ADDRESS pageAddress = PAGE_PAD_ADDRESS(*buffer); // Address of buffer`s page
    UINT32 pageIndex = 0; // Initial page index 
    PhysicalPageToIndex(pageAddress, &pageIndex);
    UINTN i = pageIndex;  // Current page index

    if (freePageCount > pageCount) return BT_NOT_ENOUGH_MEMORY; // If amount of pages to free is bigger than overall page count

    while (i < pageCount){
        freed++;

        // If page is allocated process it
        if (PAGE_CHECK(i) == PAGE_ALLOCATED){   
            PAGE_DEALLOC(i);
                        
            // If already freed required pages
            if (freed == freePageCount){
                *size = freed * PAGE_SIZE;
                // Clear out all freed pages from start address
                ForceSetPhysicalMemory((VOID*)pageAddress, 0, *size);
                
                // If closest free index is bigger than initial page index replace it 
                if (closestPageIndex > pageIndex){
                    closestPageIndex = pageIndex;
                    closestPageAddress = pageAddress;
                }

                *buffer = NULL;
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
    MEMORY_PAGE_POOL_HEADER **pPool = NULL; // Currently selected pool header
    
    if (*size >= PAGE_SIZE - sizeof(MEMORY_PAGE_POOL_HEADER)){
        return AllocPhysicalPages(buffer, size, flags);
    }

    UINT32 blockSize = POOL_BLOCK_SIZE(*size);
    UINT32 blockCount = POOL_BLOCK_COUNT(blockSize);

    // Get the pool size that pPool should point to
    switch (blockSize)
    {
        case POOL_SMALL_BLOCK_SIZE:
            pPool = &smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = &mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = &bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = &hugePool; break;
        default:
            pPool = &customPool; break;
    }

    UINT32 pageIndex = 0;
    
    ALLOC:
    PhysicalPageToIndex(PAGE_PAD_ADDRESS((PHYSICAL_ADDRESS)*pPool), &pageIndex);
    
    while (*pPool != NULL){
        if (flagMap[pageIndex] != flags){
            pPool = &(*pPool)->next;
            pageIndex++;
            continue;
        }
        if (*size > (*pPool)->blockSize){
            pPool = &(*pPool)->next;
            pageIndex++;
            continue;
        }

        UINT32 blockIndex = 0; // Index of current block
        UINT32 blockRva = sizeof(MEMORY_PAGE_POOL_HEADER); // Current RVA from pool header
         
        while (((*pPool)->used < blockCount) && blockIndex < blockCount){
            // If current block is allocated continue
            if (POOL_BLOCK_CHECK((*pPool)->poolMap, blockIndex) == BLOCK_ALLOCATED){
                blockIndex++;
                blockRva += (*pPool)->blockSize;
                continue;
            }
            
            // Allocate to current block and set buffer to pool and block RVA
            POOL_BLOCK_ALLOC((*pPool)->poolMap, blockIndex);
            *buffer = (VOID*)((PHYSICAL_ADDRESS)*pPool + blockRva);
            *size = (*pPool)->blockSize;

            (*pPool)->used++;

            return BT_SUCCESS;
        }
        // Move to the next pool using current pointer
        pPool = &(*pPool)->next;
        pageIndex++;
    }

    MEMORY_PAGE_POOL_HEADER *newPool = NULL;

    // Allocate new page for the pool
    UINTN poolSize = PAGE_SIZE;
    BT_STATUS status = AllocPhysicalPages((VOID**)&newPool, &poolSize, flags);
    if (BT_ERROR(status)){
        return status;
    }
    
    // Assign new pool`s values
    newPool->used = 0;
    newPool->blockSize = blockSize;
    (*pPool)->next = newPool;
    
    *pPool = newPool;
    
    // Go back to allocation loop
    goto ALLOC;
}
BT_STATUS ByteAPI FreePhysicalPool(IN OUT VOID **buffer, IN OUT UINTN *size){
    MEMORY_PAGE_POOL_HEADER **prev = NULL; // Previously selected pool header
    MEMORY_PAGE_POOL_HEADER **pPool = NULL; // Currently selected pool header

    if (*size >= PAGE_SIZE - sizeof(MEMORY_PAGE_POOL_HEADER)){
        return FreePhysicalPages(buffer, size);
    }

    UINT32 blockSize = POOL_BLOCK_SIZE(*size);
    UINT32 blockCount = POOL_BLOCK_COUNT(blockSize);
    
    // Get the pool size that pPool should point to
    switch (blockSize)
    {
        case POOL_SMALL_BLOCK_SIZE:
            pPool = &smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = &mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = &bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = &hugePool; break;
        default:
            pPool = &customPool; break;
    }
    
    // Get the pool that has the buffer`s address in its range
    while (*pPool != NULL && ((PHYSICAL_ADDRESS)*pPool + PAGE_SIZE) < (PHYSICAL_ADDRESS)*buffer){
        *prev = *pPool;
        pPool = &(*pPool)->next;
    }
    
    // If buffer`s pool was not found
    if (*pPool == NULL){
        return BT_INVALID_BUFFER;
    }
    
    UINT32 blockIndex = 0; // Current block index
    UINT32 blockRva = sizeof(MEMORY_PAGE_POOL_HEADER); // Current block RVA from it`s header

    while (blockIndex < blockCount){
        // If current block address is equal to buffer
        if ((PHYSICAL_ADDRESS)*buffer == (PHYSICAL_ADDRESS)*pPool + blockRva){
            POOL_BLOCK_DEALLOC((*pPool)->poolMap, blockIndex);
            *size = (*pPool)->blockSize;
            (*pPool)->used--;
            
            // If pool has no blocks after removing the block
            if ((*pPool)->used == 0){
                MEMORY_PAGE_POOL_HEADER *toFree = *pPool;
                // If there is a previous pool assign it`s next to current next
                if (prev != NULL){
                    (*prev)->next = (*pPool)->next; 
                }
                else{
                    *pPool = (*pPool)->next;
                }
                
                // Free current pool`s page
                UINTN s = PAGE_SIZE;
                BT_STATUS status = FreePhysicalPages((VOID**)&toFree, &s);
                if (BT_ERROR(status)){
                    return status;
                }
                
                *buffer = NULL;
                return BT_SUCCESS;
            }
            else{
                // Clear out removed block
                ForceSetPhysicalMemory(*buffer, 0, (*pPool)->blockSize);
            }
            
            *buffer = NULL;
            return BT_SUCCESS;
        }
        
        // Move to another block
        blockIndex++;
        blockRva += (*pPool)->blockSize;
    }

    return BT_INVALID_BUFFER;
}
MEMORY_PAGE_POOL_HEADER ByteAPI GetPhysicalPool(IN UINT32 index, IN UINT32 poolSize){
    MEMORY_PAGE_POOL_HEADER **pPool = NULL;
    UINT32 blockSize = POOL_BLOCK_SIZE(poolSize);

    switch (blockSize)
    {
        case POOL_SMALL_BLOCK_SIZE:
            pPool = &smallPool; break;
        case POOL_MEDIUM_BLOCK_SIZE:
            pPool = &mediumPool; break;
        case POOL_BIG_BLOCK_SIZE:
            pPool = &bigPool; break;
        case POOL_HUGE_BLOCK_SIZE:
            pPool = &hugePool; break;
        default:
            if (blockSize > PAGE_SIZE){
                return (MEMORY_PAGE_POOL_HEADER){0};
            }
            else{
                pPool = &customPool; break;
            } 
    }   

    UINT32 i = 0;
    while (i < index){
        *pPool = (*pPool)->next;
        i++;

        if (*pPool == NULL){
            return (MEMORY_PAGE_POOL_HEADER){0};
        }
    }

    return **pPool;
}

BT_STATUS ByteAPI ForceSetPhysicalMemory(IN VOID *buffer, IN BYTE value, IN UINTN size){
    if (buffer == NULL){
        return BT_INVALID_ARGUMENT;
    }

    BYTE *ptr = (BYTE*)buffer;

    for (UINTN i = 0; i < size; i++) {
        ptr[i] = value;
    }
    return BT_SUCCESS;
}
BT_STATUS ByteAPI SetPhysicalMemory(IN VOID *buffer, IN BYTE value, IN UINTN size){
    if (buffer == NULL){
        return BT_INVALID_ARGUMENT;
    }

    BYTE *ptr = (BYTE*)buffer;

    UINT32 index = 0;
    PhysicalPageToIndex((PHYSICAL_ADDRESS)ptr, &index);

    // PERMISSION CHECK
    UINTN pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (UINTN i = index; i < pageCount; i++){
        if ((flagMap[i] & BT_MEMORY_WRITE) == FALSE){
            return BT_MEMORY_NOT_WRITABLE;
        }
    }
    
    for (UINTN i = 0; i < size; i++){
        ptr[i] = value;
    }
    return BT_SUCCESS;
}
BT_STATUS ByteAPI GetPhysicalFlags(IN VOID *buffer, OUT BT_MEMORY_PAGE_FLAGS *flags){
    if (buffer == NULL){
        return BT_INVALID_ARGUMENT;
    }
    if (flags == NULL){
        return BT_INVALID_BUFFER;
    }
    
    UINT32 pageIndex = 0;
    BT_STATUS status = PhysicalPageToIndex(PAGE_PAD_ADDRESS((PHYSICAL_ADDRESS)buffer), &pageIndex);
    if (BT_ERROR(status)) return status;
    
    *flags = flagMap[pageIndex];

    return BT_SUCCESS;
}

BT_STATUS ByteAPI CopyPhysicalMemory(IN VOID *from, IN UINTN size, IN OUT VOID *to){
    if (from == NULL) return BT_INVALID_ARGUMENT;
    if (to == NULL) return BT_INVALID_BUFFER;

    BYTE *fptr = (BYTE*)from;
    BYTE *tptr = (BYTE*)to;

    BT_STATUS status = 0;

    status = PhysicalCheckPermission(from, BT_MEMORY_READ, size);
    if (status == BT_INVALID_MEMORY) return BT_MEMORY_NOT_READABLE;

    status = PhysicalCheckPermission(to, BT_MEMORY_WRITE, size);
    if (status == BT_INVALID_MEMORY) return BT_MEMORY_NOT_WRITABLE;
    
    for (UINTN i = 0; i < size; i++){
        tptr[i] = fptr[i];
    }
    return BT_SUCCESS;
}
BT_STATUS ByteAPI ComparePhysicalMemory(IN VOID *from, IN UINTN size, IN VOID *to){
    if (from == NULL) return BT_INVALID_ARGUMENT;
    if (to == NULL) return BT_INVALID_ARGUMENT;

    BYTE *fptr = (BYTE*)from;
    BYTE *tptr = (BYTE*)to;
    
    BT_STATUS status = 0;

    status = PhysicalCheckPermission(from, BT_MEMORY_READ, size);
    if (status == BT_INVALID_MEMORY) return BT_MEMORY_NOT_READABLE;
    
    UINT32 fptrZc = 0;
    UINT32 tptrZc = 0;
    UINTN i = 0;

    while (i < size && fptr[i++] == 0) fptrZc++;
    
    i = 0;
    while (i < size && tptr[i++] == 0) tptrZc++;
    
    // Compare leading zero`s count
    // The lower zero count is the higher the value is
    // Ex: 0x0100 (1) > 0x0006 (3)
    if (fptrZc < tptrZc) return BT_COMPARE_SMALLER;
    if (fptrZc > tptrZc) return BT_COMPARE_BIGGER;
        
    for (UINTN i = 0; i < size; i++){
        if (fptr[i] < tptr[i]) return BT_COMPARE_SMALLER;
        if (fptr[i] > tptr[i]) return BT_COMPARE_BIGGER;
    }

    return BT_COMPARE_EQUAL;
}

// ==================================== |
//           PHYSICAL HELPERS           |
// ==================================== |

BT_STATUS ByteAPI PhysicalIndexToPage(IN UINT32 pageIndex, OUT PHYSICAL_ADDRESS *address){
    if (address == NULL){
        return BT_INVALID_BUFFER;
    }
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
    if (index == NULL){
        return BT_INVALID_BUFFER;
    }
    if (*index > pageCount){
        return BT_INVALID_PAGE_INDEX;
    }

    *index = PAGE_PAD_ADDRESS(pageAddress) / PAGE_SIZE;  
    return BT_SUCCESS;
}
BT_STATUS ByteAPI PhysicalGetClosest(IN UINT32 fromIndex, OUT UINT32 *index, OUT PHYSICAL_ADDRESS *address){
    if (index == NULL){
        return BT_INVALID_BUFFER;
    }
    if (address == NULL){
        return BT_INVALID_BUFFER;
    }    

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
BT_STATUS ByteAPI PhysicalCheckPermission(IN VOID *buffer, IN BT_MEMORY_PAGE_FLAGS permission, IN UINTN size){
    if (buffer == NULL) return BT_INVALID_ARGUMENT;

    UINT32 index = 0;
    BT_STATUS status = PhysicalPageToIndex((PHYSICAL_ADDRESS)buffer, &index);
    if (BT_ERROR(status)) return status;
    
    UINTN pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (UINTN i = index; i < pageCount; i++){
        if ((flagMap[i] & permission) == FALSE) return BT_INVALID_MEMORY;
    }

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

