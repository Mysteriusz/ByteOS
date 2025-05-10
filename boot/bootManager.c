#include "efi/efi.h"
#include "bootSelector.h"
#include "bootSelectorGraphics.h"

EFI_STATUS EFI_MAIN(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *sysTable){
    imageHandle = imgHandle;
    systemTable = sysTable;

    SetupVideoBuffer();
    DrawSelector();

    while (1);
    return EFI_SUCCESS;
}
