#include "console.h"
#include "pe32.h"

EFI_STATUS BTM_StartConsole(){
    EFI_Print(L"STARTING BOOTMANAGER CONSOLE");
    
    BTM_TOKENS tokens;
    CHAR16* bLoadCmd = L"load kernel\\byteos.bin 232000";
    BTM_Tokenize(bLoadCmd, 29, &tokens);
    BTM_Execute(&tokens);
    CHAR16* bRunCmd = L"run 232000 raw gpui mm";
    BTM_Tokenize(bRunCmd, 23, &tokens);
    BTM_Execute(&tokens);
    
    BTM_PrintDefaultString(L"\r\nBOOTMANAGER>");
    EFI_INPUT_KEY* input = (EFI_INPUT_KEY*)NULL;
    CHAR16 cmd[256];
    UINT32 cmdLen = 0;

    while (TRUE){
        if (BTM_CheckInput(input) == EFI_SUCCESS){
            if (input->unicodeChar == EFI_KEY_ENTER){
                BTM_TOKENS tokens;

                BTM_Tokenize(cmd, cmdLen, &tokens);
                BTM_Execute(&tokens);

                for (UINT i = 0; i < tokens.tokenCount; i++){
                    EFI_DeAllocPool(tokens.tokens[i]);
                }

                BTM_PrintDefaultString(L"\r\nBOOTMANAGER>");

                cmdLen = 0;
                cmd[0] = '\0';
            }
            else if (input->unicodeChar == EFI_KEY_BACKSPACE){
                if (cmdLen > 0){
                    cmd[cmdLen - 1] = L'\0';
                    cmdLen--;
                    EFI_Print(L"\b \b");
                }
            }
            else {
                cmd[cmdLen] = input->unicodeChar;     
                cmd[cmdLen + 1] = L'\0';     
                cmdLen++;

                EFI_Print(&input->unicodeChar);
            }
        }
    }
}
EFI_STATUS BTM_PrintDefaultString(IN CHAR16* message){
    systemTable->conOut->setAttribute(systemTable->conOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLACK));
    EFI_Print(message);
    systemTable->conOut->setAttribute(systemTable->conOut, EFI_TEXT_ATTR(EFI_LIGHTGRAY, EFI_BLACK));    

    return EFI_SUCCESS;
}
EFI_STATUS BTM_CheckInput(OUT EFI_INPUT_KEY *input){
    EFI_STATUS status;
    status = systemTable->conIn->readKeyStroke(systemTable->conIn, input);
    return status;
}
EFI_STATUS BTM_Tokenize(IN CHAR16* cmd, IN UINT32 cmdLen, OUT BTM_TOKENS* btmTokens){
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
            EFI_AllocPool(EfiLoaderData, (tokenLen + 1) * sizeof(CHAR16), (VOID**)&btmTokens->tokens[t]);

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
EFI_STATUS BTM_Execute(IN BTM_TOKENS* btmTokens){
    EFI_STATUS execStatus = 0;

    // =============== HELP ===============
    if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"help") == TRUE){
        EFI_Print(L"\r\nLIST OF AVAILABLE COMMANDS:");
        EFI_Print(L"\r\n-- help");
        EFI_Print(L"\r\n-- load 'full path' 'address' ");
        EFI_Print(L"\r\n-- run 'address' 'offset type ex: (raw, virtual)'");
        EFI_Print(L"\r\n-- alloc 'address' 'size'");
        EFI_Print(L"\r\n-- free 'address' 'size'");
        EFI_Print(L"\r\n-- rb 'address' 'byte count' 'charset ex: (byte, ascii, utf8, utf16)'");
        EFI_Print(L"\r\n-- clear");
    }
    // =============== LOAD 'FULL PATH' 'ADDRESS' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"load") == TRUE){
        CHAR16* filePath = btmTokens->tokens[1];
        UINT64 address = Char16ToUInt64(btmTokens->tokens[2]);
    
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp = NULL;
        EFI_STATUS execStatus;
        EFI_HANDLE* handleBuffer = NULL;
        UINTN handleCount = 0;
    
        execStatus = systemTable->bootServices->locateHandleBuffer(ByProtocol, &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, NULL, &handleCount, &handleBuffer);
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nERROR OCCURRED WHILE FINDING THE PROTOCOL: ", UInt8ToChar16(execStatus)));
            return execStatus;
        }
    
        execStatus = systemTable->bootServices->handleProtocol(handleBuffer[0], &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (VOID**)&sfsp);
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nERROR OCCURRED WHILE OPENING THE FILE SYSTEM: ", UInt8ToChar16(execStatus)));
            return execStatus;
        }
    
        EFI_Print(ConcatChar16(L"\r\nPATH: ", filePath));
    
        EFI_FILE_PROTOCOL* root = NULL;
        execStatus = sfsp->openVolume(sfsp, &root);
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nVOLUME ERROR: ", filePath));
            return execStatus;
        }
    
        EFI_FILE_PROTOCOL* file = NULL;
        execStatus = root->open(root, &file, filePath, (UINT64*)EFI_FILE_MODE_READ, 0);
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nFILE NOT FOUND: ", filePath));
            return execStatus;
        }
    
        UINTN readSize = 50000;
        CHAR16* buffer = (CHAR16*)address;

        execStatus = file->read(file, &readSize, buffer);
    
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nERROR READING FILE: ", filePath));
            return execStatus;
        }
    
        EFI_Print(ConcatChar16(L"\r\nLOADED AT: ", UInt64ToChar16Hex(address)));
        EFI_Print(ConcatChar16(L"\r\nSIZE: ", UInt64ToChar16(readSize)));
    
        file->close(file);
    }   
    // =============== RUN 'ADDRESS' 'OFFSET TYPE' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"run") == TRUE){
        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        CHAR16 *type = btmTokens->tokens[2];

        VOID* baseAdr = (VOID*)address;

        UINT16 magic = GetPeVersion(baseAdr);
        UINT32 entryPointOffset = 0;

        if (CompareString16((STRING16)type, (STRING16)L"raw") == TRUE){
            entryPointOffset = GetRawEntryPointOffset(baseAdr);
        }
        else if (CompareString16((STRING16)type, (STRING16)L"virtual") == TRUE){
            entryPointOffset = GetVirutalEntryPointOffset(baseAdr);
        }
        else{
            return execStatus;
        }

        KERNEL_DEVICE_INFO devInfo;
        KERNEL_MEMORY_MAP *memMap;

        BOOLEAN mm = FALSE;
        BOOLEAN gpui = FALSE;

        for (UINTN i = 0; i < BTM_MAX_TOKENS; i++){
            if (CompareString16((STRING16)btmTokens->tokens[i], (STRING16)L"gpui") == TRUE){
                GATHER_GPU_INFO(&devInfo.gpuiCount, &devInfo.gpui);       
                gpui = TRUE;
            }
            if (CompareString16((STRING16)btmTokens->tokens[i], (STRING16)L"mm") == TRUE){
                GATHER_MEM_MAP(TRUE, &memMap);  
                mm = TRUE;
            }
        }

        typedef UINT64 (*ENTRY_POINT)(KERNEL_DEVICE_INFO *devi, KERNEL_MEMORY_MAP *memm);
        ENTRY_POINT entryPoint = (ENTRY_POINT)((UINT64)(address + entryPointOffset));
        
        EFI_Print(ConcatChar16(L"\r\nMAGIC: ", UInt16ToChar16Hex(magic)));
        EFI_Print(ConcatChar16(L"\r\nENTRY POINT OFFSET: ", UInt16ToChar16Hex(entryPointOffset)));
        EFI_Print(ConcatChar16(L"\r\nENTRY POINT: ", UInt64ToChar16Hex((UINT64)entryPoint)));
        EFI_Print(ConcatChar16(L"\r\nGPUI 0 ADDRESS: ", UInt32ToChar16Hex(devInfo.gpui[0].framebufferAddress)));
        
        if (gpui == TRUE){
            EFI_Print(L"\r\nGPUI: TRUE");
        }
        else{
            EFI_Print(L"\r\nGPUI: FALSE");
        }
        if (mm == TRUE){
            EFI_Print(L"\r\nMM: TRUE");
            systemTable->bootServices->exitBootServices(imageHandle, memMap->mapKey);
        }
        else{
            EFI_Print(L"\r\nMM: FALSE");
        }
        
        EFI_Print(ConcatChar16(L"\r\nRETURN ADDRESS: ", UInt64ToChar16Hex(entryPoint(&devInfo, memMap))));
    } 
    // =============== ALLOC 'ADDRESS' 'SIZE' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"alloc") == TRUE){
        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        UINT64 size = Char16ToUInt64(btmTokens->tokens[2]);
        
        address &= ~(EFI_PAGE_SIZE - 1);
        UINT64 pages = EFI_SIZE_TO_PAGES(size);
        if (size % EFI_PAGE_SIZE != 0) {
            pages++;
        }

        execStatus = EFI_AllocPages(AllocateAddress, EfiLoaderCode, pages, &address);
         
        if (EFI_ERROR(execStatus)) {
            EFI_Print(ConcatChar16(L"\r\nERROR OCCURRED: ", UInt8ToChar16(execStatus)));
        } else {
            EFI_Print(ConcatChar16(L"\r\nALLOCATED MEMORY AT: ", UInt64ToChar16Hex(address)));
            EFI_Print(ConcatChar16(L" WITH SIZE: ", UInt32ToChar16(size)));
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

        execStatus = EFI_DeAllocPages(pages, address);

        if (EFI_ERROR(execStatus)){
            EFI_Print(ConcatChar16(L"\r\nERROR OCCURED: ", EFI_GetStatus(execStatus)));
        }
        else{
            EFI_Print(ConcatChar16(L"\r\nFREED MEMORY AT: ", UInt64ToChar16Hex(address)));
        }    
    }  
    // =============== RB 'ADDRESS' 'BYTE COUNT' 'CHARSET' ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"rb") == TRUE){
        UINT64 address = Char16ToUInt64(btmTokens->tokens[1]);
        UINT64 readCount = Char16ToUInt64(btmTokens->tokens[2]);

        VOID *addressBuffer = (VOID*)address;
        
        BYTE *byteBuffer = (BYTE*)addressBuffer;
        CHAR8 *char8Buffer = (CHAR8*)addressBuffer;
        CHAR16 *char16Buffer = (CHAR16*)addressBuffer;
        CHAR32 *char32Buffer = (CHAR32*)addressBuffer;
        
        EFI_Print(L"\r\n");
        if (CompareString16((STRING16)btmTokens->tokens[3], (STRING16)L"byte") == TRUE){
            for (UINTN i = 0; i < readCount; i++){
                EFI_Print(UInt8ToChar16Hex(byteBuffer[i]));
            }
        }
        else if (CompareString16((STRING16)btmTokens->tokens[3], (STRING16)L"ascii") == TRUE){
            for (UINTN i = 0; i < readCount; i++){
                CHAR16 printable[2] = { (CHAR16)char8Buffer[i], L'\0' };
                EFI_Print(printable);
            }
        }
        else if (CompareString16((STRING16)btmTokens->tokens[3], (STRING16)L"utf8") == TRUE){
            for (UINTN i = 0; i < readCount; i++){
                CHAR16 printable[2] = { (CHAR16)char8Buffer[i], L'\0' };
                EFI_Print(printable);
            }
        }
        else if (CompareString16((STRING16)btmTokens->tokens[3], (STRING16)L"utf16") == TRUE){
            for (UINTN i = 0; i < readCount; i++){
                EFI_Print(&char16Buffer[i]);
            }
        }
    }
    // =============== CLEAR ===============
    else if (CompareString16((STRING16)btmTokens->tokens[0], (STRING16)L"clear") == TRUE){
        execStatus = systemTable->conOut->clearScreen(systemTable->conOut);
    }
    
    return execStatus;
}

