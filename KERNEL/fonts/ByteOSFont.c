#include "ByteOSFont.h"

VOID PrintChar(BTS_FONT font, BTS_CHAR *character, UINT32 *frameBufferAddress, UINT32 frameBufferHorizontalSize) {
    for (INT8 y = 0; y < BASE_FONT_HEIGHT; y++) {
        for (INT8 x = 0; x < BASE_FONT_WIDTH; x++) {
            BOOLEAN bit = (character->points[y] >> (BASE_FONT_WIDTH - 1 - x)) & 1; 
            
            for (INT8 dy = 0; dy < font.header.verticalMultiplier; dy++) {
                for (INT8 dx = 0; dx < font.header.horizontalMultiplier; dx++) {
                    UINT32 frameBufferIndex = ((y * font.header.verticalMultiplier + dy) * frameBufferHorizontalSize) + (x * font.header.horizontalMultiplier + dx);
                    frameBufferAddress[frameBufferIndex] = (bit == TRUE) ? 0xffffff : 0x000000;
                }
            }
        }
    }
}

