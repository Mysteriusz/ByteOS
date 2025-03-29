#include "disk.h"
#include "pcie.h"

UINT32 closestFree = 0;
CHAR8 diskCharacters[IO_MAX_DISKS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
IO_DISK disks[IO_MAX_DISKS];

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS RecognizeDisk(IN VOID *pcieAddress, OPTIONAL IN CHAR8 *symbol, OUT IO_DISK *io){
    if (closestFree == IO_MAX_DISKS){
        return BT_IO_DISK_OVERFLOW;
    }

    PCIE *pcie = (PCIE*)pcieAddress;

    if (pcie->header.bcc != PCIE_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCIE;
    }

    disks[closestFree].free = FALSE;
    disks[closestFree].fileSystem = NONE;
    disks[closestFree].size = 0;
    disks[closestFree].symbol = diskCharacters[closestFree];
    disks[closestFree].pcieAddress = pcieAddress;

    while (disks[closestFree++].free == FALSE){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
    }

    return BT_SUCCESS;
}
BT_STATUS SetupFileSystem(IN FS_TYPE type, IN OUT IO_DISK *disk){
    return BT_SUCCESS;
}
