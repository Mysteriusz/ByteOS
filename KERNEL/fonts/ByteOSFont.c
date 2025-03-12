#include "ByteOSFont.h"

VOID PrintChar(BTS_FONT font, BTS_CHAR *character, UINT32 *frameBufferAddress, UINT32 frameBufferHorizontalSize){
    for (INT8 y = 0; y < font.header.verticalSize; y++){
        for (INT8 x = 0; x < font.header.horizontalSize; x++) {
            BOOLEAN bit = (character->points[y] >> x) & 1; 
            frameBufferAddress[(y * frameBufferHorizontalSize) + x] = bit == TRUE ? 0xffffff : 0x000000;
        }
    }
}