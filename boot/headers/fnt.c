#include "fnt.h"

EFI_STATUS FntCharBitmap(IN FONT* font, IN UINT32 character, OUT BITMAP* buffer) {
	if (font == NULLPTR || buffer == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	FNT_HEADER* fnt = (FNT_HEADER*)font->file;

	if (character >= FNT_TABLE_SIZE(fnt)) {
		return EFI_INVALID_PARAMETER;
	}

	buffer->sizeY = fnt->pixHeight;
	buffer->sizeX = fnt->charTables[character].characterWidth;
	buffer->data = (UINT8*)((EFI_PHYSICAL_ADDRESS)fnt + fnt->charTables[character].byteOffset);

	return status;
}
