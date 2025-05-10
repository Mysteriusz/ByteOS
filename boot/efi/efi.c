#include "efi.h"

// ==================================== |
//              EFI_GLOBALS             |
// ==================================== |

EFI_HANDLE imageHandle;
EFI_SYSTEM_TABLE *systemTable;

// ==================================== |
//              EFI_METHODS             |
// ==================================== |

EFI_STATUS EFI_Print(IN UINT16* message){
    return systemTable->conOut->outputString(systemTable->conOut, message);
}
EFI_STATUS EFI_AllocPool(IN EFI_MEMORY_TYPE memoryType, IN UINTN bufferSize, OUT VOID** buffer){
    return systemTable->bootServices->allocatePool(memoryType, bufferSize, buffer);
}
EFI_STATUS EFI_AllocPages(IN EFI_ALLOCATE_TYPE allocType, IN EFI_MEMORY_TYPE memType, IN UINTN pages, OUT EFI_PHYSICAL_ADDRESS* buffer){
    return systemTable->bootServices->allocatePages(allocType, memType, pages, buffer);
}
EFI_STATUS EFI_DeAllocPool(IN VOID* buffer){
    return systemTable->bootServices->freePool(buffer);
}
EFI_STATUS EFI_DeAllocPages(IN UINTN pages, IN EFI_PHYSICAL_ADDRESS buffer){
    return systemTable->bootServices->freePages(buffer, pages);
}
EFI_STATUS EFI_FindProtocol(IN EFI_GUID guid, OUT UINTN* count, OUT VOID** buffer){
    return systemTable->bootServices->locateHandleBuffer(ByProtocol, &guid, NULL, count, (EFI_HANDLE**)buffer);
}
EFI_STATUS EFI_HandleProtocol(IN EFI_HANDLE* handle, IN EFI_GUID guid, OUT VOID** buffer) {
    return systemTable->bootServices->handleProtocol(*handle, &guid, (VOID**)buffer);
}
