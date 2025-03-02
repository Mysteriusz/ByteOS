#include "../EFI/EFITypes.h"

static EFI_SYSTEM_TABLE *sysTable;

KERNEL_LOAD_STATUS EFIAPI LoadKernel(CHAR16 *kernelPath, KERNEL_ENTRY_POINT* kernelEntry);

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imgHandle, EFI_SYSTEM_TABLE *systemTable){
    (VOID)imgHandle;
    
    sysTable = systemTable;

    KERNEL_ENTRY_POINT entry = NULL;
    KERNEL_LOAD_STATUS kernelLoadStatus = LoadKernel(KERNEL_BASE_PATH, &entry);

    EFI_Print(sysTable->conOut, ConcatChar16(L"KERNEL LOAD: ", GetKernelLoadStatus(kernelLoadStatus)));

    if (entry){
        entry();
    }

    while (1) {};
    return 0;
}

KERNEL_LOAD_STATUS EFIAPI LoadKernel(CHAR16 *kernelPath, KERNEL_ENTRY_POINT* kernelEntry){
    EFI_STATUS status = sysTable->conOut->clearScreen(sysTable->conOut);
    
    EFI_HANDLE* fsHandle;
    UINTN handleCount = 0;
    status = sysTable->bootServices->locateHandleBuffer(ByProtocol, &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, NULL, &handleCount, &fsHandle);
    
    if (EFI_ERROR(status)){
        return KERNEL_LOAD_ERROR_HANDLE_BUFFER;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfsp;
    status = sysTable->bootServices->handleProtocol(fsHandle[0], &(EFI_GUID)EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (VOID**)&sfsp);
    
    if (EFI_ERROR(status)){
        return KERNEL_LOAD_ERROR_HANDLE_PROTOCOL;
    }

    EFI_FILE_PROTOCOL* root;
    status = sfsp->openVolume(sfsp, &root);
    
    if (EFI_ERROR(status)){
        return KERNEL_LOAD_ERROR_VOLUME;
    }

    EFI_FILE_PROTOCOL* kernelFile;
    status = root->open(root, &kernelFile, kernelPath, (UINT64*)EFI_FILE_MODE_READ, 0);
    
    if (EFI_ERROR(status)){
        return KERNEL_LOAD_ERROR_FILE;
    }

    UINTN bufferSize = KERNEL_BASE_SIZE;
    BYTE* buffer = NULL;

    status = EFI_Alloc(sysTable->bootServices->allocatePool, EfiBootServicesData, bufferSize, (VOID**)&buffer);

    if (EFI_ERROR(status)){
        return KERNEL_LOAD_ERROR_ALLOC;
    }

    UINT64 address = (UINT64)buffer;
    *kernelEntry = (KERNEL_ENTRY_POINT)address;

    status = EFI_DeAlloc(sysTable->bootServices->freePool, buffer);

    return status;
}