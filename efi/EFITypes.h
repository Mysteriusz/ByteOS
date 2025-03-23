#ifndef EFI_TYPES_H
#define EFI_TYPES_H

#include "../kernel/ByteOS.h"

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

#define EFI_KEY_ENTER '\r'
#define EFI_KEY_BACKSPACE '\b'
#define EFI_KEY_ESCAPE '\e'
#define EFI_KEY_TABULATOR '\t'
#define EFI_KEY_SPACE ' '
#define EFI_KEY_DELETE '\x7f'

#define EFI_PAGE_SHIFT 12
#define EFI_PAGE_MASK 0xFFF
#define EFI_PAGE_SIZE 0x00001000

#define EFI_TEXT_ATTR(Foreground, Background) ((Foreground) | ((Background) << 4))
#define EFI_ERROR(status) (((UINTN)(status)) > 0)
#define EFI_SIZE_TO_PAGES(Size) (((Size) >> EFI_PAGE_SHIFT) + (((Size) & EFI_PAGE_MASK) ? 1 : 0))
#define EFI_SUCCESS 0

typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;

typedef UINTN EFI_STATUS;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

typedef struct EFI_INPUT_KEY EFI_INPUT_KEY;
typedef struct SIMPLE_TEXT_OUTPUT_MODE SIMPLE_TEXT_OUTPUT_MODE;
typedef struct EFI_RUNTIME_SERVICES EFI_RUNTIME_SERVICES;

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef struct EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;

typedef struct EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL;
typedef struct EFI_PCI_IO_PROTOCOL EFI_PCI_IO_PROTOCOL;
typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;
typedef struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;
typedef struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef struct EFI_MEMORY_DESCRIPTOR EFI_MEMORY_DESCRIPTOR;

typedef struct EFI_TABLE_HEADER EFI_TABLE_HEADER;
typedef struct EFI_BOOT_SERVICES  EFI_BOOT_SERVICES;
typedef struct EFI_SYSTEM_TABLE EFI_SYSTEM_TABLE;

