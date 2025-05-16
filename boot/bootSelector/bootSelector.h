#pragma once

#include "bootSelectorGraphics.h"

#define BOOT_SELECTOR_RECT \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_RECT, \
	.lPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x200, 0x200, 0x00}, \
	.color = COLOR_RED \
}
#define BOOT_SELECTOR_BIN_RECT \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_RECT, \
	.lPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x100, 0x100, 0x00}, \
	.color = COLOR_WHITE \
}

#define TEST_ELLIPSE \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_ELLIPSE, \
	.lPos = (COORDS_INFO){0x100, 0x100, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x180, 0x100, 0x00}, \
	.color = COLOR_RED \
}
#define TEST_RECT \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_RECT, \
	.lPos = (COORDS_INFO){0x100, 0x100, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x180, 0x100, 0x00}, \
	.color = COLOR_WHITE \
}

EFI_STATUS DrawSelector(VOID);
VOID EnterPanic(VOID);
UINT16* UInt32ToChar16(UINT32 i);
