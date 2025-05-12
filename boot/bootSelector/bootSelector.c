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
	
	CreateAndDrawElement(BOOT_SELECTOR_RECT, gop, NULLPTR);
	CreateAndDrawElement(BOOT_SELECTOR_BIN_RECT, gop, NULLPTR);

	UINT8 v = 2;
	VIDEO_ELEMENT* elem = NULLPTR;
	GetElement(&v, sizeof(UINT8), VIDEO_ELEMENT_TYPE_OFFSET, &elem);

	status = EraseElement(&elem, gop);
	//EFI_Print(UInt32ToChar16((UINT32)elem));
	EFI_Print(UInt32ToChar16(status));

	return 0;
}

EFI_STATUS EnterPanic(VOID) {
	while (1);
	return 0;
}
