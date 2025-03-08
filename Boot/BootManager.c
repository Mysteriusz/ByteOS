#include "../EFI/EFITypes.h"
#include "../BTM/BTMConsole.h"

static EFI_SYSTEM_TABLE *sysTable;
static EFI_HANDLE *imgHandle;

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable){
    *imgHandle = imageHandle;
    sysTable = systemTable;

    sysTable->conOut->clearScreen(sysTable->conOut);

    // EFI_STATUS execStatus;

    // EFI_Print(sysTable, L"\r\nLOADING");
        
    // CHAR16* filePath = L"\\EFI\\BOOT\\test.txt";
    // CHAR16* buffer;
    
    // EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp = NULL;
    
    // execStatus = sysTable->bootServices->locateProtocol(&(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, NULL, (VOID**)&sfsp);
    
    // if (EFI_ERROR(execStatus)){
    //     EFI_Print(sysTable, ConcatChar16(L"\r\nERROR OCCURED WHILE FINDING THE PROTOCOL: ", UInt8ToChar16(execStatus)));
    // }
    // else {
    //     EFI_FILE_PROTOCOL* root = NULL;
    //     sfsp->openVolume(sfsp, &root);
        
    //     EFI_FILE_PROTOCOL* lookup = NULL;
    //     execStatus = root->open(root, &lookup, filePath, (UINT64*)EFI_FILE_MODE_READ, 0);
        
    //     if (EFI_ERROR(execStatus)){
    //         EFI_Print(sysTable, ConcatChar16(L"\r\nFILE NOT FOUND: ", filePath));
    //         return execStatus;
    //     }

    //     lookup->read(lookup, (UINTN*)1000, buffer);
    // }

    BTM_StartConsole(sysTable);
    
    while (TRUE);
    return EFI_SUCCESS;
}