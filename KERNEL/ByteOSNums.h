#ifndef BYTEOS_NUMS_H
#define BYTEOS_NUMS_H

#include "ByteOS.h"

// ==================================== |
//               INTIGERS               |
// ==================================== |

#pragma region NUMS

typedef unsigned long long UINT64;
typedef UINT64* UINT64PTR;

#define UINT64_MAX 0xffffffffffffffff
#define UINT64_MIN 0x0000000000000000

typedef signed long long INT64;
typedef INT64* INT64PTR;
#define INT64_MAX 0x7fffffffffffffff
#define INT64_MIN 0x8000000000000000

typedef unsigned long UINT32; 
typedef UINT32* UINT32PTR; 
#define UINT32_MAX 0xffffffff
#define UINT32_MIN 0x00000000

typedef signed long INT32;
typedef INT32* INT32PTR;
#define INT32_MAX 0x7fffffff
#define INT32_MIN 0x80000000

typedef unsigned short UINT16;
typedef UINT16* UINT16PTR;
#define UINT16_MAX 0xffff
#define UINT16_MIN 0x0000

typedef signed short INT16;
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

#define FALSE 0
#define TRUE 1
typedef unsigned char BOOLEAN;

#ifdef _WIN64
    typedef unsigned long long UINTN;
    typedef signed long long INTN;
#else
    typedef unsigned int UINTN;
    typedef signed int INTN;
#endif

#pragma endregion NUMS

#endif