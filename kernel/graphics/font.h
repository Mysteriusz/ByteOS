#ifndef BYTE_OS_FONT
#define BYTE_OS_FONT

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

VOID PrintChar(FONT *font, FONT_CHAR *character, UINT32 *frameBufferAddress, UINT32 frameBufferHorizontalSize);

#endif