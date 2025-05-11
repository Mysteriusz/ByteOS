#include "efi/efi.h"
#include "bootSelector/bootSelector.h"
#include "bootSelector/bootSelectorGraphics.h"

EFI_STATUS EFI_MAIN(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable){
    imageHandle = imgHandle;
    systemTable = sysTable;

    systemTable->conOut->clearScreen(systemTable->conOut);

    EFI_STATUS status = 0;

    status = SetupVideoBuffer();
    
    EFI_PHYSICAL_ADDRESS base;
    GetVideoBuffer(&base, NULLPTR);

    EFI_Print(L"VIDEO BUFFER STATUS: ");
    EFI_Print(UInt32ToChar16(status));
    
    EFI_Print(L"\r\nVIDEO BUFFER AT: ");
    EFI_Print(UInt32ToChar16((UINT32)base));

    DrawSelector();

    while (1);
    return EFI_SUCCESS;
}
