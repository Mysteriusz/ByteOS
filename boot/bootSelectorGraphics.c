#include "bootSelectorGraphics.h"

EFI_STATUS DrawRect(COORDS_INFO* pos, COORDS_INFO* size, COLOR_INFO color, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop){
	UINT32 pixelVerticalOffset = 0;
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	for (UINT32 i = pos->y; i < pos->y + size->y; i++) {
		pixelVerticalOffset = i * gop->mode->info->pixelsPerScanLine;
		
		for (UINT32 j = pos->x; j < pos->x + size->x; j++) {
			framebuffer[pixelVerticalOffset + j] = (UINT32)(color.a << 24 | color.r << 16 | color.g << 8 | color.b);
		}
	}
	
	return EFI_SUCCESS;
}
