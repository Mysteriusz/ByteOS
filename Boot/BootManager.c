#include "../efi/EFITypes.h"
#include "../btm/BTMConsole.h"

static EFI_SYSTEM_TABLE *sysTable;
static EFI_HANDLE *imgHandle;

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable){
    *imgHandle = imageHandle;
    sysTable = systemTable;

    sysTable->conOut->clearScreen(sysTable->conOut);

    BTM_StartConsole(sysTable);
    
    while (TRUE);
    return EFI_SUCCESS;
}