#include "bootSelector.h"

EFI_STATUS DrawSelector(VOID){
	EFI_STATUS status = 0;
	
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_HANDLE* gopHandle;
	UINTN gopCount = 0;
	
	status = EFI_FindProtocol((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, &gopCount, (VOID**)&gopHandle);
	if (EFI_ERROR(status)) return EnterPanic();
	
	status = EFI_HandleProtocol(gopHandle, (EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, (VOID**)&gop);
	if (EFI_ERROR(status)) return EnterPanic();
	
	VIDEO_ELEMENT main = BOOT_SELECTOR_RECT;
	VIDEO_ELEMENT* mainBase = &main;
	status = CreateElement(mainBase);
	status = DrawRect(mainBase, gop);

	VIDEO_ELEMENT next = BOOT_SELECTOR_BIN_RECT;
	VIDEO_ELEMENT* nextBase = &next;
	status = CreateElement(nextBase);
	status = DrawRect(nextBase, gop);

	return 0;
}

EFI_STATUS EnterPanic(VOID) {
	while (1);
	return 0;
}
