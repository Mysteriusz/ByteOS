#include "efi/efi.h"

typedef struct COORDS_INFO {
	UINT32 x;
	UINT32 y;
	UINT32 z;
} COORDS_INFO;

typedef struct COLOR_INFO {
	UINT8 r;
	UINT8 g;
	UINT8 b;
	UINT8 a;
} COLOR_INFO;

EFI_STATUS DrawRect(COORDS_INFO* pos, COORDS_INFO* size, COLOR_INFO color, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop);
EFI_STATUS DrawLine(VOID);
EFI_STATUS DrawCircle(VOID);
EFI_STATUS FillRect(VOID);
