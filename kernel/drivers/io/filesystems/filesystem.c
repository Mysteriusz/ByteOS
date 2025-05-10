#include "filesystem.h"
#include "fat32.h"

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI FilesystemSetup(IN IO_DISK_PARTITION *partition, IN FILE_SYSTEM_TYPE type){
    if (partition->disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    BT_STATUS status = 0;
    switch (type)
    {
        case FAT32:
            status = Fat32Setup(partition);
            if (BT_ERROR(status)) return status;
            break;
        default:
            return BT_INVALID_ARGUMENT;
    } 

    return BT_SUCCESS;
}
