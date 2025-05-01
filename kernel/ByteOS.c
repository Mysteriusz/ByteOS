#include "byteos.h"
#include "disk.h"
#include "linked_list.h"

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

    UNSAFE_LINKED_LIST* list = NULL;
    status = LinkedUnsafeCreate(&list);

    list->nextRva = OFFSET_OF(IO_DISK_MAP_REGION, next);
    list->sizeOfNode = sizeof(IO_DISK_MAP_REGION);

    UINT32 v0 = 0x3;
    UINT32 v1 = 0x32;
    UINT32 v2 = 0x64;

    VOID* value = NULL;

    status = LinkedUnsafeAdd(list, &v2, sizeof(UINT32), OFFSET_OF(IO_DISK_MAP_REGION, endCha));
    status = LinkedUnsafeAdd(list, &v1, sizeof(UINT32), OFFSET_OF(IO_DISK_MAP_REGION, endCha));
    status = LinkedUnsafeAdd(list, &v0, sizeof(UINT32), OFFSET_OF(IO_DISK_MAP_REGION, endCha));
    return status;

    //return ((IO_DISK_MAP_REGION*)list->root)->endCha;


    //return ((IO_DISK_MAP_REGION*)((IO_DISK_MAP_REGION*)list->root)->next)->endCha;
    
    //status = LinkedUnsafeRemove((VOID**)&root, &v0, sizeof(UINT32), sizeof(IO_DISK_MAP_REGION), OFFSET_OF(IO_DISK_MAP_REGION, endCha), OFFSET_OF(IO_DISK_MAP_REGION, next));
    
    //status = LinkedUnsafeAdd(root, &v1, sizeof(UINT32), sizeof(IO_DISK_MAP_REGION), OFFSET_OF(IO_DISK_MAP_REGION, endCha), OFFSET_OF(IO_DISK_MAP_REGION, next));*/

    //UINT32 size = 0;
    //status = LinkedUnsafeSize(root, OFFSET_OF(IO_DISK_MAP_REGION, next), &size);

    //return root->next->endCha;


    //status = MapRegions(disk);
    //return disk->info.map.regionCount;
    //return status;
    //return status;

    //return disk->info.map.regionCount;

    // entryArray->typeGuid = (GUID)TEST;
    // return ((MEMORY_PAGE_POOL_HEADER*)0x2000)->blockSize;

    // status = GptIdentifyPartitions(disk);
    
    // GPT_PARTITON_ENTRY *gptPartition = NULL;
    
    // GptReadPartitonEntry(disk, 1, &gptPartition);

    // gptPartition->lastLba = 0x23324;
    // status = GptWritePartitonEntry(disk, 1, gptPartition);


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
