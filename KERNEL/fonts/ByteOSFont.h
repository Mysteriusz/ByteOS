#include "../ByteOS.h"

#define BASE_FONT_WIDTH 8
#define BASE_FONT_HEIGHT 16

typedef struct BTS_FONT_HEADER {
    UINT8 fontMode;
    UINT8 horizontalSize;
    UINT8 verticalSize;
} BTS_FONT_HEADER;
typedef struct BTS_CHAR{
    UINT8 points[16];
} BTS_CHAR;
typedef struct BTS_FONT {
    BTS_FONT_HEADER header;
    BTS_CHAR *characters;
    UINT32 characterCount;
} BTS_FONT;

#define BTS_CAPITAL_A { 0x3c, 0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0x00, 0x00 }

VOID PrintChar(BTS_FONT font, BTS_CHAR *character, UINT32 *frameBufferAddress, UINT32 frameBufferHorizontalSize);
