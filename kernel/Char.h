#ifndef BYTEOS_CHAR_H
#define BYTEOS_CHAR_H

#include "byteos.h"

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

#endif