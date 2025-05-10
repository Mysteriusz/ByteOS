#include "byteos.h"
#include "disk.h"
#include "gpt.h"
#include "mbr.h"
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
    
    return devInfo->gpuiCount;
    status = InitializePhysicalPages(memMap);
    status = InitializePhysicalPool();
    FIRST_PAGE *f = NULL;
    UINTN fs = sizeof(FIRST_PAGE);    
    status = AllocPhysicalPages((VOID**)&f, &fs, 0);
    /*

    PCI *pci = (PCI*)devInfo->ioi[3].pciAddress;

    pci->header.common.command.interruptDisable = FALSE;
    pci->header.common.command.memorySpace = TRUE;
    pci->header.common.command.busMaster = TRUE;
    
    IO_DISK *disk = NULL;
    status = InjectDisk(pci, &disk);
    status = MapRegions(disk);

    GPT_PARTITON_ENTRY* entry = NULL;
    UINTN ps = sizeof(GPT_PARTITON_ENTRY);
    AllocPhysicalPool((VOID**)&entry, &ps, BT_MEMORY_KERNEL_RW);

#define uqg { 0x13, 0x4B, 0x8E, 0x18, 0x54, 0xBE, 0xBD, 0x40, 0xB3, 0x1E, 0xA9, 0x75, 0x05, 0x87, 0x6E, 0xB2 }
#define ptg { 0xA2, 0xA0, 0xD0, 0xEB, 0xE5, 0xB9, 0x33, 0x44, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 }
#define pn { \
    0x42, 0x00, 0x61, 0x00, 0x73, 0x00, 0x69, 0x00, \
    0x63, 0x00, 0x20, 0x00, 0x64, 0x00, 0x61, 0x00, \
    0x74, 0x00, 0x61, 0x00, 0x20, 0x00, 0x70, 0x00, \
    0x61, 0x00, 0x72, 0x00, 0x74, 0x00, 0x69, 0x00, \
    0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
}

    BYTE guid1[] = uqg;
    BYTE guid2[] = ptg;
    BYTE name[] = pn;

    CopyPhysicalMemory(guid1, sizeof(GUID), &entry->uniqueGuid);
    CopyPhysicalMemory(guid2, sizeof(GUID), &entry->typeGuid);
    CopyPhysicalMemory(name, 36 * 2, entry->partitionName);
    entry->firstLba = 0xfff80;
    entry->lastLba = 0xfff8a;

    status = GptSafeWritePartitionEntry(disk, 2, entry);

    IO_DISK_REGION* root = disk->info.regionList.root;
    UINT32 s = 0;
    LinkedUnsafeSize(&disk->info.regionList, &s);
    return s;*/

}
