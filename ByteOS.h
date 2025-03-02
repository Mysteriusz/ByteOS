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

typedef int INT;

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

CHAR8* ByteAPI Concat8(CHAR8* b1, CHAR8* b2){
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
CHAR16* ByteAPI Concat16(CHAR16* b1, CHAR16* b2){
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
CHAR32* ByteAPI Concat32(CHAR32* b1, CHAR32* b2){
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

CHAR16* ByteAPI Char8ToChar16(CHAR8* value){

}
CHAR32* ByteAPI Char8ToChar32(CHAR8* value){
    
}

CHAR8* ByteAPI Char16ToChar8(CHAR16* value){

}
CHAR32* ByteAPI Char16ToChar32(CHAR16* value){
    
}

CHAR8* ByteAPI Char32ToChar8(CHAR32* value){
    
}
CHAR16* ByteAPI Char32ToChar16(CHAR32* value){
    
}