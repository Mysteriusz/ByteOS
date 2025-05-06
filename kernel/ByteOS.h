#pragma once

// ==================================== |
//              DEFINITIONS             |
// ==================================== |

#define IN
#define OUT
#define NULL (VOID*)0
#define OPTIONAL
#define CONST const
#define ByteAPI

#ifdef __aarch32__
#define AARCH_32
#endif

#ifdef __x86_64__
#define X86_64
#endif

// ==================================== |
//                INTIGERS              |
// ==================================== |

#pragma region INTIGERS

typedef unsigned long long UINT64;
typedef UINT64 ULONGLONG;
typedef UINT64 QWORD;
typedef UINT64* UINT64PTR;
#define UINT64_MAX 0xffffffffffffffff
#define UINT64_MIN 0x0000000000000000

typedef signed long long INT64;
typedef INT64 LONGLONG;
typedef INT64* INT64PTR;
#define INT64_MAX 0x7fffffffffffffff
#define INT64_MIN 0x8000000000000000

typedef unsigned long UINT32; 
typedef UINT32 ULONG; 
typedef UINT32 DWORD;
typedef UINT32* UINT32PTR; 
#define UINT32_MAX 0xffffffff
#define UINT32_MIN 0x00000000

typedef signed long INT32;
typedef INT32 LONG;
typedef INT32* INT32PTR;
#define INT32_MAX 0x7fffffff
#define INT32_MIN 0x80000000

typedef unsigned short UINT16;
typedef UINT16 USHORT;
typedef UINT16 WORD;
typedef UINT16* UINT16PTR;
#define UINT16_MAX 0xffff
#define UINT16_MIN 0x0000

typedef signed short INT16;
typedef INT16 SHORT;
typedef INT16* INT16PTR;
#define INT16_MAX 0x7fff
#define INT16_MIN 0x8000

typedef unsigned char UINT8;
typedef UINT8* UINT8PTR;
#define UINT8_MAX 0xff
#define UINT8_MIN 0x00

typedef signed char INT8;
typedef INT8* INT8PTR;
#define INT8_MAX 0x7f
#define INT8_MIN 0x80

typedef unsigned char BYTE;
typedef BYTE* BYTEPTR;
#define BYTE_MAX 0xff
#define BYTE_MIN 0x00

typedef signed char SBYTE;
typedef SBYTE* SBYTEPTR;
#define SBYTE_MAX 0x7f
#define SBYTE_MIN 0x80

typedef unsigned int UINT;
typedef UINT* UINTPTR;

typedef signed int INT;
typedef INT* INTPTR;

typedef void VOID;
typedef VOID *VOIDPTR;

#define FALSE 0
#define TRUE 1
typedef unsigned char BOOLEAN;

#ifdef X86_64 
    typedef UINT64 UINTN;
    typedef INT64 INTN;
#else
    typedef UINT32 UINTN;
    typedef INT32 INTN;
#endif

#ifdef X86_64
    typedef UINT64 PHYSICAL_ADDRESS;
    typedef UINT64 VIRTUAL_ADDRESS;
#else
    typedef UINT32 PHYSICAL_ADDRESS;
    typedef UINT32 VIRTUAL_ADDRESS;
#endif

#pragma endregion INTIGERS

// ==================================== |
//                 CHARS                |
// ==================================== |

#pragma region CHARS

typedef unsigned int CHAR32;
typedef CHAR32 WCHAR32;
typedef CHAR32 UCHAR32;
typedef CHAR32* STRING32;

typedef unsigned short CHAR16;
typedef CHAR16 WCHAR16;
typedef CHAR16 UCHAR16;
typedef CHAR16* STRING16;

typedef unsigned char CHAR8;
typedef CHAR8 WCHAR8;
typedef CHAR8 UCHAR8;
typedef CHAR8* STRING8;

typedef union {
    CHAR8 CHAR8;
    CHAR16 CHAR16;
    CHAR32 CHAR32;
} CHAR;
typedef union {
    WCHAR8 WCHAR8;
    WCHAR16 WCHAR16;
    WCHAR32 WCHAR32;
} WCHAR;
typedef union {
    UCHAR8 UCHAR8;
    UCHAR16 UCHAR16;
    UCHAR32 UCHAR32;
} UCHAR;
typedef union{
    STRING8 STRING8;
    STRING16 STRING16;
    STRING32 STRING32;
} STRING;

#pragma endregion CHARS

// ==================================== |
//                KERNEL                |
// ==================================== |

#pragma region KERNEL