EFI_STATUS GATHER_MEM_MAP(IN BOOLEAN bootServices, OUT KERNEL_MEMORY_MAP **memMap){
    UINTN size = 0;
    EFI_MEMORY_DESCRIPTOR *memDesc = NULL;
    UINTN mapKey = 0;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;

    systemTable->bootServices->getMemoryMap(&size, NULL, &mapKey, &descriptorSize, &descriptorVersion);
    
    size += 2 * descriptorSize;
    EFI_AllocPool(EfiLoaderData, size, (VOID**)&memDesc);
    
    systemTable->bootServices->getMemoryMap(&size, memDesc, &mapKey, &descriptorSize, &descriptorVersion);
    
    UINT8 *start = (UINT8*)memDesc;
    UINT8 *end = start + size;
    UINT8 *offset = start;
    UINT64 usable = 0;
    
    (*memMap)->entryCount = 0;

    while (offset < end){
        (*memMap)->entryCount++;
        offset += descriptorSize;
    }

    EFI_AllocPool(EfiLoaderData, (*memMap)->entryCount, (VOID**)&(*memMap)->entries);

    offset = (UINT8*)memDesc;
    
    UINTN x = 0;

    for (UINTN i = 0; i < (*memMap)->entryCount; i++){
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR*)offset;
        
        if (desc->type == 7 || (bootServices == TRUE && (desc->type == 3 || desc->type == 4))){
            if (x < 5){
                EFI_Print(ConcatChar16(L"\r\nMemory Descriptor Start Address: ", UInt64ToChar16Hex(desc->physicalStart)));
                EFI_Print(ConcatChar16(L"\r\nMemory Descriptor Size: ", UInt64ToChar16Hex(desc->numberOfPages * EFI_PAGE_SIZE)));
                x++;
            }

            usable += desc->numberOfPages * EFI_PAGE_SIZE;
        }
        
        (*memMap)->entries[i].size = desc->numberOfPages * EFI_PAGE_SIZE;
        (*memMap)->entries[i].type = desc->type;
        (*memMap)->entries[i].physicalStart = desc->physicalStart;
        (*memMap)->entries[i].virtualStart = desc->virtualStart;
        (*memMap)->entries[i].attribute = desc->attribute;

        offset += descriptorSize;
    }

    EFI_Print(ConcatChar16(L"\r\nAvailable conventional memory (MB): ", UInt64ToChar16(usable / (1024 * 1024))));  
    (*memMap)->descriptorSize = descriptorSize;
    (*memMap)->descriptorVersion = descriptorVersion;
    (*memMap)->mapKey = mapKey;
    (*memMap)->size = size;
    (*memMap)->usableSize = usable;
    
    EFI_DeAllocPool(memDesc);

    return EFI_SUCCESS;
}
EFI_STATUS GATHER_GPU_INFO(OUT UINT32 *gpuCount, OUT KERNEL_GRAPHICAL_DEVICE_INFO **gpuInfo){
    EFI_HANDLE *handleBuffer;
    UINTN handleCount;

    systemTable->bootServices->locateHandleBuffer(ByProtocol, &(EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, NULL, &handleCount, &handleBuffer);

    EFI_AllocPool(EfiLoaderData, sizeof(KERNEL_GRAPHICAL_DEVICE_INFO) *handleCount, (VOID**)gpuInfo);

    *gpuCount = handleCount;

    for (UINTN i = 0; i < handleCount; i++){
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
        systemTable->bootServices->handleProtocol(handleBuffer[i], &(EFI_GUID)EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, (VOID**)&gop);

        (*gpuInfo)[i].framebufferAddress = gop->mode->frameBufferBase;
        (*gpuInfo)[i].frameBufferSize = gop->mode->frameBufferSize;
        (*gpuInfo)[i].horizontalRes = gop->mode->info->horizontalResolution;
        (*gpuInfo)[i].verticalRes = gop->mode->info->verticalResolution;
    }

    return EFI_SUCCESS;
}