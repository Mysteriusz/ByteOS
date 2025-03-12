#ifndef BYTE_OS_H
#define BYTE_OS_H

// ==================================== |
//              DEFINITIONS             |
// ==================================== |

#include "ByteOSChar.h"
#include "ByteOSNums.h"

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

CHAR8* UInt8ToChar8(UINT8 i);
CHAR8* UInt16ToChar8(UINT16 i);
CHAR8* UInt32ToChar8(UINT32 i);
CHAR8* UInt64ToChar8(UINT64 i);

CHAR8* UInt8ToChar8Hex(UINT8 i);
CHAR8* UInt16ToChar8Hex(UINT16 i);
CHAR8* UInt32ToChar8Hex(UINT32 i);
CHAR8* UInt64ToChar8Hex(UINT64 i);

CHAR8* ConcatChar8(CHAR8* str1, CHAR8* str2);
CHAR8* RemoveChar8(CHAR8* str, UINT32 len, UINT32 i);

// =========== CHAR16 ===========

CHAR16* UInt8ToChar16(UINT8 i);
CHAR16* UInt16ToChar16(UINT16 i);
CHAR16* UInt32ToChar16(UINT32 i);
CHAR16* UInt64ToChar16(UINT64 i);

CHAR16* UInt8ToChar16Hex(UINT8 i);
CHAR16* UInt16ToChar16Hex(UINT16 i);
CHAR16* UInt32ToChar16Hex(UINT32 i);
CHAR16* UInt64ToChar16Hex(UINT64 i);

CHAR16* ConcatChar16(CHAR16* str1, CHAR16* str2);
CHAR16* RemoveChar16(CHAR16* str, UINT32 len, UINT32 i);

// =========== CHAR32 ===========

CHAR32* UInt8ToChar32(UINT8 i);
CHAR32* UInt16ToChar32(UINT16 i);
CHAR32* UInt32ToChar32(UINT32 i);
CHAR32* UInt64ToChar32(UINT64 i);

CHAR32* UInt8ToChar32Hex(UINT8 i);
CHAR32* UInt16ToChar32Hex(UINT16 i);
CHAR32* UInt32ToChar32Hex(UINT32 i);
CHAR32* UInt64ToChar32Hex(UINT64 i);

CHAR32* ConcatChar32(CHAR32* str1, CHAR32* str2);
CHAR32* RemoveChar32(CHAR32* str, UINT32 len, UINT32 i);

// =========== STRING8 ===========

BOOLEAN CompareString8(STRING8 str1, STRING8 str2);
UINT64 GetLengthString8(STRING8* str);

// =========== STRING16 ===========

BOOLEAN CompareString16(STRING16 str1, STRING16 str2);
UINT64 GetLengthString16(STRING16* str);

// =========== STRING32 ===========

BOOLEAN CompareString32(STRING32 str1, STRING32 str2);
UINT64 GetLengthString32(STRING32* str);

#pragma endregion CHAR

// ==================================== |
//                KERNEL                |
// ==================================== |

#pragma region KERNEL

typedef struct KERNEL_GRAPHICAL_DEVICE_INFO{
    UINTN framebufferAddress;
    UINTN frameBufferSize;
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