#include "crc.h"

UINT32 crc32Table[0x100];
BOOLEAN crc32Initialized = FALSE;

BT_STATUS ByteAPI Crc32Initialize(){
    UINT32 crc32 = 1;
    for (UINT32 i = 128; i; i >>= 1){
        crc32 = (crc32 >> 1) ^ (crc32 & 1 ? 0xedb88320 : 0);
        for (UINT32 j = 0; j < 0x100; j += 2 * i){
            crc32Table[i + j] = crc32 ^ crc32Table[j];
        }
    }

    crc32Initialized = TRUE;
    return BT_SUCCESS;
}
BT_STATUS ByteAPI Crc32(IN UINT8 *byteArray, IN UINT32 length, OUT UINT32 *remainder){
    if (byteArray == NULL) return BT_INVALID_ARGUMENT;
    if (remainder == NULL) return BT_INVALID_BUFFER;

    if (crc32Initialized == FALSE){
        Crc32Initialize(); 
    }

    *remainder = 0xffffffff;

    for (UINTN i = 0; i < length; i++){
        *remainder ^= byteArray[i];
        *remainder = (*remainder >> 8) ^ crc32Table[*remainder & 0xff];
    }

    *remainder ^= 0xffffffff;
    return BT_SUCCESS;
}
