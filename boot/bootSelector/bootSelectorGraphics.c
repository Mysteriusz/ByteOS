#include "bootSelectorGraphics.h"

VIDEO_ELEMENT* baseVideoBuffer;
VIDEO_ELEMENT* closestVideoBuffer;
UINT32 videoBufferPopulation;

#define VIDEO_OUT_OF_RANGE(address)((EFI_PHYSICAL_ADDRESS)address < (EFI_PHYSICAL_ADDRESS)baseVideoBuffer || (EFI_PHYSICAL_ADDRESS)address > ((EFI_PHYSICAL_ADDRESS)baseVideoBuffer + VIDEO_BUFFER_SIZE))
#define CLOSEST_INDEX (((EFI_PHYSICAL_ADDRESS)closestVideoBuffer - (EFI_PHYSICAL_ADDRESS)baseVideoBuffer) / sizeof(VIDEO_ELEMENT))
#define MAX_INDEX (VIDEO_BUFFER_SIZE / sizeof(VIDEO_ELEMENT))

// ======= LOW LEVEL API =======

EFI_STATUS SetupVideoBuffer(VOID) {
	EFI_STATUS status = 0;
	status = systemTable->bootServices->allocatePool(EfiLoaderData, VIDEO_BUFFER_SIZE, (VOID**)&baseVideoBuffer);
	if (EFI_ERROR(status)) return status;

	closestVideoBuffer = baseVideoBuffer;

	for (UINT32 i = VIDEO_BUFFER_COUNT; i > 0; i--) {
		baseVideoBuffer[i].type = VIDEO_ELEMENT_EMPTY;
	}

	videoBufferPopulation = 0;

	return EFI_SUCCESS;
}
EFI_STATUS GetVideoBuffer(
	OUT OPTIONAL VIDEO_ELEMENT** base,
	OUT OPTIONAL VIDEO_ELEMENT** closest,
	OUT OPTIONAL UINT32** population
){
	if (base != NULLPTR) *base = baseVideoBuffer;
	if (closest != NULLPTR) *closest = closestVideoBuffer;
	if (population != NULLPTR) *population = &videoBufferPopulation;

	return EFI_SUCCESS;
}

EFI_STATUS CreateElement(
	IN VIDEO_ELEMENT template,
	OUT OPTIONAL VIDEO_ELEMENT** buffer
){
	VIDEO_ELEMENT* newElement = (VIDEO_ELEMENT*)closestVideoBuffer;

	memcpy(newElement, &template, sizeof(VIDEO_ELEMENT));
	if (buffer != NULLPTR) *buffer = newElement;
	
	videoBufferPopulation++;

	VIDEO_ELEMENT* child = newElement->meta.child;
	VIDEO_ELEMENT* parent = newElement;
	while (child != NULLPTR) {
		for (UINT32 i = CLOSEST_INDEX; i < MAX_INDEX; i++) {
			if (baseVideoBuffer[i].type == VIDEO_ELEMENT_EMPTY) {
				closestVideoBuffer = &baseVideoBuffer[i];
				break;
			}
		
		}
		
		VIDEO_ELEMENT* next = child->meta.child;

		memcpy(closestVideoBuffer, child, sizeof(VIDEO_ELEMENT));
		closestVideoBuffer->meta.parent = parent;

		parent->meta.child = closestVideoBuffer;
		parent = closestVideoBuffer;
		child = next;

		videoBufferPopulation++;
	}

	return EFI_SUCCESS;
}
EFI_STATUS GetElement(
	IN UINT8* value,
	IN UINT32 valueSize,
	IN UINT32 valueRva,
	OUT VIDEO_ELEMENT** buffer
){
	if (value == NULLPTR || buffer == NULLPTR || valueSize == 0) return EFI_INVALID_PARAMETER;

	for (UINT32 i = 0; i < MAX_INDEX; i++) {
		if (memcmp((UINT8*)OFFSET_AT(&baseVideoBuffer[i], valueRva), value, valueSize) == 0) {
			*buffer = &baseVideoBuffer[i];
			break;
		}
	}

	return EFI_SUCCESS;
}
EFI_STATUS DeleteElement(
	IN OUT VIDEO_ELEMENT** elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || *elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(*elem)) return EFI_INVALID_PARAMETER;

	VIDEO_ELEMENT* child = *elem;
	while (child != NULLPTR) {
		memset(child, 0, sizeof(VIDEO_ELEMENT));
		videoBufferPopulation--;

		if ((EFI_PHYSICAL_ADDRESS)elem < (EFI_PHYSICAL_ADDRESS)closestVideoBuffer) {
			closestVideoBuffer = child;
		}

		child = child->meta.child;
	}

	*elem = NULLPTR;

	return EFI_SUCCESS;
}

