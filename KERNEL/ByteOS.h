#ifndef BYTE_OS_H
#define BYTE_OS_H

// ==================================== |
//              DEFINITIONS             |
// ==================================== |

#include "Char.h"
#include "Nums.h"

#define IN
#define OUT
#define NULL ((VOID*)0)
#define OPTIONAL
#define CONST const
#define ByteAPI

// ==================================== |
//                 NUMS                 |
// ==================================== |

#pragma region NUMS

// =========== UINT8 ===========

UINT8 ByteAPI Char8ToUInt8(CHAR8* str);
UINT8 ByteAPI Char16ToUInt8(CHAR16* str);
UINT8 ByteAPI Char32ToUInt8(CHAR32* str);

// =========== UINT16 ===========

UINT16 ByteAPI Char8ToUInt16(CHAR8* str);
UINT16 ByteAPI Char16ToUInt16(CHAR16* str);
UINT16 ByteAPI Char32ToUInt16(CHAR32* str);

// =========== UINT32 ===========

UINT32 ByteAPI Char8ToUInt32(CHAR8* str);
UINT32 ByteAPI Char16ToUInt32(CHAR16* str);
UINT32 ByteAPI Char32ToUInt32(CHAR32* str);

// =========== UINT64 ===========

UINT64 ByteAPI Char8ToUInt64(CHAR8* str);
UINT64 ByteAPI Char16ToUInt64(CHAR16* str);
UINT64 ByteAPI Char32ToUInt64(CHAR32* str);

#pragma endregion NUMS

// ==================================== |
//                 CHAR                 |
// ==================================== |

#pragma region CHAR

// =========== CHAR8 ===========

CHAR8* ByteAPI UInt8ToChar8(UINT8 i);
CHAR8* ByteAPI UInt16ToChar8(UINT16 i);
CHAR8* ByteAPI UInt32ToChar8(UINT32 i);
CHAR8* ByteAPI UInt64ToChar8(UINT64 i);

CHAR8* ByteAPI UInt8ToChar8Hex(UINT8 i);
CHAR8* ByteAPI UInt16ToChar8Hex(UINT16 i);
CHAR8* ByteAPI UInt32ToChar8Hex(UINT32 i);
CHAR8* ByteAPI UInt64ToChar8Hex(UINT64 i);

CHAR8* ByteAPI ConcatChar8(CHAR8* str1, CHAR8* str2);
CHAR8* ByteAPI RemoveChar8(CHAR8* str, UINT32 len, UINT32 i);

// =========== CHAR16 ===========

CHAR16* ByteAPI UInt8ToChar16(UINT8 i);
CHAR16* ByteAPI UInt16ToChar16(UINT16 i);
CHAR16* ByteAPI UInt32ToChar16(UINT32 i);
CHAR16* ByteAPI UInt64ToChar16(UINT64 i);

CHAR16* ByteAPI UInt8ToChar16Hex(UINT8 i);
CHAR16* ByteAPI UInt16ToChar16Hex(UINT16 i);
CHAR16* ByteAPI UInt32ToChar16Hex(UINT32 i);
CHAR16* ByteAPI UInt64ToChar16Hex(UINT64 i);

CHAR16* ByteAPI ConcatChar16(CHAR16* str1, CHAR16* str2);
CHAR16* ByteAPI RemoveChar16(CHAR16* str, UINT32 len, UINT32 i);

// =========== CHAR32 ===========

CHAR32* ByteAPI UInt8ToChar32(UINT8 i);
CHAR32* ByteAPI UInt16ToChar32(UINT16 i);
CHAR32* ByteAPI UInt32ToChar32(UINT32 i);
CHAR32* ByteAPI UInt64ToChar32(UINT64 i);

CHAR32* ByteAPI UInt8ToChar32Hex(UINT8 i);
CHAR32* ByteAPI UInt16ToChar32Hex(UINT16 i);
CHAR32* ByteAPI UInt32ToChar32Hex(UINT32 i);
CHAR32* ByteAPI UInt64ToChar32Hex(UINT64 i);

CHAR32* ByteAPI ConcatChar32(CHAR32* str1, CHAR32* str2);
CHAR32* ByteAPI RemoveChar32(CHAR32* str, UINT32 len, UINT32 i);

// =========== STRING8 ===========

BOOLEAN ByteAPI CompareString8(STRING8 str1, STRING8 str2);
UINT64 ByteAPI GetLengthString8(STRING8* str);

// =========== STRING16 ===========

BOOLEAN ByteAPI CompareString16(STRING16 str1, STRING16 str2);
UINT64 ByteAPI GetLengthString16(STRING16* str);

// =========== STRING32 ===========

BOOLEAN ByteAPI CompareString32(STRING32 str1, STRING32 str2);
UINT64 ByteAPI GetLengthString32(STRING32* str);

#pragma endregion CHAR

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
typedef struct KERNEL_CPU_DEVICE_INFO{
    CHAR8 *manufacturer;
} KERNEL_CPU_DEVICE_INFO;
typedef struct KERNEL_DEVICE_INFO{
    KERNEL_GRAPHICAL_DEVICE_INFO *gpui;
    UINT32 gpuiCount;
    KERNEL_CPU_DEVICE_INFO *cpui;
    UINT32 cpuiCount;
} KERNEL_DEVICE_INFO;

typedef VOID (*KERNEL_ENTRY_POINT)(KERNEL_DEVICE_INFO devInfo);
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

#endif