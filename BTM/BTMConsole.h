#ifndef BTM_CONSOLE
#define BTM_CONSOLE

#include "../EFI/EFITypes.h"

#define BTM_MAX_TOKENS 10

typedef struct BTM_TOKENS {
    CHAR16* tokens[BTM_MAX_TOKENS];
    UINT32 tokenCount;
} BTM_TOKENS;

EFI_STATUS BTM_StartConsole(IN EFI_SYSTEM_TABLE *sysTable);
EFI_STATUS BTM_PrintDefaultString(IN EFI_SYSTEM_TABLE *sysTable, IN CHAR16* message);
EFI_STATUS BTM_CheckInput(IN EFI_SYSTEM_TABLE *sysTable, OUT EFI_INPUT_KEY *input);
EFI_STATUS BTM_Tokenize(IN EFI_SYSTEM_TABLE *sysTable, IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens);
EFI_STATUS BTM_Execute(IN EFI_SYSTEM_TABLE *sysTable, IN BTM_TOKENS* btmTokens);

#endif  