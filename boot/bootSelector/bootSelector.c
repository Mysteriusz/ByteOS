#include "bootSelector.h"

EFI_STATUS DrawSelector(VOID){
	EFI_STATUS status = 0;
	
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_HANDLE* gopHandle;
	UINTN gopCount = 0;
	
	status = EFI_FindProtocol((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, &gopCount, (VOID**)&gopHandle);
	if (EFI_ERROR(status)) EnterPanic();
	
	status = EFI_HandleProtocol(gopHandle, (EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, (VOID**)&gop);
	if (EFI_ERROR(status)) EnterPanic();
	
	CreateAndDrawElement(BOOT_SELECTOR_RECT, gop, NULLPTR);
	CreateAndDrawElement(BOOT_SELECTOR_BIN_RECT, gop, NULLPTR);

	VIDEO_ELEMENT* base = NULLPTR;
	GetVideoBuffer(&base, NULLPTR, NULLPTR);

	UINT8 v = 2;
	VIDEO_ELEMENT* elem = NULLPTR;
	status = GetElement(&v, sizeof(UINT8), VIDEO_ELEMENT_TYPE_OFFSET, &elem);

	status = HideElement(&base[1], gop);
	status = ShowElement(&base[1], gop);

	COORDS_INFO lPos;

	lPos.x = 300;
	lPos.y = 200;

	MoveElement(elem, gop, &lPos, NULLPTR);

	return 0;
}

NORETURN VOID EnterPanic(VOID) {
	while (1);
}
