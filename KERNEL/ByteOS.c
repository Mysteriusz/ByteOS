#include "ByteOS.h"

void Kernel_Main(void){
    UINT32 *fb = (UINT32 *)(0xc0000000);

    for (UINTN i = 0; i < 1280 * 800; i++) {
        fb[i] = 0xFFFFFF;
    }
    
    while (TRUE);
}

// ==================================== |
//                BOOLEAN               |
// ==================================== |

// ==================================== |
//               INTIGERS               |
// ==================================== |

// =========== UINT8 ===========
// =========== UINT16 ===========
// =========== UINT32 ===========
// =========== UINT64 ===========

// ==================================== |
//                 CHARS                |
// ==================================== |

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

UINTN GetLenghtChar8(CHAR8* str){
    UINTN len = 0;

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

UINTN GetLenghtChar16(CHAR16* str){
    UINTN len = 0;

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

UINTN GetLenghtChar32(CHAR32* str){
    UINTN len = 0;

    while (str[len] != '\0'){
        len++;
    }

    return len;
}

// =========== STRING8 ===========

BOOLEAN StringCompare8(STRING8 str1, STRING8 str2) {
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

// =========== STRING16 ===========

BOOLEAN StringCompare16(STRING16 str1, STRING16 str2) {
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

// =========== STRING32 ===========

BOOLEAN StringCompare32(STRING32 str1, STRING32 str2){
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

// ==================================== |
//                ERRORS                |
// ==================================== |

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