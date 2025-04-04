#include "byteos.h"
#include "graphics/fonts/bts.h"
#include "drivers/io/disk.h"
#include "drivers/io/interfaces/ahci.h"
#include "drivers/pci.h"

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

    // UINT64 ptr = (PHYSICAL_ADDRESS)devInfo->ioi[3].pciAddress + ((PCI*)devInfo->ioi[3].pciAddress)->header.h0.capabilities;
    // return (PHYSICAL_ADDRESS)&pci->header.common.status;
    // return (PHYSICAL_ADDRESS)pci->header.common.status.capabilitiesList;
    PCI *pci = (PCI*)devInfo->ioi[3].pciAddress;
    PCI_HBA_GENERIC_HOST_CONTROL *hba = (PCI_HBA_GENERIC_HOST_CONTROL*)(pci->header.h0.bar5);
    PCI_HBA_PORT_REGISTER *port0 = (PCI_HBA_PORT_REGISTER*)((PHYSICAL_ADDRESS)hba + PCI_HBA_PORT_OFFSET(0));
    // PCI_HBA_PORT_REGISTER *port1 = (PCI_HBA_PORT_REGISTER*)((PHYSICAL_ADDRESS)hba + PCI_HBA_PORT_OFFSET(1));
    // PCI_HBA_PORT_REGISTER *port2 = (PCI_HBA_PORT_REGISTER*)((PHYSICAL_ADDRESS)hba + PCI_HBA_PORT_OFFSET(2));
    
    UINT64 commandListAddress = ((UINT64)port0->commandListBaseAddressUpper << 32) | (port0->commandListBaseAddress << 10);
    PCI_HBA_AHCI_COMMAND_LIST *commandList = (PCI_HBA_AHCI_COMMAND_LIST*)commandListAddress;

    UINT64 commandTableAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    PCI_HBA_AHCI_COMMAND_TABLE *commandTable = (PCI_HBA_AHCI_COMMAND_TABLE*)commandTableAddress;
    
    UINT64 dataBaseAddress = ((UINT64)commandTable->entries[0].dataBaseAddressUpper << 32) | (commandTable->entries[0].dataBaseAddress << 1);

    PCI_HBA_AHCI_COMMAND_SESSION *session;
    UINTN ss = sizeof(PCI_HBA_AHCI_COMMAND_SESSION);
    
    status = AllocPhysicalPool((VOID**)&session, &ss, BT_MEMORY_KERNEL_RW);
    session->port = port0;
    session->portIndex = 0;
    
    session->port->commandIssued |= 1 << session->portIndex;
    status = PCI_HBA_START_DMA_ENGINE(session->port);
    if (BT_ERROR(status)){
        return status;
    }
    
    while (session->port->commandIssued & (1 << session->portIndex));
    
    return session->port->commandIssued;
    
    // return (PHYSICAL_ADDRESS)status;

    // return (PHYSICAL_ADDRESS)port0->rawFisControlAndStatus.transmitterReceivedStatus;
    // return (PHYSICAL_ADDRESS)port0->rawFisControlAndStatus;
    // return (PHYSICAL_ADDRESS)(pci->header.common.latencyTimer);
    // return (PHYSICAL_ADDRESS)sizeof(PCI_HBA_PORT_REGISTER);

    // PHYSICAL_ADDRESS alin = (hba->port0.commandListBaseAddress + 0x3ff) & ~0x3ff;
    // return (PHYSICAL_ADDRESS)hba->port0.commandListBaseAddress;
    // return (PHYSICAL_ADDRESS)hba->ghc.majorVersionNumber;
    // PCI_HBA_PORT_REGISTER r = hba->ghc.
    // offset += cap0->nextCapabilityPointer;    
    // PCI_CAPABILITY *cap1 = (PCI_CAPABILITY*)(offset);
    // offset += cap1->nextCapabilityPointer;
    // PCI_CAPABILITY *cap2 = (PCI_CAPABILITY*)(offset);
    // offset += cap2->nextCapabilityPointer;

    // return ((PCI_PM*)cap0).;
    
    // testa *t1 = NULL;
    // UINTN s1 = sizeof(testa);    
    // testa *t2 = NULL;
    // UINTN s2 = sizeof(testa);
    // testb *t3 = NULL;
    // UINTN s3 = sizeof(testb);
    // FIRST_PAGE *t4 = NULL;
    // UINTN s4 = sizeof(FIRST_PAGE);
    // testb *t5 = NULL;
    // UINTN s5 = sizeof(testb);

    // BYTE *t6 = NULL;
    // UINTN s6 = sizeof(BYTE) * 16;    
    // BYTE *t7 = NULL;
    // UINTN s7 = sizeof(BYTE) * 32;
    // BYTE *t8 = NULL;
    // UINTN s8 = sizeof(BYTE) * 32;

    // testc *t9 = NULL;
    // UINTN s9 = sizeof(testc);
    
    // testc *t10 = NULL;
    // UINTN s10 = sizeof(testc);
    
    // testc *t11 = NULL;
    // UINTN s11 = sizeof(testc);
    
    // testc *t12 = NULL;
    // UINTN s12 = sizeof(testc);
    
    // testc *t13 = NULL;
    // UINTN s13 = sizeof(testc);
    
    // status = AllocPhysicalPool((VOID**)&t6, &s6, 0);
    // status = AllocPhysicalPool((VOID**)&t7, &s7, 0);    
    // status = AllocPhysicalPool((VOID**)&t8, &s8, 0);

    // status = AllocPhysicalPool((VOID**)&t9, &s9, 0);
    // status = AllocPhysicalPool((VOID**)&t10, &s10, 0);
    // status = AllocPhysicalPool((VOID**)&t11, &s11, 0);
    // status = AllocPhysicalPool((VOID**)&t12, &s12, 0);
    // status = FreePhysicalPool((VOID**)&t9, &s9);
    // status = FreePhysicalPool((VOID**)&t10, &s10);
    // status = FreePhysicalPool((VOID**)&t11, &s11);
    // status = AllocPhysicalPool((VOID**)&t13, &s13, 0);
    // status = AllocPhysicalPool((VOID**)&t13, &s13, 0);
    // return (UINT64)t13;
    // return (UINT64)GetPhysicalPool(0, s10).blockSize;
    // return (UINT64)status;
    // return (UINT64)((MEMORY_PAGE_POOL_HEADER*)0x3000)->blockSize;
    // status = AllocPhysicalPool((VOID**)&t11, &s11, 0);

    // UINT32 blockCount = POOL_BLOCK_COUNT(s9);
    // return (UINT64)blockCount;

    // return GetPhysicalPool(0, s9).used;

    // status = AllocPhysicalPages((VOID**)&t1, &s1, BT_MEMORY_WRITE);
    // t1->a = 0x11;
    // t1->b = 0x11;
    // t1->c[0x1000] = 0x11;
    // status = AllocPhysicalPages((VOID**)&t2, &s2, 0);
    // t2->a = 0x22;
    // t2->b = 0x22;
    // t2->c[0x1000] = 0x22;
    // status = AllocPhysicalPages((VOID**)&t3, &s3, 0);
    // t3->a = 0x33;
    // t3->b = 0x33;
    // t3->c[0x1000] = 0x33;
    // t3->d[0x1000] = 0x33;
    // status = AllocPhysicalPages((VOID**)&t5, &s5, 0);
    // t5->a = 0xbb;
    // t5->b = 0xcc;
    // t5->c[0x1000] = 0xdd;
    // t5->d[0x1000] = 0xee;
    // status = AllocPhysicalPool((VOID**)&t9, &s9, 0);
    // for (int i = 0; i < 16; i++) {
    //     t9[i] = i;
    // }    
    // status = FreePhysicalPool(t6, &s6);

    // UINT32 ioCount = devInfo->ioiCount;
    // status = RegisterDisksFromDevices(devInfo->ioi, &ioCount);
    // return (UINT64)status;
    
    // for (UINTN i = 0; i < devInfo->ioiCount; i++){
    //     for (UINT j = 0; j < 3; j++){
    //         if (devInfo->ioi[i].pcie.header.cc[j] != 0){
    //             return i;
    //         }
    //     }
    // }


    // for (UINT32 i = 0; i < devInfo->ioiCount; i++){
    //     IO_DISK disk;
    //     RecognizeDisk(devInfo->ioi[i].pcie.header.cc, NULL, &disk);
    // }
    
    // return devInfo->ioi[0].bar.x32.bs0;
    // return (UINT64)devInfo->ioi[0];


    // status = AllocPhysicalPool((VOID**)&t6, &s6, 0);
    // for (int i = 0; i < 16; i++) {
    //     t6[i] = i;
    // }  

    // UINT32 index = 0;
    // PhysicalPageToIndex(0x802000, &index);

    // FONT bts = BTS_GetFont();
    // PrintChar(&bts, &(FONT_CHAR)BTS_CAPITAL_B, fb, 1280);

    // return (UINT64)GetPhysicalPage(index).allocation;
    // return (UINT64)0;
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
