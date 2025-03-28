#pragma once

#include "byteos.h"

// ==================================== |
//                 CHARS                |
// ==================================== |

#pragma region CHARS

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

#pragma endregion CHARS