#include "byteos.h"
#include "graphics/fonts/bts.h"
#include "memory/memory.h"
#include "drivers/io/disk.h"

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
    status = AllocPhysicalPages((VOID**)&f, &fs, BT_MEMORY_KERNEL_RW);
    
    UINT8 t = devInfo->ioi[0].pcie.msic.mid;
    return (UINT64)&devInfo->ioi[0].pcie;

    // for (UINT32 i = 0; i < devInfo->ioiCount; i++){
    //     IO_DISK disk;
    //     RecognizeDisk(devInfo->ioi[i].pcie.header.cc, NULL, &disk);
    // }
    
    // return devInfo->ioi[0].bar.x32.bs0;
    // return (UINT64)devInfo->ioi[0];

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
    // BYTE *t9 = NULL;
    // UINTN s9 = sizeof(BYTE) * 16;
    // BYTE *t7 = NULL;
    // UINTN s7 = sizeof(BYTE) * 32;
    // BYTE *t8 = NULL;
    // UINTN s8 = sizeof(BYTE) * 32;
    
    // status = AllocPhysicalPool((VOID**)&t6, &s6, 0);
    // for (int i = 0; i < 16; i++) {
    //     t6[i] = i;
    // }       
    
    // status = AllocPhysicalPool((VOID**)&t7, &s7, 0);    
    // t7[0] = 0x01;
    
    // status = AllocPhysicalPool((VOID**)&t8, &s8, 0);
    // t8[0] = 0x0a;

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
    // return (UINT64)DEBUG_CLOSEST();
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
