#include "../EFI/EFITypes.h"

typedef struct BTM_TOKENS {
    CHAR16* tokens;
    UINT32 tokenCount;
} BTM_TOKENS;

EFI_STATUS BTM_StartConsole(IN EFI_SYSTEM_TABLE *sysTable);
EFI_STATUS BTM_PrintDefaultString(IN EFI_SYSTEM_TABLE *sysTable, IN CHAR16* message);
EFI_STATUS BTM_CheckInput(IN EFI_SYSTEM_TABLE *sysTable, OUT EFI_INPUT_KEY *input);
EFI_STATUS BTM_Tokenize(IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens);
EFI_STATUS BTM_Execute(IN BTM_TOKENS* tokens);

EFI_STATUS BTM_StartConsole(IN EFI_SYSTEM_TABLE *sysTable){
    EFI_Print(sysTable, (CHAR16*)L"STARTING BOOTMANAGER CONSOLE");
    
    BTM_PrintDefaultString(sysTable, (CHAR16*)L"\r\nBOOTMANAGER>");

    EFI_INPUT_KEY* input = (EFI_INPUT_KEY*)NULL;
    CHAR16 cmd[256];
    UINT32 cmdLen = 0;

    while (TRUE){
        if (BTM_CheckInput(sysTable, input) == EFI_SUCCESS){
            if (input->unicodeChar == EFI_KEY_ENTER){
                BTM_TOKENS tokens;

                BTM_Tokenize(cmd, cmdLen, &tokens);

                BTM_PrintDefaultString(sysTable, (CHAR16*)L"\r\nBOOTMANAGER>");
                
                cmdLen = 0;
                cmd[0] = '\0';
            }
            else if (input->unicodeChar == EFI_KEY_BACKSPACE){
                if (cmdLen > 0){
                    cmd[cmdLen - 1] = L'\0';
                    cmdLen--;
                    EFI_Print(sysTable, (CHAR16*)L"\b \b");
                }
            }
            else {
                cmd[cmdLen] = input->unicodeChar;     
                cmd[cmdLen + 1] = L'\0';     
                cmdLen++;

                EFI_Print(sysTable, &input->unicodeChar);
            }
        }
    }
}
EFI_STATUS BTM_PrintDefaultString(IN EFI_SYSTEM_TABLE *sysTable, IN CHAR16* message){
    sysTable->conOut->setAttribute(sysTable->conOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLACK));
    EFI_Print(sysTable, message);
    sysTable->conOut->setAttribute(sysTable->conOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));    

    return EFI_SUCCESS;
}
EFI_STATUS BTM_CheckInput(IN EFI_SYSTEM_TABLE *sysTable, OUT EFI_INPUT_KEY *input){
    EFI_STATUS status;
    status = sysTable->conIn->readKeyStroke(sysTable->conIn, input);
    return status;
}
EFI_STATUS BTM_Tokenize(IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens){
    UINT32 i = 0;

    btmTokens->tokenCount = 0;

    while (i < cmdLen){
        while (i < cmdLen && cmd[i] == EFI_KEY_SPACE) {
            i++;
        }

        UINT32 tokenLen = 0;
        while (i < cmdLen && cmd[i] != EFI_KEY_SPACE) {
            i++;
            tokenLen++;
        }
        
        if (tokenLen > 0){
            btmTokens->tokenCount++;
        }
    }

    return EFI_SUCCESS;
}
EFI_STATUS BTM_Execute(IN BTM_TOKENS* tokens){
    return EFI_SUCCESS;
}