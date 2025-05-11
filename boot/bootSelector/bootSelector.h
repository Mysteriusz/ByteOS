#pragma once

#include "bootSelectorGraphics.h"
#include "bootSelectorMath.h"

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

EFI_STATUS DrawSelector(VOID);
EFI_STATUS EnterPanic(VOID);
