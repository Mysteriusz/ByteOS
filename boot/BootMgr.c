#include "efi.h"
#include "console.h"

EFI_STATUS EFI_MAIN(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable){
    *imageHandle = imgHandle;
    systemTable = sysTable;

    sysTable->conOut->clearScreen(sysTable->conOut);

    BTM_StartConsole(sysTable);
    
    while (TRUE);
    return EFI_SUCCESS;
}