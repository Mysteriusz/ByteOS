#include "disk.h"
#include "pcie.h"

UINT32 closestFree = 0;
CHAR8 diskSymbols[IO_MAX_DISKS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
IO_DISK *disks[IO_MAX_DISKS];

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS MapDisks(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count){
    UINT32 msu = 0;
    for (UINT32 i = 0; i < *count; i++){
        IO_DISK *disk = NULL;
        BT_STATUS status = RecognizeDisk(devices[i].pcieAddress, NULL, disk);

        if (status == BT_IO_INVALID_PCIE){
            continue;
        }
        else if (BT_ERROR(status)){
            return status;
        }

        msu++;
    }

    *count = msu;

    return BT_SUCCESS;
}
BT_STATUS RecognizeDisk(IN VOID *pcieAddress, OPTIONAL IN CHAR8 *symbol, OUT IO_DISK *io){
    if (closestFree >= IO_MAX_DISKS){
        return BT_IO_DISK_OVERFLOW;
    }

    if (pcieAddress == NULL){
        return BT_INVALID_ARGUMENT;
    }

    PCIE *pcie = (PCIE*)pcieAddress;

    if (pcie->header.bcc != PCIE_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCIE;
    }

    UINTN s = sizeof(IO_DISK);
    BT_STATUS status = AllocPhysicalPool((VOID**)&io, &s, BT_MEMORY_KERNEL_RWX);
    if (BT_ERROR(status)){
        return status;
    }

    UINT32 diskIndex = 0;
    
    if (symbol != NULL){
        while (diskIndex++ < IO_MAX_DISKS){
            if (diskSymbols[diskIndex] == *symbol && disks[diskIndex] == NULL){
                break;
            }
            
            if (diskSymbols[diskIndex] == *symbol && disks[diskIndex] != NULL){
                return BT_IO_INVALID_SYMBOL;
            }
        }
    }
    else{
        diskIndex = closestFree;
    }

    (*disks)[diskIndex].symbol = diskSymbols[diskIndex];
    (*disks)[diskIndex].fileSystem = NONE;
    (*disks)[diskIndex].size = 0;
    (*disks)[diskIndex].pcieAddress = pcieAddress;

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    return BT_SUCCESS;
}
