#pragma once

#include "bootSelectorGraphics.h"

typedef struct FONT FONT;

typedef enum CHAR_TYPE {
	CHAR8,
	CHAR16,
	CHAR24,
	CHAR32,
	CHAR64,
} CHAR_TYPE;

typedef enum FONT_TYPE {
	FNT,
	FON, // TODO: Other font extension`s compatibility
} FONT_TYPE;

typedef EFI_STATUS(*FONT_GET_CHAR)(
	IN FONT* font,
	IN UINT32 character,
	OUT BITMAP* buffer
);
typedef EFI_STATUS(*FONT_HAS_CHAR)(
	IN FONT* font,
	IN UINT32 character
);
typedef EFI_STATUS(*FONT_LOAD)(
	IN FONT* font,
	IN UINT16* path,
	IN UINT8 type
);
typedef EFI_STATUS(*FONT_UNLOAD)(
	IN FONT* font
);

typedef struct FONT_CONTROL {
	FONT_GET_CHAR getChar;
	FONT_HAS_CHAR hasChar;
	FONT_UNLOAD unload;
	FONT_LOAD load;
} FONT_CONTROL;
typedef struct FONT {
	UINT8 charType;
	UINT8 fontType;
	UINT16 charWidth;
	UINT16 charHeight;
	VOID* file;
	FONT_CONTROL con;
} FONT;
typedef struct CHAR_LINE {
	COORDS_INFO pos;
	FONT font;
	struct CHAR_LINE* next;
} CHAR_LINE;

EFI_STATUS CreateFontBuffer(OUT FONT** buffer);

EFI_STATUS I_LoadFontFile(IN FONT* font, IN UINT16* path, IN UINT8 type);
EFI_STATUS I_UnloadFontFile(IN FONT* font);
