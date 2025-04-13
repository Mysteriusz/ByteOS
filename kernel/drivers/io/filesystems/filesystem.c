#include "filesystem.h"

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS ByteAPI SetupFilesystem(IN IO_DISK *disk, IN FS_TYPE type){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCI;
    }

    BT_STATUS status = 0;
    switch (type)
    {
        case FAT32:
            // status = FAT32_Setup(disk);
            if (BT_ERROR(status)) return status;
            break;
        default:
            return BT_INVALID_ARGUMENT;
    } 

    disk->filesystem = type;
    return BT_SUCCESS;
}