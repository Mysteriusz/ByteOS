#pragma once

#include "efi.h"

typedef struct COORDS_INFO {
	UINT32 x;
	UINT32 y;
	UINT32 z;
} COORDS_INFO;
typedef struct COLOR_INFO {
	UINT8 r;
	UINT8 g;
	UINT8 b;
	UINT8 a;
} COLOR_INFO;

#define VIDEO_ELEMENT_EMPTY		0x00
#define VIDEO_ELEMENT_PIXEL		0x01
#define VIDEO_ELEMENT_RECT		0x02
#define VIDEO_ELEMENT_LINE		0x03
#define VIDEO_ELEMENT_AWAIT  	0xff
typedef struct VIDEO_ELEMENT {
	UINT8 type;
	COORDS_INFO lPos;
	COORDS_INFO rPos;
	COORDS_INFO size;
	COLOR_INFO color;
} VIDEO_ELEMENT;

#define COLOR_WHITE		((COLOR_INFO){0xff,0xff,0xff,0xff})
#define COLOR_BLACK		((COLOR_INFO){0x00,0x00,0x00,0x00})
#define COLOR_RED		((COLOR_INFO){0xff,0x00,0x00,0xff})
#define COLOR_GREEN		((COLOR_INFO){0x00,0xff,0x00,0xff})
#define COLOR_BLUE		((COLOR_INFO){0x00,0x00,0xff,0xff})
#define COLOR_GRAY		((COLOR_INFO){0x80,0x80,0x80,0xff})

#define COLOR_READ(colorInfo)(colorInfo.a << 24 | colorInfo.r << 16 | colorInfo.g << 8 | colorInfo.b)

#define VIDEO_BUFFER_SIZE	    0xffff
#define VIDEO_BUFFER_COUNT	    (VIDEO_BUFFER_SIZE / sizeof(VIDEO_ELEMENT))

EFI_STATUS SetupVideoBuffer(VOID);
EFI_STATUS GetVideoBuffer(
	OUT OPTIONAL EFI_PHYSICAL_ADDRESS* base,
	OUT OPTIONAL EFI_PHYSICAL_ADDRESS* closest
);

EFI_STATUS CreateElement(IN OUT VIDEO_ELEMENT* template);
EFI_STATUS DrawElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS DrawRect(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS DrawLine(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS DrawPixel(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
