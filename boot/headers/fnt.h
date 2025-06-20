#pragma once

#include "efi.h"
#include "../bootSelector/bootSelectorGraphics.h"
#include "../bootSelector/bootSelectorFont.h"

#pragma pack(push, 1)
typedef struct FNT_TABLE {
	UINT16 characterWidth;
	UINT16 byteOffset;
} FNT_TABLE;

typedef struct FNT_HEADER {
	UINT16 version;
	UINT32 size;
	UINT8 copyright[60];
	UINT16 type;
	UINT16 points;
	UINT16 vertRes;
	UINT16 horizRes;
	UINT16 ascent;
	UINT16 internalLeading;
	UINT16 externalLeading;
	BOOLEAN italic;
	BOOLEAN underline;
	BOOLEAN strikeOut;
	UINT16 weight;
	UINT8 charset;
	UINT16 pixWidth;
	UINT16 pixHeight;
	UINT8 pitchAndFamily;
	UINT16 avgWidth;
	UINT16 maxWidth;
	UINT8 firstChar;
	UINT8 lastChar;
	UINT8 defaultChar;
	UINT8 breakChar;
	UINT16 widthBytes;
	UINT32 device;
	UINT32 face;
	UINT32 bitsPointer;
	UINT32 bitsOffset;
	UINT8 reserved;
	FNT_TABLE charTables[];
} FNT_HEADER;
#pragma pack(pop)

#define FNT_TABLE_SIZE(fnt)((fnt)->lastChar - (fnt)->firstChar + 2)

EFI_STATUS FntCharBitmap(IN FONT* font, IN UINT32 character, OUT BITMAP* buffer);
