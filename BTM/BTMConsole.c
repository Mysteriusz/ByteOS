#include "BTMConsole.h"

EFI_STATUS BTM_StartConsole(IN EFI_SYSTEM_TABLE *sysTable){
    EFI_Print(sysTable, L"STARTING BOOTMANAGER CONSOLE");
    
    BTM_PrintDefaultString(sysTable, L"\r\nBOOTMANAGER>");

    EFI_INPUT_KEY* input = (EFI_INPUT_KEY*)NULL;
    CHAR16 cmd[256];
    UINT32 cmdLen = 0;

    while (TRUE){
        if (BTM_CheckInput(sysTable, input) == EFI_SUCCESS){
            if (input->unicodeChar == EFI_KEY_ENTER){
                BTM_TOKENS tokens;

                BTM_Tokenize(sysTable, cmd, cmdLen, &tokens);
                BTM_Execute(sysTable, &tokens);

                for (UINT i = 0; i < tokens.tokenCount; i++){
                    EFI_DeAllocPool(sysTable, tokens.tokens[i]);
                }

                BTM_PrintDefaultString(sysTable, L"\r\nBOOTMANAGER>");

                cmdLen = 0;
                cmd[0] = '\0';
            }
            else if (input->unicodeChar == EFI_KEY_BACKSPACE){
                if (cmdLen > 0){
                    cmd[cmdLen - 1] = L'\0';
                    cmdLen--;
                    EFI_Print(sysTable, L"\b \b");
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
EFI_STATUS BTM_Tokenize(IN EFI_SYSTEM_TABLE *sysTable, IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens){
    UINT32 i = 0, s = 0, t = 0;
    btmTokens->tokenCount = 0;

    while (i < cmdLen){
        while (i < cmdLen && cmd[i] == EFI_KEY_SPACE) {
            i++;
        }

        s = i;
        UINT32 tokenLen = 0;

        while (i < cmdLen && cmd[i] != EFI_KEY_SPACE) {
            tokenLen++; i++;
        }
        
        if (tokenLen > 0){
            EFI_AllocPool(sysTable, EfiLoaderData, (tokenLen + 1) * sizeof(CHAR16), (VOID**)&btmTokens->tokens[t]);

            for (UINT32 j = 0; j < tokenLen; j++) {
                btmTokens->tokens[t][j] = cmd[s + j];
            }
            btmTokens->tokens[t][tokenLen] = L'\0';  

            btmTokens->tokenCount++;
            t++;
        }
    }

    return EFI_SUCCESS;
}
EFI_STATUS BTM_Execute(IN EFI_SYSTEM_TABLE *sysTable, IN BTM_TOKENS* btmTokens){
    EFI_STATUS execStatus = 0;

    // =============== HELP ===============
    if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"help") == TRUE){
        EFI_Print(sysTable, L"\r\nLIST OF AVAILABLE COMMANDS:");
        EFI_Print(sysTable, L"\r\n-- help");
        EFI_Print(sysTable, L"\r\n-- load 'full path' 'address' ");
        EFI_Print(sysTable, L"\r\n-- run 'address' 'header name ex: (pe32, pe32+, elf)'");
        EFI_Print(sysTable, L"\r\n-- alloc 'address' 'size'");
        EFI_Print(sysTable, L"\r\n-- free 'address' 'size'");
    }
    // =============== LOAD 'FULL PATH' 'ADDRESS' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"load") == TRUE){
        EFI_Print(sysTable, L"\r\nLOADING");
        
        CHAR16* filePath = btmTokens->tokens[1];
        UINT64 address = Char16ToUInt64(btmTokens->tokens[2]);

        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp = NULL;
        
        execStatus = sysTable->bootServices->locateProtocol(&(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, NULL, (VOID**)&sfsp);
        
        if (EFI_ERROR(execStatus)){
            EFI_Print(sysTable, ConcatChar16(L"\r\nERROR OCCURED WHILE FINDING THE PROTOCOL: ", UInt8ToChar16(execStatus)));
        }
        else {
            EFI_FILE_PROTOCOL* root = NULL;
            sfsp->openVolume(sfsp, &root);
            
            EFI_FILE_PROTOCOL* lookup = NULL;
            execStatus = root->open(root, &lookup, filePath, (UINT64*)EFI_FILE_MODE_READ, 0);
            
            if (EFI_ERROR(execStatus)){
                EFI_Print(sysTable, ConcatChar16(L"\r\nFILE NOT FOUND: ", filePath));
                return execStatus;
            }
            
            lookup->read(lookup, (UINTN*)1000000, (VOID*)address);


            EFI_Print(sysTable, ConcatChar16(L"\r\nLOADED AT: ", UInt64ToChar16Hex(address)));
        }
    }
    // =============== RUN 'ADDRESS' 'HEADER NAME' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"run") == TRUE){
        EFI_Print(sysTable, L"\r\nRUNNING");
    } 
    // =============== ALLOC 'ADDRESS' 'SIZE' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"alloc") == TRUE) {
        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        UINT64 size = Char16ToUInt64(btmTokens->tokens[2]);
        
        address &= ~(EFI_PAGE_SIZE - 1);
        UINT64 pages = EFI_SIZE_TO_PAGES(size);
        if (size % EFI_PAGE_SIZE != 0) {
            pages++;
        }

        execStatus = EFI_AllocPages(sysTable, AllocateAddress, EfiLoaderCode, pages, &address);
         
        if (EFI_ERROR(execStatus)) {
            EFI_Print(sysTable, ConcatChar16(L"\r\nERROR OCCURRED: ", UInt8ToChar16(execStatus)));
        } else {
            EFI_Print(sysTable, ConcatChar16(L"\r\nALLOCATED MEMORY AT: ", UInt64ToChar16Hex(address)));
            EFI_Print(sysTable, ConcatChar16(L" WITH SIZE: ", UInt32ToChar16(size)));
        }
    }
    // =============== FREE 'ADDRESS' 'SIZE' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"free") == TRUE){
        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        UINT64 size = Char16ToUInt64(btmTokens->tokens[2]);

        address &= ~(EFI_PAGE_SIZE - 1);
        UINT64 pages = EFI_SIZE_TO_PAGES(size);
        if (size % EFI_PAGE_SIZE != 0) {
            pages++;
        }

        execStatus = EFI_DeAllocPages(sysTable, pages, address);

        if (EFI_ERROR(execStatus)){
            EFI_Print(sysTable, ConcatChar16(L"\r\nERROR OCCURED: ", EFI_GetStatus(execStatus)));
        }
        else{
            EFI_Print(sysTable, ConcatChar16(L"\r\nFREED MEMORY AT: ", UInt64ToChar16Hex(address)));
        }    
    }  
    // =============== RB 'ADDRESS' 'BYTE COUNT' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"rb") == TRUE){
        EFI_Print(sysTable, L"\r\nREADING:\r\n");

        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        UINT32 byteCount = Char16ToUInt32(btmTokens->tokens[2]);

        BYTE* data;

        execStatus = EFI_AllocPool(sysTable, EfiBootServicesData, sizeof(BYTE) * byteCount, (VOID**)&data);
        sysTable->bootServices->copyMem(data, (VOID*)address, sizeof(BYTE) * byteCount);

        for (UINTN i = 0; i < sizeof(BYTE) * byteCount; i++){
            EFI_Print(sysTable, UInt8ToChar16(data[i]));
        }
    }
    return execStatus;
}