#ifndef EFI_TYPES_H
#define EFI_TYPES_H

#include "../ByteOS.h"

#define EFIAPI

#define EFI_BLACK 0x00
#define EFI_BLUE 0x01
#define EFI_GREEN 0x02
#define EFI_CYAN 0x03
#define EFI_RED 0x04
#define EFI_MAGENTA 0x05
#define EFI_BROWN 0x06
#define EFI_LIGHTGRAY 0x07
#define EFI_BRIGHT 0x08
#define EFI_LIGHTBLUE 0x09
#define EFI_LIGHTGREEN 0x0A
#define EFI_LIGHTCYAN 0x0B
#define EFI_LIGHTRED 0x0C
#define EFI_LIGHTMAGENTA 0x0D
#define EFI_YELLOW 0x0E
#define EFI_WHITE 0x0F

#define EFI_BACKGROUND_BLACK 0x00
#define EFI_BACKGROUND_BLUE 0x10
#define EFI_BACKGROUND_GREEN 0x20
#define EFI_BACKGROUND_CYAN 0x30
#define EFI_BACKGROUND_RED 0x40
#define EFI_BACKGROUND_MAGENTA 0x50
#define EFI_BACKGROUND_BROWN 0x60
#define EFI_BACKGROUND_LIGHTGRAY 0x70

#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))

#define EFI_SUCCESS 0

typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;

typedef UINTN EFI_STATUS;
#define EFI_ERROR (status) (((UINTN)(status)) > 0)

typedef UINTN EFI_PHYSICAL_ADDRESS;

typedef struct EFI_INPUT_KEY EFI_INPUT_KEY;
typedef struct SIMPLE_TEXT_OUTPUT_MODE SIMPLE_TEXT_OUTPUT_MODE;
typedef struct EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;
typedef struct EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;

typedef struct EFI_TABLE_HEADER EFI_TABLE_HEADER;
typedef struct EFI_BOOT_SERVICES  EFI_BOOT_SERVICES;
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef struct EFI_GUID {
    UINT32 d1;
    UINT16 d2;
    UINT16 d3;
    UINT8  d4[8];
} EFI_GUID;

typedef VOID (EFIAPI *EFI_RESET_SYSTEM)(
    IN EFI_RESET_TYPE resetType,
    IN EFI_STATUS resetStatus,
    IN UINTN dataSize,
    IN VOID *resetData OPTIONAL
);

// ==================================== |
//    EFI_SIMPLE_TEXT_INPUT_PROTOCOL    |
// ==================================== |

#pragma region EFI_SIMPLE_TEXT_INPUT_PROTOCOL

typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET)(
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *protocol,
    IN BOOLEAN extendedVerification
);

typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY)(
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *protocol,
    OUT EFI_INPUT_KEY *key
);

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET reset;
    EFI_INPUT_READ_KEY readKeyStroke;
    EFI_EVENT waitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

#pragma endregion EFI_SIMPLE_TEXT_INPUT_PROTOCOL

// ==================================== |
//    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   |
// ==================================== |

#pragma region EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL

typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN BOOLEAN extendedVerification
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN CHAR16 *string
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN CHAR16 *string
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN UINTN modeNumber,
    OUT UINTN *columns,
    OUT UINTN *rows
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_MODE)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN UINTN modeNumber
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN UINTN attribute
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN UINTN column,
    IN UINTN row
);

typedef EFI_STATUS (EFIAPI *EFI_TEXT_ENABLE_CURSOR)(
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol,
    IN BOOLEAN visible
);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET reset;
    EFI_TEXT_STRING outputString;
    EFI_TEXT_TEST_STRING testString;
    EFI_TEXT_QUERY_MODE queryMode;
    EFI_TEXT_SET_MODE setMode;
    EFI_TEXT_SET_ATTRIBUTE setAttribute;
    EFI_TEXT_CLEAR_SCREEN clearScreen;
    EFI_TEXT_SET_CURSOR_POSITION setCursorPosition;
    EFI_TEXT_ENABLE_CURSOR enableCursor;
    SIMPLE_TEXT_OUTPUT_MODE *mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

#pragma endregion EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL

// ==================================== |
//    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   |
// ==================================== |

