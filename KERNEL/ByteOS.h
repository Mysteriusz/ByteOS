// TYPE DEFINITIONS

#ifndef BYTE_OS_H

#define IN
#define OUT
#define NULL ((VOID*)0)
#define OPTIONAL
#define CONST const
#define ByteAPI __declspec(cdecl)


// ==================================== |
//                BOOLEAN               |
// ==================================== |

#define FALSE 0
#define TRUE 1
typedef unsigned char BOOLEAN;

// ==================================== |
//               INTIGERS               |
// ==================================== |

typedef unsigned long long UINT64;
#define UINT64_MAX 0xffffffffffffffff
#define UINT64_MIN 0x0000000000000000

typedef signed long long INT64;
#define INT64_MAX 0x7fffffffffffffff
#define INT64_MIN 0x8000000000000000

typedef unsigned long UINT32; 
#define UINT32_MAX 0xffffffff
#define UINT32_MIN 0x00000000

typedef signed long INT32;
#define INT32_MAX 0x7fffffff
#define INT32_MIN 0x80000000

typedef unsigned short UINT16;
#define UINT16_MAX 0xffff
#define UINT16_MIN 0x0000

typedef signed short INT16;
#define INT16_MAX 0x7fff
#define INT16_MIN 0x8000

typedef unsigned char UINT8;
#define UINT8_MAX 0xff
#define UINT8_MIN 0x00

typedef signed char INT8;
#define INT8_MAX 0x7f
#define INT8_MIN 0x80

typedef unsigned char BYTE;
#define BYTE_MAX 0xff
#define BYTE_MIN 0x00

typedef signed char SBYTE;
#define SBYTE_MAX 0x7f
#define SBYTE_MIN 0x80

typedef unsigned int UINT;
typedef signed int INT;

typedef unsigned int* UINTPTR;
typedef signed int* INTPTR;

typedef void VOID;

#ifdef _WIN64
    typedef unsigned long long UINTN;
    typedef signed long long INTN;
#else
    typedef unsigned int UINTN;
    typedef signed int INTN;
#endif

// =========== UINT8 ===========
// =========== UINT16 ===========
// =========== UINT32 ===========
// =========== UINT64 ===========

// ==================================== |
//                 CHARS                |
// ==================================== |

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
    UCHAR32 UCHAR8;
    UCHAR16 UCHAR16;
    UCHAR32 UCHAR32;
} UCHAR;
typedef union{
    STRING8 STRING8;
    STRING16 STRING16;
    STRING32 STRING32;
} STRING;

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
UINT64 GetLengthString8(CHAR8* str);

// =========== STRING16 ===========

BOOLEAN CompareString16(STRING16 str1, STRING16 str2);
UINT64 GetLengthString16(CHAR16* str);

// =========== STRING32 ===========

BOOLEAN CompareString32(STRING32 str1, STRING32 str2);
UINT64 GetLengthString32(CHAR32* str);

// ==================================== |
//                KERNEL                |
// ==================================== |

#define KERNEL_BASE_PATH L"\\KERNEL\\ByteOS.bin"
#define KERNEL_BASE_ALLOC_SIZE (10 * 1024 * 1024)

typedef VOID (*KERNEL_ENTRY_POINT)(VOID);

// ==================================== |
//                ERRORS                |
// ==================================== |

typedef UINTN KERNEL_LOAD_STATUS;
#define KERNEL_LOAD_SUCCESS 0
#define KERNEL_LOAD_ERROR_FILE 1
#define KERNEL_LOAD_ERROR_FILE_INFD 2
#define KERNEL_LOAD_ERROR_ALLOC 3
#define KERNEL_LOAD_ERROR_HANDLE_BUFFER 4
#define KERNEL_LOAD_ERROR_HANDLE_PROTOCOL 5
#define KERNEL_LOAD_ERROR_VOLUME 6
#define KERNEL_LOAD_ERROR_FILE_READ 7

CHAR16* GetKernelLoadStatus(KERNEL_LOAD_STATUS status);

#endif