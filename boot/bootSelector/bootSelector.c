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
	
	/*CreateAndDrawElement(BOOT_SELECTOR_RECT, gop, NULLPTR);
	CreateAndDrawElement(BOOT_SELECTOR_BIN_RECT, gop, NULLPTR);

	VIDEO_ELEMENT* base = NULLPTR;
	GetVideoBuffer(&base, NULLPTR, NULLPTR);*/

	VIDEO_ELEMENT* elem = NULLPTR;
	CreateAndDrawElement(TEST_RECT, gop, &elem);
	CreateAndDrawElement(TEST_ELLIPSE, gop, NULLPTR);

	EFI_Print(UInt32ToChar16(elem->type));

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
