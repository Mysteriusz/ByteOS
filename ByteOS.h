// TYPE DEFINITIONS

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

// ==================================== |
//                 CHARS                |
// ==================================== |

typedef unsigned int CHAR32;
typedef unsigned short CHAR16;
typedef unsigned char CHAR8;
typedef char CHAR;

CHAR8* ByteAPI ConcatChar8(CHAR8* b1, CHAR8* b2){
    static CHAR8 buffer[256];
    UINTN i = 0;

    while (*b1 != '\0'){
        buffer[i++] = *b1++;
    }
    while (*b2 != '\0'){
        buffer[i++] = *b2++;
    }

    buffer[i] = '\0';
    return buffer;  
}
CHAR16* ByteAPI ConcatChar16(CHAR16* b1, CHAR16* b2){
    static CHAR16 buffer[256];
    UINTN i = 0;

    while (*b1 != '\0'){
        buffer[i++] = *b1++;
    }
    while (*b2 != '\0'){
        buffer[i++] = *b2++;
    }

    buffer[i] = '\0';
    return buffer;  
}
CHAR32* ByteAPI ConcatChar32(CHAR32* b1, CHAR32* b2){
    static CHAR32 buffer[256];
    UINTN i = 0;

    while (*b1 != '\0'){
        buffer[i++] = *b1++;
    }
    while (*b2 != '\0'){
        buffer[i++] = *b2++;
    }

    buffer[i] = '\0';
    return buffer;  
}

// ==================================== |
//                KERNEL                |
// ==================================== |

#define KERNEL_BASE_PATH L"\\KERNEL\\ByteOS.bin"
#define KERNEL_BASE_SIZE 1024

typedef VOID (*KERNEL_ENTRY_POINT)(VOID);

// ==================================== |
//                ERRORS                |
// ==================================== |

typedef UINTN KERNEL_LOAD_STATUS;
#define KERNEL_LOAD_SUCCESS 0
#define KERNEL_LOAD_ERROR_FILE 1
#define KERNEL_LOAD_ERROR_ALLOC 2
#define KERNEL_LOAD_ERROR_HANDLE_BUFFER 3
#define KERNEL_LOAD_ERROR_HANDLE_PROTOCOL 4
#define KERNEL_LOAD_ERROR_VOLUME 5

CHAR16* GetKernelLoadStatus(KERNEL_LOAD_STATUS status) {
    switch (status) {
        case KERNEL_LOAD_SUCCESS:
        return L"Kernel loaded successfully.";
        case KERNEL_LOAD_ERROR_FILE:
            return L"Failed to open kernel file.";
        case KERNEL_LOAD_ERROR_ALLOC:
            return L"Memory allocation failed.";
        case KERNEL_LOAD_ERROR_HANDLE_BUFFER:
            return L"Failed to retrieve handle buffer.";
        case KERNEL_LOAD_ERROR_HANDLE_PROTOCOL:
            return L"Failed to handle protocol.";
        case KERNEL_LOAD_ERROR_VOLUME:
            return L"Failed to open file system volume.";
        default:
            return L"Unknown error.";
    }
}