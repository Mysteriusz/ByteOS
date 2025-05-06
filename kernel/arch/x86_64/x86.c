#include "x86.h"

BT_STATUS X86_FlushLine(IN PHYSICAL_ADDRESS address) {
    CLFLUSH((VOID*)address);

    return BT_SUCCESS;
}
BT_STATUS X86_FlushPage(IN PHYSICAL_ADDRESS address) {
    PHYSICAL_ADDRESS pageAddress = PAGE_PAD_ADDRESS(address);
    UINT32 clCount = PAGE_SIZE / X86_CACHE_LINE_SIZE;

    for (UINT32 i = 0; i < clCount; i++) {
        CLFLUSH((VOID*)pageAddress);
        pageAddress += X86_CACHE_LINE_SIZE;
    }

    return BT_SUCCESS;
}
