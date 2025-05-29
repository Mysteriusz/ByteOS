#include "bootSelectorGraphics.h"

typedef enum CHAR_TYPE {
	CHAR8,
	CHAR16,
	CHAR24,
	CHAR32,
	CHAR64,
} CHAR_TYPE;

typedef struct FONT {
	CHAR_TYPE charType;
	UINT16 height;
	UINT16 width;
	VOID* bitmap;
} FONT;
typedef struct CHAR_LINE {
	COORDS_INFO pos;
	CHAR_LINE* next;
	FONT font;
} CHAR_LINE;

EFI_STATUS Print(IN COORDS_INFO pos, IN FONT font, IN UINT8 text);
