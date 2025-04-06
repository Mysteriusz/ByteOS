#include "byteos.h"
#include "graphics/fonts/bts.h"
#include "drivers/io/disk.h"
#include "drivers/io/interfaces/ahci.h"
#include "drivers/pci.h"
#include "drivers/io/types/sata.h"

// ==================================== |
//                KERNEL                |
// ==================================== |

typedef struct testa{
    UINT8 a;
    UINT8 b;
    UINT8 c[PAGE_SIZE];
} testa;
typedef struct testb{
    UINT8 a;
    UINT8 b;
    UINT8 c[PAGE_SIZE];
    UINT8 d[PAGE_SIZE];
} testb;

typedef struct FIRST_PAGE{
    BYTE d1[PAGE_SIZE];
} FIRST_PAGE;

typedef struct testc{
    BYTE d1[0x512];
} testc;

BT_STATUS Kernel_Main(KERNEL_DEVICE_INFO *devInfo, KERNEL_MEMORY_MAP *memMap){

    BT_STATUS status;
    UINT32 *fb = (UINT32*)(devInfo->gpui[0].framebufferAddress);
    // for (UINTN i = 0; i < devInfo.gpui[0].horizontalRes * devInfo.gpui[0].verticalRes; i++) {
    //     fb[i] = 0x000000;
    // }
    
    status = InitializePhysicalPages(memMap);
    status = InitializePhysicalPool();

    FIRST_PAGE *f = NULL;
    UINTN fs = sizeof(FIRST_PAGE);    
    status = AllocPhysicalPages((VOID**)&f, &fs, 0);

    PCI *pci = (PCI*)devInfo->ioi[3].pciAddress;
    SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)pci->header.h0.bar5);
    SATA_PORT_REGISTER *port = NULL;
    SATA_FIND_PORT(hba, &port);

    pci->header.common.command.interruptDisable = FALSE;
    pci->header.common.command.memorySpace = TRUE;
    pci->header.common.command.busMaster = TRUE;

    hba->globalHostControl.interruptEnable = TRUE;

    status = SATA_START_DMA_ENGINE(port);
    
    SATA_IDENTIFY_DEVICE_DATA *idfData = NULL;
    status = SATA_IDENTIFY_DEVICE(port, &idfData);
    SATA_ISSUE_PORT(port->commandIssued, 0);
    
    while (SATA_PORT_FREE(port) == FALSE);

    BYTE *data = NULL;
    status = SATA_READ_DMA_EXT(port, 2, &data);
    SATA_ISSUE_PORT(port->commandIssued, 0);
    
    return (PHYSICAL_ADDRESS)data;    
    status = SATA_STOP_DMA_ENGINE(port);
    
    // return (PHYSICAL_ADDRESS)idfData->logicalPerDrq;    
}

CHAR16* GetKernelLoadStatus(KERNEL_LOAD_STATUS status) {
    switch (status) {
        case KERNEL_LOAD_SUCCESS:
        return L"Kernel loaded successfully.";
        case KERNEL_LOAD_ERROR_FILE:
            return L"Failed to open kernel file.";      
        case KERNEL_LOAD_ERROR_FILE_INFD:
            return L"Failed to read kernel file information.";
        case KERNEL_LOAD_ERROR_ALLOC:
            return L"Memory allocation failed.";
        case KERNEL_LOAD_ERROR_HANDLE_BUFFER:
            return L"Failed to retrieve handle buffer.";
        case KERNEL_LOAD_ERROR_HANDLE_PROTOCOL:
            return L"Failed to handle protocol.";
        case KERNEL_LOAD_ERROR_VOLUME:
            return L"Failed to open file system volume.";
        case KERNEL_LOAD_ERROR_FILE_READ:
            return L"Failed to read from kernel file.";
        default:
            return L"Unknown error.";
    }
}
