#include "disk.h"
#include "pci.h"

UINT32 closestFree = 0;
CHAR8 diskSymbols[IO_MAX_DISKS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
IO_DISK *disks[IO_MAX_DISKS];

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count){
    UINT32 msu = 0;
    for (UINT32 i = 0; i < *count; i++){
        IO_DISK *disk = NULL;
        BT_STATUS status = RegisterDisk(devices[i].pciAddress, NULL, disk);

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
BT_STATUS RegisterDisk(IN VOID *pciAddress, OPTIONAL IN CHAR8 *symbol, OUT IO_DISK *io){
    if (closestFree >= IO_MAX_DISKS){
        return BT_IO_DISK_OVERFLOW;
    }

    if (pciAddress == NULL){
        return BT_INVALID_ARGUMENT;
    }

    PCI *pci = (PCI*)pciAddress;

    if (pci->header.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCIE;
    }

    UINTN s = sizeof(IO_DISK);
    BT_STATUS status = AllocPhysicalPool((VOID**)&io, &s, BT_MEMORY_KERNEL_RW);
    if (BT_ERROR(status)){
        return status;
    }

    UINT32 diskIndex = 0;
    
    if (symbol != NULL){
        while (diskIndex < IO_MAX_DISKS){
            if (diskSymbols[diskIndex] == *symbol){
                if (disks[diskIndex] == NULL){
                    break;
                }
                FreePhysicalPool((VOID*)io, &s);
                return BT_IO_INVALID_SYMBOL;
            }
            diskIndex++;
        }
    }
    else{
        diskIndex = closestFree;
    }

    *disks[diskIndex] = *io;

    (*disks)[diskIndex].symbol = diskSymbols[diskIndex];
    (*disks)[diskIndex].fileSystem = NONE;
    (*disks)[diskIndex].size = 0;
    (*disks)[diskIndex].pciAddress = pciAddress;

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    return BT_SUCCESS;
}
