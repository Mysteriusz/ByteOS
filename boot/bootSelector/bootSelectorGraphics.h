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
typedef struct BITMAP {
	UINT32 sizeX;
	UINT32 sizeY;
	UINT8* data;
} BITMAP;

#define VIDEO_ELEMENT_EMPTY		0x00
#define VIDEO_ELEMENT_PIXEL		0x01
#define VIDEO_ELEMENT_RECT		0x02
#define VIDEO_ELEMENT_LINE		0x03
#define VIDEO_ELEMENT_ELLIPSE	0x04
#define VIDEO_ELEMENT_BORDER	0x0a
#define VIDEO_ELEMENT_CHILD  	(1 << 5) // 6th bit checked
#define VIDEO_ELEMENT_AWAIT  	(1 << 6) // 7th bit checked
#define VIDEO_ELEMENT_HIDDEN	(1 << 7) // 8th bit checked

typedef struct VIDEO_ELEMENT VIDEO_ELEMENT;

typedef struct VIDEO_ELEMENT_META {
	UINT16 id;
	VIDEO_ELEMENT* parent;
	VIDEO_ELEMENT* child;
} VIDEO_ELEMENT_META;
typedef struct VIDEO_ELEMENT {
	UINT8 type;
	COORDS_INFO lPos;
	COORDS_INFO rPos;
	COORDS_INFO size;
	COLOR_INFO color;
	VIDEO_ELEMENT_META meta;
} VIDEO_ELEMENT;

#define VIDEO_ELEMENT_DRAWABLE(ptr)( \
	((ptr)->type != VIDEO_ELEMENT_EMPTY) && \
	!((ptr)->type & VIDEO_ELEMENT_HIDDEN) && \
	!((ptr)->type & VIDEO_ELEMENT_AWAIT) && \
	!((ptr)->type & VIDEO_ELEMENT_CHILD) \
)

#define COLOR_WHITE		((COLOR_INFO){0xff,0xff,0xff,0xff})
#define COLOR_BLACK		((COLOR_INFO){0x00,0x00,0x00,0xff})
#define COLOR_RED		((COLOR_INFO){0xff,0x00,0x00,0xff})
#define COLOR_GREEN		((COLOR_INFO){0x00,0xff,0x00,0xff})
#define COLOR_BLUE		((COLOR_INFO){0x00,0x00,0xff,0xff})
#define COLOR_GRAY		((COLOR_INFO){0x80,0x80,0x80,0xff})

#define COLOR_READ(colorInfo)(colorInfo.a << 24 | colorInfo.r << 16 | colorInfo.g << 8 | colorInfo.b)

#define VIDEO_BUFFER_SIZE	    0x10000
#define VIDEO_BUFFER_COUNT	    (VIDEO_BUFFER_SIZE / sizeof(VIDEO_ELEMENT))

// ======= LOW LEVEL API =======

EFI_STATUS SetupVideoBuffer(VOID);
EFI_STATUS GetVideoBuffer(
	OUT OPTIONAL VIDEO_ELEMENT** base,
	OUT OPTIONAL VIDEO_ELEMENT** closest,
	OUT OPTIONAL UINT32** population
);

EFI_STATUS CreateElement(
	IN VIDEO_ELEMENT template,
	OUT OPTIONAL VIDEO_ELEMENT** buffer
);
EFI_STATUS GetElement(
	IN UINT8* value,
	IN UINT32 valueSize,
	IN UINT32 valueRva,
	OUT VIDEO_ELEMENT** buffer
);
EFI_STATUS DeleteElement(
	IN OUT VIDEO_ELEMENT** elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

/// <summary>
/// Draws a rectangle given elem arguments without type checking.
/// </summary>
/// <param name="elem"><para />
/// as VIDEO_ELEMENT containing:<para />
/// -- lPos - XY Top left position.<para />
/// -- size - XY Width and height.<para />
/// -- color - RGB Drawing color.<para />
/// </param>
EFI_STATUS DrawRect(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

/// <summary>
/// Draws an ellipse given elem arguments without type checking.
/// </summary>
/// <param name="elem"><para />
/// as VIDEO_ELEMENT containing:<para />
/// -- lPos - XY Top left position.<para />
/// -- size - XY Width and height.<para />
/// -- color - RGB Drawing color.<para />
/// </param>
EFI_STATUS DrawEllipse(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

/// <summary>
/// Draws a line given elem arguments without type checking.
/// </summary>
/// <param name="elem"><para />
/// as VIDEO_ELEMENT containing:<para />
/// -- lPos - XY First point position.<para />
/// -- rPos - XY Second point position.<para />
/// -- color - RGB Drawing color.<para />
/// </param>
EFI_STATUS DrawLine(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

/// <summary>
/// Draws a pixel given elem arguments without type checking.
/// </summary>
/// <param name="elem"><para />
/// as VIDEO_ELEMENT containing:<para />
/// -- lPos - XY Exact pixel coordinates.<para />
/// -- color - RGB Drawing color.<para />
/// </param>
EFI_STATUS DrawPixel(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

/// <summary>
/// Draws a border given elem arguments without type checking.
/// </summary>
/// <param name="elem"><para />
/// as VIDEO_ELEMENT containing:<para />
/// -- size - Thickness of the border.<para />
/// -- color - RGB Drawing color.<para />
/// -- elem->meta.parent - VIDEO_ELEMENT that the border is drawn around.<para />
/// </param>
EFI_STATUS DrawBorder(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

// ======= HIGH LEVEL API =======

#define VIDEO_ELEMENT_OFFSET_TYPE OFFSET_OF(VIDEO_ELEMENT, type)
#define VIDEO_ELEMENT_OFFSET_LPOS OFFSET_OF(VIDEO_ELEMENT, lPos)
#define VIDEO_ELEMENT_OFFSET_RPOS OFFSET_OF(VIDEO_ELEMENT, rPos)
#define VIDEO_ELEMENT_OFFSET_COLOR OFFSET_OF(VIDEO_ELEMENT, color)
#define VIDEO_ELEMENT_OFFSET_SIZE OFFSET_OF(VIDEO_ELEMENT, size)
#define VIDEO_ELEMENT_OFFSET_META OFFSET_OF(VIDEO_ELEMENT, meta)

EFI_STATUS RedrawVideoBuffer(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS ClearVideoBuffer(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

EFI_STATUS CreateAndDrawElement(
	IN VIDEO_ELEMENT template,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	OUT OPTIONAL VIDEO_ELEMENT** buffer
);

EFI_STATUS DrawElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS EraseElement(
	IN OUT VIDEO_ELEMENT** elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

EFI_STATUS MoveElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN OPTIONAL COORDS_INFO* newLpos,
	IN OPTIONAL COORDS_INFO* newRpos
);
EFI_STATUS ResizeElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN COORDS_INFO* newSize
);
EFI_STATUS RepaintElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN COLOR_INFO* newColor
);

EFI_STATUS HideElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);
EFI_STATUS ShowElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
);

EFI_STATUS AddChild(
	IN VIDEO_ELEMENT* parent,
	IN VIDEO_ELEMENT* child
);