typedef enum EFI_RESET_TYPE {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

typedef enum EFI_LOCATE_SEARCH_TYPE {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef enum EFI_ALLOCATE_TYPE {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
 } EFI_ALLOCATE_TYPE;

typedef enum EFI_MEMORY_TYPE {
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

typedef struct EFI_GUID {
    UINT32 d1;
    UINT16 d2;
    UINT16 d3;
    UINT8  d4[8];
} EFI_GUID;

typedef struct EFI_PIXEL_BITMASK {
    UINT32 redMask;
    UINT32 greenMask;
    UINT32 blueMask;
    UINT32 reservedMask;
} EFI_PIXEL_BITMASK;

typedef struct EFI_TIME {
    UINT16 year;              // 1900 - 9999
    UINT8 month;             // 1 - 12
    UINT8 day;               // 1 - 31
    UINT8 hour;              // 0 - 23
    UINT8 minute;            // 0 - 59
    UINT8 second;            // 0 - 59
    UINT8 pad1;
    UINT32 nanosecond;        // 0 - 999,999,999
    INT16 timeZone;          // —1440 to 1440 or 2047
    UINT8 daylight;
    UINT8 pad2;
} EFI_TIME;

typedef struct EFI_INPUT_KEY {
    UINT16 scanCode;
    CHAR16 unicodeChar;
} EFI_INPUT_KEY;

typedef VOID (EFIAPI *EFI_RESET_SYSTEM)(
    IN EFI_RESET_TYPE resetType,
    IN EFI_STATUS resetStatus,
    IN UINTN dataSize,
    IN VOID *resetData OPTIONAL
);

// ==================================== |
//              EFI_GLOBALS             |
// ==================================== |

extern EFI_HANDLE *imageHandle;
extern EFI_SYSTEM_TABLE *systemTable;

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

typedef struct SIMPLE_TEXT_OUTPUT_MODE {
    INT32 maxMode;
    INT32 mode;
    INT32 attribute;
    INT32 cursorColumn;
    INT32 cursorRow;
    BOOLEAN cursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

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

#define EFI_FILE_INFO_GUID \
    { 0x09576e92, 0x6d3f, 0x11d2, \
    { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

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

typedef EFI_STATUS (EFIAPI *EFI_FILE_GET_INFO) (
    IN EFI_FILE_PROTOCOL *protocol,
    IN EFI_GUID *informationType,
    IN OUT UINTN *bufferSize,
    OUT VOID *buffer
);

typedef EFI_STATUS (EFIAPI *EFI_FILE_CLOSE) (
    IN EFI_FILE_PROTOCOL *protocol
);

typedef struct EFI_FILE_INFO {
    UINT64 size;
    UINT64 fileSize;
    UINT64 physicalSize;
    EFI_TIME createTime;
    EFI_TIME lastAccessTime;
    EFI_TIME modificationTime;
    UINT64 attribute;
    CHAR16 fileName [];
} EFI_FILE_INFO;  

typedef struct EFI_FILE_PROTOCOL{
    UINT64 revision;
    EFI_FILE_OPEN open;
    EFI_FILE_CLOSE close;
    VOID *delete;
    EFI_FILE_READ read;
    VOID *write;
    VOID *getPosition;
    VOID *setPosition;
    EFI_FILE_GET_INFO getInfo;
    VOID *setInfo;
    VOID *flush;
    VOID *openEx;
    VOID *readEx;
    VOID *writeEx;
    VOID *flushEx;
} EFI_FILE_PROTOCOL;

#pragma endregion EFI_FILE_PROTOCOL

// ==================================== |
//    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   |
// ==================================== |

#pragma region EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL

#define EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID \
    {0x2F707EBB,0x4A1A,0x11d4,\
    {0x9A,0x38,0x00,0x90,0x27,0x3F,0xC1,0x4D}}

typedef enum EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH {
    EfiPciWidthUint8,
    EfiPciWidthUint16,
    EfiPciWidthUint32,
    EfiPciWidthUint64,
    EfiPciWidthFifoUint8,
    EfiPciWidthFifoUint16,
    EfiPciWidthFifoUint32,
    EfiPciWidthFifoUint64,
    EfiPciWidthFillUint8,
    EfiPciWidthFillUint16,
    EfiPciWidthFillUint32,
    EfiPciWidthFillUint64,
    EfiPciWidthMaximum
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH;
typedef enum EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION {
    EfiPciOperationBusMasterRead,
    EfiPciOperationBusMasterWrite,
    EfiPciOperationBusMasterCommonBuffer,
    EfiPciOperationBusMasterRead64,
    EfiPciOperationBusMasterWrite64,
    EfiPciOperationBusMasterCommonBuffer64,
    EfiPciOperationMaximum
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION;

typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH width,
    IN UINT64 address,
    IN UINT64 mask,
    IN UINT64 value,
    IN UINT64 delay,
    OUT UINT64 *result
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH width,
    IN UINT64 address,
    IN UINTN count,
    IN OUT VOID *buffer
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_COPY_MEM)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH width,
    IN UINT64 destAddress,
    IN UINT64 srcAddress,
    IN UINTN count,
    IN OUT VOID *buffer
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_MAP)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_OPERATION operation,
    IN VOID *hostAddress,
    IN OUT UINTN *numberOfBytes,
    OUT EFI_PHYSICAL_ADDRESS *deviceAddress,
    OUT VOID **mapping
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_UNMAP)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN VOID *mapping
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ALLOCATE_BUFFER)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN EFI_ALLOCATE_TYPE type,
    IN EFI_MEMORY_TYPE memType,
    IN UINTN pages,
    OUT VOID **hostAddress,
    IN INT64 attributes
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FREE_BUFFER)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN UINTN pages,
    IN VOID *hostAddress
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FLUSH)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GET_ATTRIBUTES)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    OUT UINT64 *supports, OPTIONAL
    OUT UINT64 *attributes OPTIONAL
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_SET_ATTRIBUTES)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    IN UINT64 attributes,
    IN OUT UINT64 *resourceBase, OPTIONAL
    IN OUT UINT64 *resourceLength OPTIONAL
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_CONFIGURATION)(
    IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *protocol,
    OUT VOID **resources
);

