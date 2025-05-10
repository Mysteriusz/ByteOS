#include "bootSelector.h"
#include "bootSelectorGraphics.h"

EFI_STATUS DrawSelector(VOID){
	EFI_STATUS status = 0;
	
	systemTable->conOut->clearScreen(systemTable->conOut);

	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_HANDLE* gopHandle;
	UINTN gopCount = 0;
	
	status = EFI_FindProtocol((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, &gopCount, (VOID**)&gopHandle);
	if (EFI_ERROR(status)) return EnterPanic();
	
	status = EFI_HandleProtocol(gopHandle, (EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, (VOID**)&gop);
	if (EFI_ERROR(status)) return EnterPanic();

	COORDS_INFO pos;
	pos.x = 10;
	pos.y = 11;
	COORDS_INFO size;
	size.x = 10;
	size.y = 5;
	COLOR_INFO color;
	color.r = 0xff;
	color.g = 0xff;
	color.b = 0xff;
	color.a = 0xff;

	DrawRect(&pos, &size, color, gop);

	return 0;
}

EFI_STATUS EnterPanic(VOID) {
	while (1);
	return 0;
}
