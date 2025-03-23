#include "Font.h"

VOID PrintChar(IN FONT *font, IN FONT_CHAR *character, IN UINT32 *frameBufferAddress, IN UINT32 frameBufferHorizontalSize) {
    for (UINT32 y = 0; y < font->pixelHeight; y++) {
        for (UINT32 x = 0; x < font->pixelWidth; x++) {
            BOOLEAN bit = (character->points[y] >> (font->pixelWidth - 1 - x)) & 1; 
            
            for (INT8 dy = 0; dy < font->verticalMultiplier; dy++) {
                for (INT8 dx = 0; dx < font->horizontalMultiplier; dx++) {
                    UINT32 frameBufferIndex = ((y * font->verticalMultiplier + dy) * frameBufferHorizontalSize) + (x * font->horizontalMultiplier + dx);
                    frameBufferAddress[frameBufferIndex] = (bit == TRUE) ? 0xffffff : 0x000000;
                }
            }
        }
    }
}
