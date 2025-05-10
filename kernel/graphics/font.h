#pragma once

#include "byteos.h"

#define FONT_BASE_POINT_COUNT 8
#define FONT_BASE_WIDTH 8
#define FONT_BASE_HEIGHT 8

typedef struct FONT_CHAR{
    UINT8 points[FONT_BASE_POINT_COUNT];
} FONT_CHAR;
typedef struct FONT{
    UINT8 fontMode;
    UINT8 horizontalMultiplier;
    UINT8 verticalMultiplier;
    UINT32 pixelWidth;
    UINT32 pixelHeight;
    FONT_CHAR *characters;
    UINT32 characterCount;
} FONT;

VOID PrintChar(IN FONT* font, IN FONT_CHAR* character, IN UINT32* frameBufferAddress, IN UINT32 frameBufferHorizontalSize);