EFI_STATUS DrawRect(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR) return EFI_INVALID_PARAMETER;

	UINT32 pixelVerticalOffset = 0;
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	for (UINT32 i = elem->lPos.y; i < elem->lPos.y + elem->size.y && i < gop->mode->info->verticalResolution; i++) {
		pixelVerticalOffset = i * gop->mode->info->pixelsPerScanLine;
		
		for (UINT32 j = elem->lPos.x; j < elem->lPos.x + elem->size.x && j < gop->mode->info->horizontalResolution; j++) {
			framebuffer[pixelVerticalOffset + j] = (UINT32)COLOR_READ(elem->color);
		}
	}

	return EFI_SUCCESS;
}	 

#define PLOT_ELLIPSE(xc, yc, x, y, fb, gop, col) \
do { \
	if ((gop)->mode->info->verticalResolution > (yc) - (y) && (gop)->mode->info->horizontalResolution > (xc) - (x)) { \
		((UINT32*)fb)[((yc) - (y)) * (gop)->mode->info->pixelsPerScanLine + ((xc) - (x))] = COLOR_READ(col); \
	} \
	if ((gop)->mode->info->verticalResolution > (yc) + (y) && (gop)->mode->info->horizontalResolution > (xc) + (x)) { \
		((UINT32*)fb)[((yc) + (y)) * (gop)->mode->info->pixelsPerScanLine + ((xc) + (x))] = COLOR_READ(col); \
	} \
	if ((gop)->mode->info->verticalResolution > (yc) - (y) && (gop)->mode->info->horizontalResolution > (xc) + (x)) { \
		((UINT32*)fb)[((yc) - (y)) * (gop)->mode->info->pixelsPerScanLine + ((xc) + (x))] = COLOR_READ(col); \
	} \
	if ((gop)->mode->info->verticalResolution > (yc) + (y) && (gop)->mode->info->horizontalResolution > (xc) - (x)) { \
		((UINT32*)fb)[((yc) + (y)) * (gop)->mode->info->pixelsPerScanLine + ((xc) - (x))] = COLOR_READ(col); \
	} \
} while(0)
#define FILL_ELLIPSE_R1(xc, yc, x, y, fb, gop, col) \
do{ \
	for (UINT32 iy = (yc) - (y); iy <= (yc) + (y); iy++) { \
		if ((gop)->mode->info->verticalResolution > iy && (gop)->mode->info->horizontalResolution > ((xc) - (x))) { \
			((UINT32*)fb)[iy * (gop)->mode->info->pixelsPerScanLine + ((xc) - (x))] = COLOR_READ(col); \
		} \
	}\
	for (UINT32 iy = (yc) - (y); iy <= (yc) + (y); iy++) { \
		if ((gop)->mode->info->verticalResolution > iy && (gop)->mode->info->horizontalResolution > ((xc) + (x))) { \
			((UINT32*)fb)[iy * (gop)->mode->info->pixelsPerScanLine + ((xc) + (x))] = COLOR_READ(col); \
		} \
	}\
} while(0)
#define FILL_ELLIPSE_R2(xc, yc, x, y, fb, gop, col) \
do{ \
	for (UINT32 ix = (xc) - (x); ix <= (xc) + (x); ix++) { \
		if ((gop)->mode->info->verticalResolution > ((yc) - (y)) && (gop)->mode->info->horizontalResolution > ix) { \
			((UINT32*)fb)[((yc) - (y)) * (gop)->mode->info->pixelsPerScanLine + ix] = COLOR_READ(col); \
		} \
	}\
	for (UINT32 ix = (xc) - (x); ix <= (xc) + (x); ix++) { \
		if ((gop)->mode->info->verticalResolution > ((yc) + (y)) && (gop)->mode->info->horizontalResolution > ix) { \
			((UINT32*)fb)[((yc) + (y)) * (gop)->mode->info->pixelsPerScanLine + ix] = COLOR_READ(col); \
		} \
	}\
} while(0)
EFI_STATUS DrawEllipse(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	INT32 a = elem->size.x / 2;
	INT32 b = elem->size.y / 2;

	INT32 xc = elem->lPos.x + a;
	INT32 yc = elem->lPos.y + b;

	INT32 x = 0;
	INT32 y = b;

	INT32 a2 = pow(a, 2);
	INT32 b2 = pow(b, 2);

	INT32 p = b2 + (a2 * (1 - 4 * b) - 2) / 4;
	
	INT32 p1e = 3 * b2;
	INT32 p2e = 2 * b2;

	INT32 p2s = (2 * a2);

	INT32 p1se = p1e - (2 * a2) * (b - 1);
	INT32 p2se = p2e + (2 * a2);

	PLOT_ELLIPSE(xc, yc, x, y, framebuffer, gop, elem->color);
	FILL_ELLIPSE_R1(xc, yc, x, y, framebuffer, gop, elem->color);

	while (p1se < (2 * a2) + (3 * b2)) {
		if (p < 0) {
			p = p + p1e;
			p1e = p1e + p2e;
			p1se = p1se + p2e;
		}
		else {
			p = p + p1se;
			p1e = p1e + p2e;
			p1se = p1se + p2se;
			y--;
		}
		x++;
		FILL_ELLIPSE_R1(xc, yc, x, y, framebuffer, gop, elem->color);
		PLOT_ELLIPSE(xc, yc, x, y, framebuffer, gop, elem->color);
	}

	p = p - (a2 * (4 * y - 3) + b2 * (4 * x + 3) + 2) / 4;
	p1e = a2 * (3 - 2 * y);
	p1se = (2 * b2) + (3 * a2);

	while (y > 0) {
		if (p > 0) {
			p = p + p1e;
			p1e = p1e + p2s;
			p1se = p1se + p2s;
		}
		else {
			p = p + p1se;
			p1e = p1e + p2s;
			p1se = p1se + p2se;
			x++;
		}
		y--;
		FILL_ELLIPSE_R2(xc, yc, x, y, framebuffer, gop, elem->color);
		PLOT_ELLIPSE(xc, yc, x, y, framebuffer, gop, elem->color);
	}

	return EFI_SUCCESS;
}
EFI_STATUS DrawLine(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR) return EFI_INVALID_PARAMETER;

	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	INT32 dx = elem->rPos.x - elem->lPos.x;
	INT32 dy = -(elem->rPos.y - elem->lPos.y);
	INT32 sx = elem->lPos.x < elem->rPos.x ? 1 : -1;
	INT32 sy = elem->lPos.y < elem->rPos.y ? 1 : -1;
	INT32 err = dx + dy;

	INT32 e2 = 0;

	UINT32 i = 0;

	while (i++ < gop->mode->frameBufferSize) {
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

	return EFI_SUCCESS;
}
EFI_STATUS DrawPixel(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR) return EFI_INVALID_PARAMETER;

	UINT32* framebuffer = (UINT32*)(UINTN)gop->mode->frameBufferBase;

	framebuffer[elem->lPos.y * gop->mode->info->pixelsPerScanLine + elem->lPos.x] = (UINT32)COLOR_READ(elem->color);

	return EFI_SUCCESS;
}
EFI_STATUS DrawBorder(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
) {
	if (elem == NULLPTR || elem->meta.parent == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;

	// TODO: Support for X and Y thickenss
	UINT8 thickness = elem->size.x;

	VIDEO_ELEMENT* parent = elem->meta.parent;
	VIDEO_ELEMENT border = (VIDEO_ELEMENT){ .lPos = (COORDS_INFO){.x = parent->lPos.x - thickness, .y = parent->lPos.y - thickness}, .color = elem->color };

	for (UINT32 i = 0; i < thickness; i++) {
		// LEFT
		border.lPos.x = parent->lPos.x - i;
		border.lPos.y = parent->lPos.y - i;
		border.rPos.x = parent->lPos.x - i;
		border.rPos.y = parent->lPos.y + (parent->size.y - 1) + i;
		DrawLine(&border, gop);
		// BOTTOM 
		border.lPos.x = parent->lPos.x - i;
		border.lPos.y = parent->lPos.y + (parent->size.y - 1) + i;
		border.rPos.x = parent->lPos.x + (parent->size.x - 1) + i;
		border.rPos.y = parent->lPos.y - i;
		DrawLine(&border, gop);
		// RIGHT
		border.lPos.x = parent->lPos.x + (parent->size.x - 1) + i;
		border.lPos.y = parent->lPos.y - i;
		border.rPos.x = parent->lPos.x + (parent->size.x - 1) + i;
		border.rPos.y = parent->lPos.y + (parent->size.y - 1) + i;
		DrawLine(&border, gop);
		// TOP
		border.lPos.x = parent->lPos.x - i;
		border.lPos.y = parent->lPos.y - i;
		border.rPos.x = parent->lPos.x + (parent->size.x - 1) + i;
		border.rPos.y = parent->lPos.y - i;
		DrawLine(&border, gop);
	}

	return EFI_SUCCESS;
}

// ======= HIGH LEVEL API =======

EFI_STATUS RedrawVideoBuffer(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (gop == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	status = gop->blt(gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&COLOR_BLACK, EfiBltVideoFill, 0, 0, 0, 0, gop->mode->info->horizontalResolution, gop->mode->info->verticalResolution, 0);
	if (EFI_ERROR(status)) return status;

	for (UINT32 i = 0; i < MAX_INDEX; i++) {
		if (VIDEO_ELEMENT_DRAWABLE(&baseVideoBuffer[i]) == 0) {
			continue;
		}

		DrawElement(&baseVideoBuffer[i], gop);

		VIDEO_ELEMENT* child = baseVideoBuffer[i].meta.child;
		while (child != NULLPTR) {
			DrawElement(child, gop);
			child = child->meta.child;
		}
	}
	
	return EFI_SUCCESS;
}
EFI_STATUS ClearVideoBuffer(
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (gop == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	status = gop->blt(gop, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)&COLOR_BLACK, EfiBltVideoFill, 0, 0, 0, 0, gop->mode->info->horizontalResolution, gop->mode->info->verticalResolution, 0);
	if (EFI_ERROR(status)) return status;

	memset(baseVideoBuffer, 0, VIDEO_BUFFER_SIZE);

	return EFI_SUCCESS;
}

EFI_STATUS CreateAndDrawElement(
	IN VIDEO_ELEMENT template,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	OUT OPTIONAL VIDEO_ELEMENT** buffer
){
	if (gop == NULLPTR) return EFI_INVALID_PARAMETER;

	VIDEO_ELEMENT temp;
	VIDEO_ELEMENT* tempBase = &temp;

	EFI_STATUS status = 0;
	status = CreateElement(template, &tempBase);
	if (EFI_ERROR(status)) return status;

	status = DrawElement(tempBase, gop);
	if (EFI_ERROR(status)) return status;

	if (buffer != NULLPTR) *buffer = tempBase;

	return EFI_SUCCESS;
}

EFI_STATUS DrawElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	VIDEO_ELEMENT* curr = elem;
	while (curr != NULLPTR) {
		switch (curr->type & 0x1F) {
		case VIDEO_ELEMENT_PIXEL:
			status = DrawPixel(curr, gop);
			break;
		case VIDEO_ELEMENT_RECT:
			status = DrawRect(curr, gop);
			break;
		case VIDEO_ELEMENT_LINE:
			status = DrawLine(curr, gop);
			break;
		case VIDEO_ELEMENT_ELLIPSE:
			status = DrawEllipse(curr, gop);
			break;
		case VIDEO_ELEMENT_BORDER:
			status = DrawBorder(curr, gop);
			break;
		default:
			return EFI_INVALID_PARAMETER;
		}

		curr = curr->meta.child;
	}

	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}
EFI_STATUS EraseElement(
	IN OUT VIDEO_ELEMENT** elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(*elem)) return EFI_INVALID_PARAMETER;

	DeleteElement(elem, gop);
	RedrawVideoBuffer(gop);

	return EFI_SUCCESS;
}

