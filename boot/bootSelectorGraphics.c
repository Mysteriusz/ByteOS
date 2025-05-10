#include "bootSelectorGraphics.h"

EFI_PHYSICAL_ADDRESS baseVideoBuffer;
EFI_PHYSICAL_ADDRESS closestVideoBuffer;

#define VIDEO_OUT_OF_RANGE(address)((address) < baseVideoBuffer || (address) > (baseVideoBuffer + VIDEO_BUFFER_SIZE))

EFI_STATUS SetupVideoBuffer(VOID) {
	EFI_PHYSICAL_ADDRESS* buffer = &closestVideoBuffer;
	return systemTable->bootServices->allocatePool(EfiLoaderData, VIDEO_BUFFER_SIZE, (VOID**)buffer);
}
EFI_STATUS CreateElemenent(IN OUT VIDEO_ELEMENT* template) {
	EFI_STATUS status = 0;
	status = systemTable->bootServices->copyMem((VOID*)closestVideoBuffer, (VOID*)template, sizeof(VIDEO_ELEMENT));
	if (EFI_ERROR(status)) return status;
	
	VIDEO_ELEMENT* curr = template;
	while (1) {
		if (curr->type == VIDEO_ELEMENT_EMPTY) {
			closestVideoBuffer = (EFI_PHYSICAL_ADDRESS)curr;
			break;
		}
		if (VIDEO_OUT_OF_RANGE((EFI_PHYSICAL_ADDRESS)curr) == 1) break;

		curr++;
	}

	template->type = VIDEO_ELEMENT_AWAIT;
	return EFI_SUCCESS;
}

EFI_STATUS DrawRect(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	UINT32 pixelVerticalOffset = 0;
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	for (UINT32 i = elem->lPos.y; i < elem->lPos.y + elem->size.y; i++) {
		pixelVerticalOffset = i * gop->mode->info->pixelsPerScanLine;
		
		for (UINT32 j = elem->lPos.x; j < elem->lPos.x + elem->size.x; j++) {
			framebuffer[pixelVerticalOffset + j] = (UINT32)COLOR_READ(elem->color);
		}
	}

	elem->type = VIDEO_ELEMENT_RECT;
	return EFI_SUCCESS;
}
EFI_STATUS DrawLine(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	INT32 dx = elem->rPos.x - elem->lPos.x;
	INT32 dy = -(elem->rPos.y - elem->lPos.y);
	INT32 sx = elem->lPos.x < elem->rPos.x ? 1 : -1;
	INT32 sy = elem->lPos.y < elem->rPos.y ? 1 : -1;
	INT32 err = dx + dy;

	INT32 e2 = 0;

	while (1) {
		framebuffer[elem->lPos.y * gop->mode->info->pixelsPerScanLine + elem->lPos.x] = (UINT32)COLOR_READ(elem->color);

		e2 = 2 * err;
		if (e2 >= dy) {
			if (elem->lPos.x == elem->rPos.x) break;
			err += dy;
			elem->lPos.x += sx;
		}
		if (e2 <= dx) {
			if (elem->lPos.y == elem->rPos.y) break;
			err += dx;
			elem->lPos.y += sy;
		}
	}

	elem->type = VIDEO_ELEMENT_LINE;
	return EFI_SUCCESS;
}
EFI_STATUS DrawPixel(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	framebuffer[elem->lPos.y * gop->mode->info->pixelsPerScanLine + elem->lPos.x] = (UINT32)COLOR_READ(elem->color);

	elem->type = VIDEO_ELEMENT_PIXEL;
	return EFI_SUCCESS;
}