typedef struct EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS {
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM read;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_IO_MEM write;
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS;

typedef struct EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL {
    EFI_HANDLE parent;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM pollMem;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_POLL_IO_MEM pollIo;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS mem;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS io;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ACCESS pci;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_COPY_MEM copyMem;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_MAP map;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_UNMAP unmap;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_ALLOCATE_BUFFER allocateBuffer;  
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FREE_BUFFER freeBuffer;  
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_FLUSH flush;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GET_ATTRIBUTES getAttributes;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_SET_ATTRIBUTES setAttributes;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_CONFIGURATION configuration;
    UINT32 segmentNumber;
} EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL;

#pragma endregion EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL

// ==================================== |
//      EFI_GRAPHICS_OUTPUT_PROTCOL     |
// ==================================== |

#pragma region EFI_GRAPHICS_OUTPUT_PROTCOL

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
 {0x9042a9de,0x23dc,0x4a38,\
  {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

typedef enum EFI_GRAPHICS_PIXEL_FORMAT {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef enum EFI_GRAPHICS_OUTPUT_BLT_OPERATION {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *protocol,
    IN UINT32 modeNumber,
    OUT UINTN *sizeOfInfo,
    OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **info
);

typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *protocol,
    IN UINT32 modeNumber
);

typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *protocol,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bltBuffer, OPTIONAL
    IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION bltOperation,
    IN UINTN sourceX,
    IN UINTN sourceY,
    IN UINTN destinationX,
    IN UINTN destinationY,
    IN UINTN width,
    IN UINTN height,
    IN UINTN delta OPTIONAL
);

typedef struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL {
    UINT8 blue;
    UINT8 green;
    UINT8 red;
    UINT8 reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
    UINT32 version;
    UINT32 horizontalResolution;
    UINT32 verticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT pixelFormat;
    EFI_PIXEL_BITMASK pixelInformation;
    UINT32 pixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    UINT32 maxMode;
    UINT32 mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN sizeOfInfo;
    EFI_PHYSICAL_ADDRESS frameBufferBase;
    UINTN frameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE queryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE setMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

#pragma endregion EFI_GRAPHICS_OUTPUT_PROTCOL

// ==================================== |
//          EFI_PCI_IO_PROTOCOL         |
// ==================================== |

#pragma region EFI_PCI_IO_PROTOCOL

#define EFI_PCI_IO_PROTOCOL_GUID \
    {0x4cf5b200,0x68b8,0x4ca5,\
    {0x9e,0xec,0xb2,0x3e,0x3f,0x50,0x02,0x9a}}

typedef enum EFI_PCI_IO_PROTOCOL_WIDTH {
    EfiPciIoWidthUint8,
    EfiPciIoWidthUint16,
    EfiPciIoWidthUint32,
    EfiPciIoWidthUint64,
    EfiPciIoWidthFifoUint8,
    EfiPciIoWidthFifoUint16,
    EfiPciIoWidthFifoUint32,
    EfiPciIoWidthFifoUint64,
    EfiPciIoWidthFillUint8,
    EfiPciIoWidthFillUint16,
    EfiPciIoWidthFillUint32,
    EfiPciIoWidthFillUint64,
    EfiPciIoWidthMaximum
} EFI_PCI_IO_PROTOCOL_WIDTH;

typedef enum EFI_PCI_IO_PROTOCOL_OPERATION {
    EfiPciIoOperationBusMasterRead,
    EfiPciIoOperationBusMasterWrite,
    EfiPciIoOperationBusMasterCommonBuffer,
    EfiPciIoOperationMaximum
} EFI_PCI_IO_PROTOCOL_OPERATION;

typedef enum EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION {
    EfiPciIoAttributeOperationGet,
    EfiPciIoAttributeOperationSet,
    EfiPciIoAttributeOperationEnable,
    EfiPciIoAttributeOperationDisable,
    EfiPciIoAttributeOperationSupported,
    EfiPciIoAttributeOperationMaximum
} EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION;

typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_POLL_IO_MEM) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_WIDTH width,
    IN UINT8 barIndex,
    IN UINT64 offset,
    IN UINT64 mask,
    IN UINT64 value,
    IN UINT64 delay,
    OUT UINT64 *result
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_IO_MEM) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_WIDTH width,
    IN UINT8 barIndex,
    IN UINT64 offset,
    IN UINTN count,
    IN OUT VOID *buffer
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_CONFIG) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_WIDTH width,
    IN UINT32 offset,
    IN UINTN count,
    IN OUT VOID *buffer
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_COPY_MEM) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_WIDTH width,
    IN UINT8 DestBarIndex,
    IN UINT64 destOffset,
    IN UINT8 srcBarIndex,
    IN UINT64 srcOffset,
    IN UINTN count
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_MAP) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_OPERATION operation,
    IN VOID *hostAddress,
    IN OUT UINTN *numberOfBytes,
    OUT EFI_PHYSICAL_ADDRESS *deviceAddress,
    OUT VOID **mapping
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_UNMAP) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN VOID *mapping
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_ALLOCATE_TYPE type,
    IN EFI_MEMORY_TYPE memoryType,
    IN UINTN pages,
    OUT VOID **hostAddress,
    IN UINT64 attributes
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_FREE_BUFFER) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN UINTN pages,
    IN VOID *hostAddress
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_FLUSH) (
    IN EFI_PCI_IO_PROTOCOL *protocol
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_GET_LOCATION) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    OUT UINTN *segmentNumber,
    OUT UINTN *busNumber,
    OUT UINTN *deviceNumber,
    OUT UINTN *functionNumber
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_ATTRIBUTES) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION operation,
    IN UINT64 attributes,
    OUT UINT64 *result OPTIONAL
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN UINT8 barIndex,
    OUT UINT64 *supports, OPTIONAL
    OUT VOID **resources OPTIONAL
);
typedef EFI_STATUS (EFIAPI *EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES) (
    IN EFI_PCI_IO_PROTOCOL *protocol,
    IN UINT64 attributes,
    IN UINT8 barIndex,
    IN OUT UINT64 *offset,
    IN OUT UINT64 *length
);

