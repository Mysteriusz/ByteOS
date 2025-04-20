#include "byteos.h"
#include "filesystem.h"
#include "fat32.h"
#include "crc.h"

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

    pci->header.common.command.interruptDisable = FALSE;
    pci->header.common.command.memorySpace = TRUE;
    pci->header.common.command.busMaster = TRUE;
    
    IO_DISK *disk = NULL;
    status = InjectDisk(pci, &disk);
    
    status = GptIdentifyPartitions(disk);
    
    GPT_PARTITON_ENTRY *gptPartition = NULL;
    UINTN s = sizeof(GPT_PARTITON_ENTRY);
    AllocPhysicalPool((VOID**)&gptPartition, &s, BT_MEMORY_KERNEL_RW);
    
    BYTE *arr = NULL;
    UINTN ss = 2;
    status = AllocPhysicalPool((VOID**)&arr, &ss, BT_MEMORY_KERNEL_RW);
    
    arr[0] = 0x01;
    arr[1] = 0x02;
    
    UINT32 crc32 = 0;
    Crc32(arr, 2, &crc32);
    return crc32;

    // #define TEST \
    // { 0x188E4B13, 0xBE54, 0x40BD, { 0xB3, 0x1E, 0xA9, 0x75, 0x05, 0x87, 0x6E, 0xB2 } }
        
    // #define TEST2 \
    // { 0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 } }

    // gptPartition->typeGuid = (GUID)TEST2;
    // gptPartition->uniqueGuid = (GUID)TEST;
    // gptPartition->firstLba = 323223;
    // gptPartition->lastLba = 0xfffffff;
    // gptPartition->attributeFlags = 0;
    // CopyPhysicalMemory((VOID*)L"TEST", 16, gptPartition->partitionName);
    
    // status = GptWritePartitonEntry(disk, 2, gptPartition);
    
    // GPT_PARTITON_ENTRY *gptPartition1 = NULL;
    // status = GptReadPartitonEntry(disk, 2, &gptPartition1);

    // return gptPartition1->lastLba;
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
