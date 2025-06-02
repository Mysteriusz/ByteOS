#include "bootSelectorFont.h"
#include "fnt.h"

EFI_STATUS CreateFontBuffer(OUT FONT** buffer){
	if (buffer == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	status = EFI_AllocPool(EfiLoaderData, sizeof(FONT), (VOID**)buffer);
	if (EFI_ERROR(status)) return status;

	(*buffer)->con.load = I_LoadFontFile;
	(*buffer)->con.unload = I_UnloadFontFile;

	return EFI_SUCCESS;
}

EFI_STATUS I_LoadFontFile(IN FONT* font, IN UINT16* path, IN UINT8 type) {
	if (font == NULLPTR || path == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	UINTN count;
	EFI_HANDLE* handle;
	status = EFI_FindProtocol((EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, &count, (VOID**)&handle);
	if (EFI_ERROR(status)) return status;

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;
	status = EFI_HandleProtocol(handle, (EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (VOID**)&fs);
	if (EFI_ERROR(status)) return status;

	EFI_FILE_PROTOCOL* root;
	status = fs->openVolume(fs, &root);
	if (EFI_ERROR(status)) return status;

	EFI_FILE_PROTOCOL* fontFile;
	status = root->open(root, &fontFile, path, EFI_FILE_MODE_READ, 0);
	if (EFI_ERROR(status)) return status;

	EFI_FILE_INFO* fontFileInfo;
	UINTN fontFileInfoSize;

	EFI_GUID fileInfoGuid = EFI_FILE_INFO_GUID;
	fontFile->getInfo(fontFile, &fileInfoGuid, &fontFileInfoSize, NULLPTR);

	status = EFI_AllocPool(EfiLoaderData, fontFileInfoSize, (VOID**)&fontFileInfo);
	if (EFI_ERROR(status)) return status;

	status = fontFile->getInfo(fontFile, &fileInfoGuid, &fontFileInfoSize, fontFileInfo);
	if (EFI_ERROR(status)) return status;

	VOID* data;
	UINTN fontSize = fontFileInfo->fileSize;
	status = EFI_AllocPool(EfiLoaderData, fontSize, (VOID**)&data);
	if (EFI_ERROR(status)) return status;

	status = fontFile->read(fontFile, &fontSize, data);
	if (EFI_ERROR(status)) return status;

	switch (type) {
	case FNT:
		FNT_HEADER* fnt = (FNT_HEADER*)data;
		font->charWidth = fnt->pixWidth;
		font->charHeight = fnt->pixHeight;
		font->con.getChar = FntCharBitmap;
		break;
	default:
		break;
	}

	font->charType = type;
	font->file = data;

	return status;
}
EFI_STATUS I_UnloadFontFile(IN FONT* font) {
	if (font == NULLPTR) return EFI_INVALID_PARAMETER;

	EFI_STATUS status = 0;

	status = EFI_DeAllocPool(font->file);
	if (EFI_ERROR(status)) return status;

	font->con.getChar = NULLPTR;
	font->con.hasChar = NULLPTR;

	return status;
}
