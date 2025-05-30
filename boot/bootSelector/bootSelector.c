#include "bootSelector.h"
#include "fnt.h"

EFI_STATUS DrawSelector(VOID){
	EFI_STATUS status = 0;
	
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_HANDLE* gopHandle;
	UINTN gopCount = 0;
	
	status = EFI_FindProtocol((EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, &gopCount, (VOID**)&gopHandle);
	if (EFI_ERROR(status)) EnterPanic();
	
	status = EFI_HandleProtocol(gopHandle, (EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, (VOID**)&gop);
	if (EFI_ERROR(status)) EnterPanic();
	
	VIDEO_ELEMENT* base = NULLPTR;
	CreateAndDrawElement(BOOT_SELECTOR_RECT, gop, &base);

	FONT* font;
	status = LoadFont(L"\\EFI\\BOOT\\terminusbold.fnt", FNT, &font);
	
	FNT_HEADER* fnt = (FNT_HEADER*)font->file;
	EFI_Print(UInt32ToChar16(font->charWidth));
	//EFI_Print(UInt32ToChar16(fnt->ascent));

	return 0;
}

VOID EnterPanic(VOID) {
	while (1);
}

UINT16* UInt32ToChar16(UINT32 i) {
	static UINT16 buffer[11];

	if (i == 0) {
		buffer[0] = u'0';
		buffer[1] = u'\0';
		return buffer;
	}

	INT32 j = 0;
	while (i > 0) {
		buffer[j++] = (i % 10) + u'0';
		i /= 10;
	}

	buffer[j] = u'\0';

	for (INT32 k = 0, l = j - 1; k < l; k++, l--) {
		UINT16 temp = buffer[k];
		buffer[k] = buffer[l];
		buffer[l] = temp;
	}

	return buffer;
}
UINT16* Char8ToChar16(UINT8* str) {
	static UINT16 buffer[256];
	INT32 i = 0;

	while (str[i] != '\0' && i < 255) {
		buffer[i] = (UINT16)str[i];
		i++;
	}

	buffer[i] = u'\0';
	return buffer;
}
