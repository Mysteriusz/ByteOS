#include "sata.h"
#include "../pci.h"


BT_STATUS SATA_Setup(IN IO_DISK *disk, IN UINT8 pi){
    if (disk == NULL){
        return 23;
    }

    switch (pi)
    {
        case PCI_PI_0x01_0x06_CVS:
            break;
            
        case PCI_PI_0x01_0x06_CAHCI:
            break;
    
        case PCI_PI_0x01_0x06_CBUS:
            break;
    
        default:
            return BT_INVALID_ARGUMENT;
    }

    disk;

    return BT_SUCCESS;
}