typedef struct EFI_PCI_IO_PROTOCOL_ACCESS {
    EFI_PCI_IO_PROTOCOL_IO_MEM read;
    EFI_PCI_IO_PROTOCOL_IO_MEM write;
} EFI_PCI_IO_PROTOCOL_ACCESS;

typedef struct EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS {
    EFI_PCI_IO_PROTOCOL_CONFIG read;
    EFI_PCI_IO_PROTOCOL_CONFIG write;
} EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS;

typedef struct EFI_PCI_IO_PROTOCOL {
  EFI_PCI_IO_PROTOCOL_POLL_IO_MEM pollMem;
  EFI_PCI_IO_PROTOCOL_POLL_IO_MEM pollIo;
  EFI_PCI_IO_PROTOCOL_ACCESS mem;
  EFI_PCI_IO_PROTOCOL_ACCESS io;
  EFI_PCI_IO_PROTOCOL_CONFIG_ACCESS pci;
  EFI_PCI_IO_PROTOCOL_COPY_MEM copyMem;
  EFI_PCI_IO_PROTOCOL_MAP map;
  EFI_PCI_IO_PROTOCOL_UNMAP unmap;
  EFI_PCI_IO_PROTOCOL_ALLOCATE_BUFFER allocateBuffer;
  EFI_PCI_IO_PROTOCOL_FREE_BUFFER freeBuffer;
  EFI_PCI_IO_PROTOCOL_FLUSH flush;
  EFI_PCI_IO_PROTOCOL_GET_LOCATION getLocation;
  EFI_PCI_IO_PROTOCOL_ATTRIBUTES attributes;
  EFI_PCI_IO_PROTOCOL_GET_BAR_ATTRIBUTES getBarAttributes;
  EFI_PCI_IO_PROTOCOL_SET_BAR_ATTRIBUTES setBarAttributes;
  UINT64 romSize;
  VOID *romImage;
} EFI_PCI_IO_PROTOCOL;