#pragma region EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID \
    {0x0964e5b22,0x6459,0x11d2,\
    {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

typedef EFI_STATUS (EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
    IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *protocol,
    OUT EFI_FILE_PROTOCOL **root
);

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL{
    UINT64 revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME openVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

#pragma endregion EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

// ==================================== |
//           EFI_FILE_PROTOCOL          |
// ==================================== |

#pragma region EFI_FILE_PROTOCOL

#define EFI_FILE_PROTOCOL_REVISION 0x00010000
#define EFI_FILE_PROTOCOL_REVISION2 0x00020000

// =============== ATTRIBUTES ===============  

#define EFI_FILE_READ_ONLY       0x0000000000000001
#define EFI_FILE_HIDDEN          0x0000000000000002
#define EFI_FILE_SYSTEM          0x0000000000000004
#define EFI_FILE_RESERVED        0x0000000000000008
#define EFI_FILE_DIRECTORY       0x0000000000000010
#define EFI_FILE_ARCHIVE         0x0000000000000020
#define EFI_FILE_VALID_ATTR      0x0000000000000037

// =============== OPEN MODES ===============  

#define EFI_FILE_MODE_READ       0x0000000000000001
#define EFI_FILE_MODE_WRITE      0x0000000000000002
#define EFI_FILE_MODE_CREATE     0x8000000000000000

typedef EFI_STATUS (EFIAPI *EFI_FILE_OPEN)(
    IN EFI_FILE_PROTOCOL *protocol,
    OUT EFI_FILE_PROTOCOL **newProtocol,
    IN CHAR16 *fileName,
    IN UINT64 *openMode,
    IN UINT64 *attributes
);

typedef EFI_STATUS (EFIAPI *EFI_FILE_READ)(
    IN EFI_FILE_PROTOCOL *protocol,
    IN OUT UINTN *bufferSize,
    OUT VOID *buffer
);

typedef struct EFI_FILE_PROTOCOL{
    UINT64 revision;
    EFI_FILE_OPEN open;
    VOID *close;
    VOID *delete;
    EFI_FILE_READ read;
    VOID *write;
    VOID *getPosition;
    VOID *setPosition;
    VOID *getInfo;
    VOID *setInfo;
    VOID *flush;
    VOID *openEx;
    VOID *readEx;
    VOID *writeEx;
    VOID *flushEx;
} EFI_FILE_PROTOCOL;

#pragma endregion EFI_FILE_PROTOCOL

// ==================================== |
//               EFI_BASE               |
// ==================================== |

#pragma region EFI_BASE

typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType,
} EFI_MEMORY_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(
    IN EFI_MEMORY_TYPE poolType,
    IN UINTN size,
    OUT VOID **buffer
);typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL)(
    IN VOID *buffer
);

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL)(
    IN EFI_GUID *protocol,
    IN VOID *registration, OPTIONAL
    OUT VOID **interface
);

typedef EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL)(
    IN EFI_HANDLE handle,
    IN EFI_GUID *protocol,
    OUT VOID **interface
);

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER)(
    IN EFI_LOCATE_SEARCH_TYPE searchType,
    IN EFI_GUID *protocol, OPTIONAL
    IN VOID *searchKey, OPTIONAL
    OUT UINTN *noHandles,
    OUT EFI_HANDLE **buffer
);

typedef struct EFI_TABLE_HEADER {
    UINT64 signature;
    UINT32 revision;
    UINT32 headerSize;
    UINT32 crc32;
    UINT32 reserved;
} EFI_TABLE_HEADER;

typedef struct EFI_SYSTEM_TABLE {
    EFI_TABLE_HEADER header;
    CHAR16 *firmwareVendor;
    UINT32 firmwareRevision;
    EFI_HANDLE consoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *conIn;
    EFI_HANDLE consoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *conOut;
    EFI_HANDLE standardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdErr;
    EFI_RUNTIME_SERVICES *runtimeServices;
    EFI_BOOT_SERVICES *bootServices;
    UINTN numberOfEntries;
    VOID* efiConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct EFI_RUNTIME_SERVICES {
    EFI_TABLE_HEADER header;
    VOID* getTime;
    VOID* setTime;
    VOID* getWakeupTime;
    VOID* setWakeupTime;
    VOID* setVirtualAddressMap;
    VOID* convertPointer;
    VOID* getVariable;
    VOID* getNextVariableName;
    VOID* setVariable;
    VOID* getNextHighMonotonicCount;
    EFI_RESET_SYSTEM resetSystem;
    VOID* updateCapsule;
    VOID* queryCapsuleCapabilities;
    VOID* queryVariableInfo;
} EFI_RUNTIME_SERVICES;

typedef struct EFI_BOOT_SERVICES {
    EFI_TABLE_HEADER header;
    VOID *raiseTpl;
    VOID *restoreTpl;
    VOID *allocatePages;
    VOID *freePages;
    VOID *getMemoryMap;
    EFI_ALLOCATE_POOL allocatePool;
    EFI_FREE_POOL freePool;
    VOID *createEvent;
    VOID *setTimer;
    VOID *waitForEvent;
    VOID *signalEvent;
    VOID *closeEvent;
    VOID *checkEvent;
    VOID *installProtocolInterface;
    VOID *reinstallProtocolInterface;
    VOID *uninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL handleProtocol;
    VOID *reserved;
    VOID *registerProtocolNotify;
    VOID *locateHandle;
    VOID *locateDevicePath;
    VOID *installConfigurationTable;
    VOID *loadImage;
    VOID *startImage;
    VOID *exit;
    VOID *unloadImage;
    VOID *exitBootServices;
    VOID *getNextMonotonicCount;
    VOID *stall;
    VOID *setWatchdogTimer;
    VOID *connectController;
    VOID *disconnectController;
    VOID *openProtocol;
    VOID *closeProtocol;
    VOID *openProtocolInformation;
    VOID *protocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER locateHandleBuffer;
    EFI_LOCATE_PROTOCOL locateProtocol;
    VOID *installMultipleProtocolInterfaces;
    VOID *uninstallMultipleProtocolInterfaces;
    VOID *calculateCrc32;
    VOID *copyMem;
    VOID *setMem;
    VOID *createEventEx;
} EFI_BOOT_SERVICES;

#pragma endregion EFI_BASE

EFI_STATUS EFI_Print(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* con, CHAR16* message){
    EFI_STATUS status;
    status = con->outputString(con, message);
    return status;
}
EFI_STATUS EFI_Alloc(EFI_ALLOCATE_POOL allocPool, EFI_MEMORY_TYPE memoryType, UINTN bufferSize, VOID** buffer){
    EFI_STATUS status;
    status = allocPool(memoryType, bufferSize, buffer);
    return status;
}
EFI_STATUS EFI_DeAlloc(EFI_FREE_POOL freePool, VOID* buffer){
    EFI_STATUS status;
    status = freePool(buffer);
    return status;
}

CHAR16* EFI_GetStatus(EFI_STATUS status){
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

#endif