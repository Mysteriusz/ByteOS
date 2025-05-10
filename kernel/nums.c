#include "nums.h"

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
