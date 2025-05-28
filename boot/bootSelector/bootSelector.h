#pragma once

#include "bootSelectorGraphics.h"

#define BOOT_SELECTOR_RECT_ID 0x0001
#define BOOT_SELECTOR_BORDER \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_BORDER | VIDEO_ELEMENT_CHILD, \
	.lPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x05, 0x05, 0x00}, \
	.meta = (VIDEO_ELEMENT_META) \
	{ \
		.id = BOOT_SELECTOR_RECT_ID, \
		.child = NULLPTR, \
		.parent = NULLPTR \
	}, \
	.color = COLOR_GRAY \
}
#define BOOT_SELECTOR_RECT \
(VIDEO_ELEMENT) \
{ \
	.type = VIDEO_ELEMENT_RECT, \
	.lPos = (COORDS_INFO){0x50, 0x50, 0x00}, \
	.rPos = (COORDS_INFO){0x00, 0x00, 0x00}, \
	.size = (COORDS_INFO){0x200, 0x200, 0x00}, \
	.meta = (VIDEO_ELEMENT_META) \
	{ \
		.id = BOOT_SELECTOR_RECT_ID, \
		.child = &BOOT_SELECTOR_BORDER, \
		.parent = NULLPTR \
	}, \
	.color = COLOR_BLACK \
}

EFI_STATUS DrawSelector(VOID);
VOID EnterPanic(VOID);
UINT16* UInt32ToChar16(UINT32 i);
