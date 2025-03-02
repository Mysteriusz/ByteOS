#include "../EFI/EFITypes.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable){
    (VOID)imgHandle;

    EFI_STATUS status = sysTable->conOut->clearScreen(sysTable->conOut);
    
    EFI_Print(sysTable->conOut, Concat16(L"sysTable->conOut->clearScreen: ", EFI_GetStatus(status)));
    
    EFI_HANDLE* fsHandle;
    UINTN handleCount = 0;
    status = sysTable->bootServices->locateHandleBuffer(ByProtocol, &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, NULL, &handleCount, &fsHandle);
    
    EFI_Print(sysTable->conOut, Concat16(L"\r\nsysTable->bootServices->locateHandleBuffer: ", EFI_GetStatus(status)));

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp;
    status = sysTable->bootServices->handleProtocol(fsHandle[0], &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (VOID**)&sfsp);
    
    EFI_Print(sysTable->conOut, Concat16(L"\r\nsysTable->bootServices->handleProtocol: ", EFI_GetStatus(status)));
    
    EFI_FILE_PROTOCOL* root;
    status = sfsp->openVolume(sfsp, &root);
    
    EFI_Print(sysTable->conOut, Concat16(L"\r\nsfsp->openVolume: ", EFI_GetStatus(status)));
    
    EFI_FILE_PROTOCOL* testFile;
    status = root->open(root, &testFile, L"\\EFI\\BOOT\\test.txt", (UINT64*)EFI_FILE_MODE_READ, 0);

    EFI_Print(sysTable->conOut, Concat16(L"\r\nroot->open: ", EFI_GetStatus(status)));

    UINTN bufferSize = 1024;
    CHAR16* buffer = NULL;

    EFI_Alloc(sysTable->bootServices->allocatePool, EfiBootServicesData, bufferSize, (VOID**)&buffer);
    
    status = testFile->read(testFile, &bufferSize, buffer);
    EFI_Print(sysTable->conOut, Concat16(L"\r\ntestFile->read: ", EFI_GetStatus(status)));
    EFI_Print(sysTable->conOut, Concat16(L"\r\nDATA: ", buffer));
    
    status = EFI_DeAlloc(sysTable->bootServices->freePool, buffer);

    EFI_Print(sysTable->conOut, Concat16(L"\r\nEFI_DeAlloc: ", EFI_GetStatus(status)));

    while (1) {};
    return 0;
}