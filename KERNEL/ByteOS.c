#include "ByteOS.h"

void Kernel_Main(void){
    UINT32 *fb = (UINT32 *)(0xc0000000);

    for (UINTN i = 0; i < 1280 * 800; i++) {
        fb[i] = 0xFFFFFF;
    }
    
    while (TRUE);
}