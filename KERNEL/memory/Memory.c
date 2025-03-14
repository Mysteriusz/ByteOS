#include "Memory.h"

#define KERNEL_PAGE_SIZE 1024
#define CONV_PAGE_SIZE 4096

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap){
    for (UINTN i = 0; i < memMap->entryCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory || EfiBootServicesCode || EfiBootServicesData
        if (desc.type == 7 || desc.type == 3 || desc.type == 4){
            
        }
    }

    return BT_SUCCESS;
}

BT_STATUS ByteAPI Alloc(IN OUT VOID *buffer, IN OUT UINTN size){
    return BT_SUCCESS;
}
BT_STATUS ByteAPI DeAlloc(IN OUT VOID *buffer, IN OUT UINTN size){
    return BT_SUCCESS;
}