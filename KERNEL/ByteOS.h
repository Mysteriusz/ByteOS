// TYPE DEFINITIONS

#ifndef BYTE_OS_H

#define IN
#define OUT
#define NULL ((VOID*)0)
#define OPTIONAL
#define CONST const
#define ByteAPI __declspec(cdecl)

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

#define FALSE 0
#define TRUE 1
typedef unsigned char BOOLEAN;

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
typedef unsigned short CHAR16;
typedef unsigned char CHAR8;
typedef char CHAR;

// =========== CHAR8 ===========

CHAR8* ConcatChar8(CHAR8* str1, CHAR8* str2){
    static CHAR8 buffer[256];
    UINTN i = 0;

    while (*str1 != '\0'){
        buffer[i++] = *str1++;
    }
    while (*str2 != '\0'){
        buffer[i++] = *str2++;
    }

    buffer[i] = '\0';
    return buffer;  
}
CHAR8* RemoveChar8(CHAR8* str, UINT32 len, UINT32 i){
    static CHAR8 buffer[256];
    UINT32 curr = 0;

    while (curr < len){
        if (curr == i){
            continue;
        }

        buffer[curr] = str[curr]; 
        curr++;
    }

    return buffer;
}

UINT64 GetLenghtChar8(CHAR8* str){
    UINT64 len = 0;

    while (str[len] != '\0'){
        len++;
    }

    return len;
}

// =========== CHAR16 ===========

CHAR16* UInt8ToChar16(UINT8 i){
    static CHAR16 buffer[4];

    if (i == 0) {
        buffer[0] = u'0';
        buffer[1] = u'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0){
        buffer[j++] = (i % 10) + u'0';
        i /= 10;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt16ToChar16(UINT16 i){
    static CHAR16 buffer[6];

    if (i == 0) {
        buffer[0] = u'0';
        buffer[1] = u'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0){
        buffer[j++] = (i % 10) + u'0';
        i /= 10;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt32ToChar16(UINT32 i){
    static CHAR16 buffer[11];

    if (i == 0) {
        buffer[0] = u'0';
        buffer[1] = u'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0){
        buffer[j++] = (i % 10) + u'0';
        i /= 10;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt64ToChar16(UINT64 i){
    static CHAR16 buffer[21];

    if (i == 0) {
        buffer[0] = u'0';
        buffer[1] = u'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0){
        buffer[j++] = (i % 10) + u'0';
        i /= 10;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}

CHAR16* UInt8ToChar16Hex(UINT8 i){
    static CHAR16 buffer[2];

    if (i == 0){
        return L"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        if (digit < 10) {
            buffer[j++] = (digit) + u'0';
        } else {
            buffer[j++] = (digit - 10) + u'a';
        }
        i /= 16;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt16ToChar16Hex(UINT16 i){
    static CHAR16 buffer[5];

    if (i == 0){
        return L"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        if (digit < 10) {
            buffer[j++] = (digit) + u'0';
        } else {
            buffer[j++] = (digit - 10) + u'a';
        }
        i /= 16;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt32ToChar16Hex(UINT32 i){
    static CHAR16 buffer[9];

    if (i == 0){
        return L"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        if (digit < 10) {
            buffer[j++] = (digit) + u'0';
        } else {
            buffer[j++] = (digit - 10) + u'a';
        }
        i /= 16;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR16* UInt64ToChar16Hex(UINT64 i){
    static CHAR16 buffer[17];

    if (i == 0){
        return L"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        if (digit < 10) {
            buffer[j++] = (digit) + u'0';
        } else {
            buffer[j++] = (digit - 10) + u'a';
        }
        i /= 16;
    }

    buffer[j] = u'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--){
        CHAR16 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}

CHAR16* ConcatChar16(CHAR16* str1, CHAR16* str2){
    static CHAR16 buffer[256];
    UINTN i = 0;

    while (*str1 != '\0'){
        buffer[i++] = *str1++;
    }
    while (*str2 != '\0'){
        buffer[i++] = *str2++;
    }

    buffer[i] = '\0';
    return buffer;  
}
CHAR16* RemoveChar16(CHAR16* str, UINT32 len, UINT32 i){
    static CHAR16 buffer[256];
    UINT32 curr = 0;

    while (curr < len){
        if (curr == i){
            continue;
        }

        buffer[curr] = str[curr]; 
        curr++;
    }

    return buffer;
}

UINT64 GetLenghtChar16(CHAR16* str){
    UINT64 len = 0;

    while (str[len] != '\0'){
        len++;
    }

    return len;
}

// =========== CHAR32 ===========

CHAR32* ConcatChar32(CHAR32* str1, CHAR32* str2){
    static CHAR32 buffer[256];
    UINTN i = 0;

    while (*str1 != '\0'){
        buffer[i++] = *str1++;
    }
    while (*str2 != '\0'){
        buffer[i++] = *str2++;
    }

    buffer[i] = '\0';
    return buffer;  
}
CHAR32* RemoveChar32(CHAR32* str, UINT32 len, UINT32 i){
    static CHAR32 buffer[256];
    UINT32 curr = 0;

    while (curr < len){
        if (curr == i){
            continue;
        }

        buffer[curr] = str[curr]; 
        curr++;
    }

    return buffer;
}

UINT64 GetLenghtChar32(CHAR32* str){
    UINT32 len = 0;

    while (str[len] != '\0'){
        len++;
    }

    return len;
}

// ==================================== |
//                KERNEL                |
// ==================================== |

#define KERNEL_BASE_PATH L"\\KERNEL\\ByteOS.bin"
#define KERNEL_BASE_SIZE (10 * 1024 * 1024)

typedef struct KERNEL_ENTRY_FRAME_BUFFER_INFO{
    UINTN *address;
    UINTN *size;
    UINT32 *width;
    UINT32 *height;
    UINT32 *perLine;
} KERNEL_ENTRY_FRAME_BUFFER_INFO;

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

CHAR16* GetKernelLoadStatus(KERNEL_LOAD_STATUS status) {
    switch (status) {
        case KERNEL_LOAD_SUCCESS:
        return L"Kernel loaded successfully.";
        case KERNEL_LOAD_ERROR_FILE:
            return L"Failed to open kernel file.";      
        case KERNEL_LOAD_ERROR_FILE_INFD:
            return L"Failed to read kernel file information.";
        case KERNEL_LOAD_ERROR_ALLOC:
            return L"Memory allocation failed.";
        case KERNEL_LOAD_ERROR_HANDLE_BUFFER:
            return L"Failed to retrieve handle buffer.";
        case KERNEL_LOAD_ERROR_HANDLE_PROTOCOL:
            return L"Failed to handle protocol.";
        case KERNEL_LOAD_ERROR_VOLUME:
            return L"Failed to open file system volume.";
        case KERNEL_LOAD_ERROR_FILE_READ:
            return L"Failed to read from kernel file.";
        default:
            return L"Unknown error.";
    }
}

#endif