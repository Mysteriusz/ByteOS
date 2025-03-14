#include "Memory.h"

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap){
    for (UINTN i = 0; i < memMap->descriptorCount; i++){
        KERNEL_MEMORY_DESCRIPTOR desc = memMap->entries[i];
        
        // EfiConventionalMemory
        if (desc.type == 7){
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