EFI_STATUS MoveElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN OPTIONAL COORDS_INFO* newLpos,
	IN OPTIONAL COORDS_INFO* newRpos
){
	if (elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;
	
	EFI_STATUS status = 0;

	if (newLpos != NULLPTR) memcpy(&elem->lPos, newLpos, sizeof(COORDS_INFO));
	if (newRpos != NULLPTR) memcpy(&elem->rPos, newRpos, sizeof(COORDS_INFO));

	status = RedrawVideoBuffer(gop);
	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}
EFI_STATUS ResizeElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN COORDS_INFO* newSize
){
	if (elem == NULLPTR || gop == NULLPTR || newSize == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	memcpy(&elem->size, newSize, sizeof(COORDS_INFO));

	status = RedrawVideoBuffer(gop);
	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}
EFI_STATUS RepaintElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
	IN COLOR_INFO* newColor
) {
	if (elem == NULLPTR || gop == NULLPTR || newColor == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	memcpy(&elem->color, newColor, sizeof(COLOR_INFO));

	status = RedrawVideoBuffer(gop);
	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}

EFI_STATUS HideElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	elem->type |= VIDEO_ELEMENT_HIDDEN;

	status = RedrawVideoBuffer(gop);
	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}
EFI_STATUS ShowElement(
	IN VIDEO_ELEMENT* elem,
	IN EFI_GRAPHICS_OUTPUT_PROTOCOL* gop
){
	if (elem == NULLPTR || gop == NULLPTR || VIDEO_OUT_OF_RANGE(elem)) return EFI_INVALID_PARAMETER;	
	
	EFI_STATUS status = 0;

	elem->type &= ~VIDEO_ELEMENT_HIDDEN;

	status = RedrawVideoBuffer(gop);
	if (EFI_ERROR(status)) return status;

	return EFI_SUCCESS;
}

EFI_STATUS AddChild(
	IN VIDEO_ELEMENT* parent,
	IN VIDEO_ELEMENT* child
) {
	if (parent == NULLPTR || child == NULLPTR) return EFI_INVALID_PARAMETER;

	VIDEO_ELEMENT* curr = parent->meta.child;
	while (curr != NULLPTR) {
		curr = curr->meta.child;
	}

	curr->meta.child = child;

	child->type |= VIDEO_ELEMENT_CHILD;

	return EFI_SUCCESS;
}