#pragma endregion EFI_PCI_IO_PROTOCOL

// ==================================== |
//               EFI_BASE               |
// ==================================== |

#pragma region EFI_BASE

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL)(
    IN EFI_MEMORY_TYPE poolType,
    IN UINTN size,
    OUT VOID **buffer
);
typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL)(
    IN VOID *buffer
);

typedef EFI_STATUS(EFIAPI * EFI_ALLOCATE_PAGES)(
    IN EFI_ALLOCATE_TYPE type,
    IN EFI_MEMORY_TYPE memoryType,
    IN UINTN pages,
    IN OUT EFI_PHYSICAL_ADDRESS *memory
);
typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES) (
    IN EFI_PHYSICAL_ADDRESS memory,
    IN UINTN pages
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

typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP)(
    IN OUT UINTN *memoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR *memoryMap,
    OUT UINTN *mapKey,
    OUT UINTN *descriptorSize,
    OUT UINT32 *descriptorVersion
);

typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES)(
    IN EFI_HANDLE imageHandle,
    IN UINTN mapKey
);

typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT)(
    IN UINTN numberOfEvents,
    IN EFI_EVENT *event,
    OUT UINTN *index
);

typedef EFI_STATUS (EFIAPI *EFI_COPY_MEM)(
    IN VOID *destination,
    IN VOID *source,
    IN UINTN length
);

typedef struct EFI_MEMORY_DESCRIPTOR{
    UINT32 type;
    EFI_PHYSICAL_ADDRESS physicalStart;
    EFI_VIRTUAL_ADDRESS virtualStart;
    UINT64 numberOfPages;
    UINT64 attribute;
} EFI_MEMORY_DESCRIPTOR;

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
    EFI_ALLOCATE_PAGES allocatePages;
    EFI_FREE_PAGES freePages;
    EFI_GET_MEMORY_MAP getMemoryMap;
    EFI_ALLOCATE_POOL allocatePool;
    EFI_FREE_POOL freePool;
    VOID *createEvent;
    VOID *setTimer;
    EFI_WAIT_FOR_EVENT waitForEvent;
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
    EFI_EXIT_BOOT_SERVICES exitBootServices;
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
    EFI_COPY_MEM copyMem;
    VOID *setMem;
    VOID *createEventEx;
} EFI_BOOT_SERVICES;

#pragma endregion EFI_BASE

// ==================================== |
//              EFI_METHODS             |
// ==================================== |

EFI_STATUS EFI_Print(IN CHAR16* message);
EFI_STATUS EFI_AllocPool(IN EFI_MEMORY_TYPE memoryType, IN UINTN bufferSize, OUT VOID** buffer);
EFI_STATUS EFI_AllocPages(IN EFI_ALLOCATE_TYPE allocType, IN EFI_MEMORY_TYPE memType, IN UINTN pages, OUT EFI_PHYSICAL_ADDRESS* buffer);
EFI_STATUS EFI_DeAllocPool(IN VOID* buffer);
EFI_STATUS EFI_DeAllocPages(IN UINTN pages, IN EFI_PHYSICAL_ADDRESS buffer);
EFI_STATUS EFI_FindProtocol(IN EFI_GUID guid, OUT UINTN* count, OUT VOID** buffer);

CHAR16* EFI_GetStatus(IN EFI_STATUS status);

#endif