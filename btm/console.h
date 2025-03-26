#ifndef BTM_CONSOLE
#define BTM_CONSOLE

#include "efi/efi.h"

#define BTM_MAX_TOKENS 10

typedef struct BTM_TOKENS {
    CHAR16* tokens[BTM_MAX_TOKENS];
    UINT32 tokenCount;
} BTM_TOKENS;

EFI_STATUS BTM_StartConsole();
EFI_STATUS BTM_PrintDefaultString(IN CHAR16* message);
EFI_STATUS BTM_CheckInput(OUT EFI_INPUT_KEY *input);
EFI_STATUS BTM_Tokenize(IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens);
EFI_STATUS BTM_Execute(IN BTM_TOKENS* btmTokens);

EFI_STATUS GATHER_MEM_MAP(IN BOOLEAN bootServices, OUT KERNEL_MEMORY_MAP **memMap);
EFI_STATUS GATHER_GPU_INFO(OUT UINT32 *gpuCount, OUT KERNEL_GRAPHICAL_DEVICE_INFO **gpuInfo);

#endif  