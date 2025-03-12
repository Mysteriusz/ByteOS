#include "ByteOS.h"
#include "fonts/ByteOSFont.h"
#include "../efi/EFITypes.h"

// ==================================== |
//                KERNEL                |
// ==================================== |

void Kernel_Main(KERNEL_DEVICE_INFO devInfo){
    UINT32 *fb = (UINT32*)(devInfo.gpui[0].framebufferAddress);

    BTS_FONT font;
    font.header.horizontalSize = BASE_FONT_WIDTH;
    font.header.verticalSize = BASE_FONT_HEIGHT;

    for (UINTN i = 0; i < devInfo.gpui[0].horizontalRes * devInfo.gpui[0].verticalRes; i++) {
        fb[i] = 0x000000;
    }

    BTS_CHAR c = {BTS_CAPITAL_A}; 
    PrintChar(font, &c, fb, devInfo.gpui[0].horizontalRes);
    
    while (TRUE);
}

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

// ==================================== |
//                 NUMS                 |
// ==================================== |

#pragma region NUMS

// =========== UINT8 ===========

UINT8 ByteAPI Char8ToUInt8(CHAR8* str){
    UINT8 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR8 c = str[i];
        UINT8 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'A' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT8 ByteAPI Char16ToUInt8(CHAR16* str){
    UINT8 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR16 c = str[i];
        UINT8 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'A' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT8 ByteAPI Char32ToUInt8(CHAR32* str){
    UINT8 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR32 c = str[i];
        UINT8 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'A' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}

// =========== UINT16 ===========

UINT16 ByteAPI Char8ToUInt16(CHAR8* str){
    UINT16 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR8 c = str[i];
        UINT16 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT16 ByteAPI Char16ToUInt16(CHAR16* str){
    UINT16 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR16 c = str[i];
        UINT16 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT16 ByteAPI Char32ToUInt16(CHAR32* str){
    UINT16 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR32 c = str[i];
        UINT16 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}

// =========== UINT32 ===========

UINT32 ByteAPI Char8ToUInt32(CHAR8* str){
    UINT32 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR8 c = str[i];
        UINT32 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT32 ByteAPI Char16ToUInt32(CHAR16* str){
    UINT32 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR16 c = str[i];
        UINT32 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT32 ByteAPI Char32ToUInt32(CHAR32* str){
    UINT32 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR32 c = str[i];
        UINT32 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}

// =========== UINT64 ===========

UINT64 ByteAPI Char8ToUInt64(CHAR8* str){
    UINT64 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR8 c = str[i];
        UINT64 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT64 ByteAPI Char16ToUInt64(CHAR16* str){
    UINT64 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR16 c = str[i];
        UINT64 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}
UINT64 ByteAPI Char32ToUInt64(CHAR32* str){
    UINT64 res = 0;

    for (UINTN i = 0; str[i] != '\0'; i++){
        if (i == 0 && str[0] == '0' && str[1] == 'x'){
            i = 2;
        }

        CHAR32 c = str[i];
        UINT64 val = 0;

        if (c >= '0' && c <= '9'){
            val = c - '0';
        }
        else if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f') {
            val = c - 'a' + 10;
        }
        else {
            return 0;
        }

        res = (res * 16) + val;
    }

    return res;
}

#pragma endregion NUMS

// ==================================== |
//                 CHAR                 |
// ==================================== |

#pragma region CHAR

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
UINT64 GetLengthString16(STRING16* str){
    UINT64 len = 0;

    while (*str[len] != '\0'){
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

#pragma endregion CHAR