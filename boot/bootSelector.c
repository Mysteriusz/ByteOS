#include "bootSelector.h"
#include "bootSelectorGraphics.h"
#include "bootSelectorMath.h"

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
	
	VIDEO_ELEMENT main;
	VIDEO_ELEMENT* mainBase = &main;
	CreateElemenent(mainBase);
	mainBase->lPos.x = 0;
	mainBase->lPos.y = 0;
	mainBase->size.x = 200;
	mainBase->size.y = 200;
	mainBase->color = COLOR_RED;
	DrawRect(mainBase, gop);

	EFI_Print(UInt32ToChar16(mainBase->type));

	return 0;
}

EFI_STATUS EnterPanic(VOID) {
	while (1);
	return 0;
}