typedef struct KERNEL_GRAPHICAL_DEVICE_INFO{
    UINTN framebufferAddress;
    UINTN frameBufferSize;
    UINTN horizontalRes;
    UINTN verticalRes;
} KERNEL_GRAPHICAL_DEVICE_INFO;
typedef struct KERNEL_IO_DEVICE_INFO{
    VOID *pciAddress;
} KERNEL_IO_DEVICE_INFO;
typedef struct KERNEL_CPU_DEVICE_INFO{
    CHAR8 *manufacturer;
} KERNEL_CPU_DEVICE_INFO;
typedef struct KERNEL_DEVICE_INFO{
    KERNEL_GRAPHICAL_DEVICE_INFO *gpui;
    UINT32 gpuiCount;
    KERNEL_CPU_DEVICE_INFO *cpui;
    UINT32 cpuiCount;
    KERNEL_IO_DEVICE_INFO *ioi;
    UINT32 ioiCount;
} KERNEL_DEVICE_INFO;
typedef struct KERNEL_MEMORY_DESCRIPTOR{
    UINT32 type;
    PHYSICAL_ADDRESS physicalStart;
    VIRTUAL_ADDRESS virtualStart;
    UINT64 size;
    UINT64 attribute;
} KERNEL_MEMORY_DESCRIPTOR;
typedef struct KERNEL_MEMORY_MAP{
    UINTN size;
    UINTN mapKey;
    UINTN descriptorSize;
    UINT32 descriptorVersion;
    UINT32 entryCount;
    UINT64 usableSize;
    KERNEL_MEMORY_DESCRIPTOR entries[];
} KERNEL_MEMORY_MAP;

typedef VOID (*KERNEL_ENTRY_POINT)(KERNEL_DEVICE_INFO devInfo, KERNEL_MEMORY_MAP *memMap);
typedef UINTN KERNEL_LOAD_STATUS;

#define KERNEL_LOAD_SUCCESS 0
#define KERNEL_LOAD_ERROR_FILE 1
#define KERNEL_LOAD_ERROR_FILE_INFD 2
#define KERNEL_LOAD_ERROR_ALLOC 3
#define KERNEL_LOAD_ERROR_HANDLE_BUFFER 4
#define KERNEL_LOAD_ERROR_HANDLE_PROTOCOL 5
#define KERNEL_LOAD_ERROR_VOLUME 6
#define KERNEL_LOAD_ERROR_FILE_READ 7

CHAR16* ByteAPI GetKernelLoadStatus(KERNEL_LOAD_STATUS status);

#pragma endregion KERNEL

// ==================================== |
//               INTERNAL               |
// ==================================== |

#pragma region INTERNAL

typedef UINT32 BT_STATUS;
#define BT_ERROR(status)(((UINTN)(status)) > 0)

#define FIT_IN(s1, s2)(((s2) + (s1) - 1) / (s1))
#define FIT_IN_SIZE(s1, s2)((s1) * FIT_IN(s1, s2))

#define PAD_UP(s1, s2)(((s1) + ((s2) - 1)) & ~((s2) - 1))
#define PAD_DOWN(s1, s2)((s1) & ~((s2) - 1))     

#define OFFSET_OF(type, member) ((UINTN)&(((type*)0)->member))

#define BT_SUCCESS 0x00

#define BT_INVALID_ARGUMENT 0x10
#define BT_TIMEOUT_ERROR 0x11
#define BT_NOT_FOUND 0x12
#define BT_UNINITIALIZED 0x13

#define BT_MEMORY_OVERFLOW 0x100
#define BT_NOT_ENOUGH_MEMORY 0x101
#define BT_UNKNOWN_MEMORY 0x102
#define BT_INVALID_MEMORY 0x103

#define BT_ACCESS_VIOLATION 0x1000
#define BT_MEMORY_NOT_WRITABLE 0x1001
#define BT_MEMORY_NOT_READABLE 0x1002
#define BT_MEMORY_NOT_EXECUTABLE 0x1003
#define BT_INVALID_POOL_BLOCK_SIZE 0x1004
#define BT_INVALID_BUFFER 0x1005
#define BT_INVALID_PAGE_INDEX 0x1006
#define BT_INVALID_PAGE_ADDRESS 0x1007

#define BT_IO_DISK_OVERFLOW 0x2000
#define BT_IO_INVALID_SYMBOL 0x2001
#define BT_IO_INVALID_PCI 0x2002
#define BT_IO_INVALID_DISK 0x2003
#define BT_IO_DISK_REGISTERED 0x2004
#define BT_IO_DISK_INTEGRITY 0x2005
#define BT_IO_DISK_TOO_SMALL 0x2006

#define BT_DIRECTORY_NOT_FOUND 0x4000

#define BT_ENGINE_RUNNING_ERROR 0x100000
#define BT_ENGINE_NOT_RUNNING_ERROR 0x100001

#pragma endregion INTERNAL

#include "nums.h"
#include "char.h"
#include "memory/memory.h"

#pragma region DEPENDANT

#define GUID_MIN \
    { 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
#define GUID_MAX \
    { 0xffffffff, 0xffff, 0xffff, { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } }

typedef struct UUID{
    UINT32 timeLow;
    UINT16 timeMid;
    UINT16 version : 4; 
    UINT16 timeHigh : 12; 
    UINT16 variant : 2; 
    UINT16 clockSequence : 14; 
    UINT8 reseved[6]; 
} UUID;
typedef struct GUID{
    UINT32 data1;
    UINT16 data2;
    UINT16 data3;
    UINT8 data4[8];
} GUID;

#pragma endregion DEPENDANT
