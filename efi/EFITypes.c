#include "EFITypes.h"

// ==================================== |
//              EFI_GLOBALS             |
// ==================================== |

EFI_HANDLE *imageHandle;
EFI_SYSTEM_TABLE *systemTable;

// ==================================== |
//              EFI_METHODS             |
// ==================================== |

EFI_STATUS EFI_Print(IN CHAR16* message){
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
    return systemTable->bootServices->locateHandleBuffer(ByProtocol, &guid, NULL, count, (EFI_HANDLE**)&buffer);
}
CHAR16* EFI_GetStatus(IN EFI_STATUS status){
    switch (status) {
        case 0:
            return L"EFI_SUCCESS";
        case 1:
            return L"EFI_LOAD_ERROR";
        case 2:
            return L"EFI_INVALID_PARAMETER";
        case 3:
            return L"EFI_UNSUPPORTED";
        case 4:
            return L"EFI_BAD_BUFFER_SIZE";
        case 5:
            return L"EFI_BUFFER_TOO_SMALL";
        case 6:
            return L"EFI_NOT_READY";
        case 7:
            return L"EFI_DEVICE_ERROR";
        case 8:
            return L"EFI_WRITE_PROTECTED";
        case 9:
            return L"EFI_OUT_OF_RESOURCES";
        case 10:
            return L"EFI_VOLUME_CORRUPTED";
        case 11:
            return L"EFI_VOLUME_FULL";
        case 12:
            return L"EFI_NO_MEDIA";
        case 13:
            return L"EFI_MEDIA_CHANGED";
        case 14:
            return L"EFI_NOT_FOUND";
        case 15:
            return L"EFI_ACCESS_DENIED";
        case 16:
            return L"EFI_NO_RESPONSE";
        case 17:
            return L"EFI_NO_MAPPING";
        case 18:
            return L"EFI_TIMEOUT";
        case 19:
            return L"EFI_NOT_STARTED";
        case 20:
            return L"EFI_ALREADY_STARTED";
        case 21:
            return L"EFI_ABORTED";
        case 22:
            return L"EFI_ICMP_ERROR";
        case 23:
            return L"EFI_TFTP_ERROR";
        case 24:
            return L"EFI_PROTOCOL_ERROR";
        case 25:
            return L"EFI_INCOMPATIBLE_VERSION";
        case 26:
            return L"EFI_SECURITY_VIOLATION";
        case 27:
            return L"EFI_CRC_ERROR";
        case 28:
            return L"EFI_END_OF_MEDIA";
        case 29:
            return L"EFI_END_OF_FILE";
        case 30:
            return L"EFI_INVALID_LANGUAGE";
        case 31:
            return L"EFI_COMPROMISED_DATA";
        case 32:
            return L"EFI_IP_ADDRESS_CONFLICT";
        case 33:
            return L"EFI_PROTOCOL_UNAVAILABLE";
        case 34:
            return L"EFI_POLICY_VIOLATION";
        case 35:
            return L"EFI_INVALID_IMAGE_HASH";
        case 36:
            return L"EFI_PLATFORM_ERROR";
        case 37:
            return L"EFI_TIMEOUT_EXPIRED";
        case 38:
            return L"EFI_UNSUPPORTED_VERSION";
        case 39:
            return L"EFI_INTERNAL_ERROR";
        case 40:
            return L"EFI_INVALID_STATE";
        case 41:
            return L"EFI_BUFFER_OVERFLOW";
        case 42:
            return L"EFI_INVALID_SIGNATURE";
        case 43:
            return L"EFI_PERSISTED_DATA_NOT_FOUND";
        case 44:
            return L"EFI_FLASH_UPDATE_ERROR";
        case 45:
            return L"EFI_FLASH_WRITE_ERROR";
        case 46:
            return L"EFI_FIRMWARE_UPDATE_REQUIRED";
        case 47:
            return L"EFI_MEMORY_ALLOCATION_ERROR";
        case 48:
            return L"EFI_LOW_MEMORY";
        case 49:
            return L"EFI_FLASH_BACKUP_ERROR";
        case 50:
            return L"EFI_CONFIGURATION_ERROR";
        case 51:
            return L"EFI_INTERNAL_STORAGE_ERROR";
        case 52:
            return L"EFI_SHUTDOWN_IN_PROGRESS";
        case 53:
            return L"EFI_RESET_REQUIRED";
        default:
            return L"Unknown Error Code";
    }
}