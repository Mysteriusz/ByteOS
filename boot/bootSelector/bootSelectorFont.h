#pragma once

#include "bootSelectorGraphics.h"

typedef enum CHAR_TYPE {
	CHAR8,
	CHAR16,
	CHAR24,
	CHAR32,
	CHAR64,
} CHAR_TYPE;

typedef enum FONT_TYPE {
	FNT,
} FONT_TYPE;

typedef struct FONT {
	UINT8 charType;
	UINT8 fontType;
	UINT16 charWidth;
	UINT16 charHeight;
	VOID* file;
} FONT;
typedef struct CHAR_LINE {
	COORDS_INFO pos;
	FONT font;
	struct CHAR_LINE* next;
} CHAR_LINE;

EFI_STATUS LoadFont(IN UINT16* path, IN UINT8 type, OUT FONT** buffer);
