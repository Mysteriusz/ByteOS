#include "char.h"

// =========== CHAR8 ===========

CHAR8* UInt8ToChar8(UINT8 i) {
    static CHAR8 buffer[4];

    if (i == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + '0';
        i /= 10;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt16ToChar8(UINT16 i) {
    static CHAR8 buffer[6];

    if (i == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + '0';
        i /= 10;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt32ToChar8(UINT32 i) {
    static CHAR8 buffer[11];

    if (i == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + '0';
        i /= 10;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt64ToChar8(UINT64 i) {
    static CHAR8 buffer[21];

    if (i == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + '0';
        i /= 10;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}

CHAR8* UInt8ToChar8Hex(UINT8 i) {
    static CHAR8 buffer[3];

    if (i == 0) {
        return (CHAR8*)"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        i /= 16;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt16ToChar8Hex(UINT16 i) {
    static CHAR8 buffer[5];

    if (i == 0) {
        return (CHAR8*)"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        i /= 16;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt32ToChar8Hex(UINT32 i) {
    static CHAR8 buffer[9];

    if (i == 0) {
        return (CHAR8*)"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        i /= 16;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR8* UInt64ToChar8Hex(UINT64 i) {
    static CHAR8 buffer[17];

    if (i == 0) {
        return (CHAR8*)"00";
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        i /= 16;
    }

    buffer[j] = '\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR8 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}

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

// =========== CHAR32 ===========

CHAR32* UInt8ToChar32(UINT8 i) {
    static CHAR32 buffer[4];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + U'0';
        i /= 10;
    }

    buffer[j] = U'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR32 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR32* UInt16ToChar32(UINT16 i) {
    static CHAR32 buffer[6];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + U'0';
        i /= 10;
    }

    buffer[j] = U'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR32 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR32* UInt32ToChar32(UINT32 i) {
    static CHAR32 buffer[11];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + U'0';
        i /= 10;
    }

    buffer[j] = U'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR32 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}
CHAR32* UInt64ToChar32(UINT64 i) {
    static CHAR32 buffer[21];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        buffer[j++] = (i % 10) + U'0';
        i /= 10;
    }

    buffer[j] = U'\0';

    for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
        CHAR32 temp = buffer[k];
        buffer[k] = buffer[l];
        buffer[l] = temp;
    }

    return buffer;
}

CHAR32* UInt8ToChar32Hex(UINT8 i) {
    static CHAR32 buffer[3];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'0';
        buffer[2] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + U'0') : (digit - 10 + U'a');
        i /= 16;
    }

    buffer[j] = U'\0';

    if (j == 1) {
        buffer[2] = U'\0';
        buffer[1] = buffer[0];
        buffer[0] = U'0';
    }

    return buffer;
}
CHAR32* UInt16ToChar32Hex(UINT16 i) {
    static CHAR32 buffer[5];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'0';
        buffer[2] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + U'0') : (digit - 10 + U'a');
        i /= 16;
    }

    buffer[j] = U'\0';

    return buffer;
}
CHAR32* UInt32ToChar32Hex(UINT32 i) {
    static CHAR32 buffer[9];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'0';
        buffer[2] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + U'0') : (digit - 10 + U'a');
        i /= 16;
    }

    buffer[j] = U'\0';

    return buffer;
}
CHAR32* UInt64ToChar32Hex(UINT64 i) {
    static CHAR32 buffer[17];

    if (i == 0) {
        buffer[0] = U'0';
        buffer[1] = U'0';
        buffer[2] = U'\0';
        return buffer;
    }

    INT32 j = 0;
    while (i > 0) {
        INT32 digit = i % 16;
        buffer[j++] = (digit < 10) ? (digit + U'0') : (digit - 10 + U'a');
        i /= 16;
    }

    buffer[j] = U'\0';

    return buffer;
}

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

// =========== STRING8 ===========

BOOLEAN CompareString8(STRING8 str1, STRING8 str2) {
    while (*str1 == *str2) {
        if (*str1 == L'\0' && *str2 == L'\0') {
            return TRUE;
        }

        else if (*str1 == L'\0' || *str2 == L'\0') {
            return FALSE;
        }
        
        str1++;
        str2++;
    }

    return FALSE;
}
UINT64 GetLengthString8(STRING8* str){
    UINT64 len = 0;

    while (*str[len] != '\0'){
        len++;
    }

    return len;
}

// =========== STRING16 ===========

BOOLEAN CompareString16(STRING16 str1, STRING16 str2) {
    if (str1 == NULL || str2 == NULL) return FALSE;

    while (*str1 && *str2 && *str1 == *str2) {
        str1++; str2++;
    }

    return (*str1 == *str2);
}
UINT64 GetLengthString16(CHAR16* str){
    UINT64 len = 0;

    while (str[len] != L'\0'){
        len++;
    }

    return len;
}

// =========== STRING32 ===========

BOOLEAN CompareString32(STRING32 str1, STRING32 str2){
    while (*str1 == *str2) {
        if (*str1 == L'\0' && *str2 == L'\0') {
            return TRUE;
        }

        else if (*str1 == L'\0' || *str2 == L'\0') {
            return FALSE;
        }
        
        str1++;
        str2++;
    }

    return FALSE;
}
UINT64 GetLengthString32(STRING32* str){
    UINT64 len = 0;

    while (*str[len] != '\0'){
        len++;
    }

    return len;
}
