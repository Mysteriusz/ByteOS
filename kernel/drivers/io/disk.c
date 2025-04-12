#include "disk.h"
#include "sata.h"

UINT32 closestFree = 0;
CHAR8 diskSymbols[IO_MAX_DISKS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
IO_DISK *disks[IO_MAX_DISKS];

// ==================================== |
//                 SETUP                |
// ==================================== |

BT_STATUS ByteAPI RegisterDisksFromDevices(IN KERNEL_IO_DEVICE_INFO *devices, IN OUT UINT32 *count){
    UINT32 msu = 0;
    for (UINT32 i = 0; i < *count; i++){
        IO_DISK *disk = NULL;
        BT_STATUS status = RegisterDisk((PCI*)devices[i].pciAddress, NULL, disk);

        if (status == BT_IO_INVALID_PCI){
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
BT_STATUS ByteAPI RegisterDisk(IN PCI *pci, OPTIONAL IN CHAR8 *symbol, OUT IO_DISK *io){
    if (closestFree >= IO_MAX_DISKS){
        return BT_IO_DISK_OVERFLOW;
    } 

    if (pci == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER){
        return BT_IO_INVALID_PCI;
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
    (*disks)[diskIndex].filesystem = 0;
    (*disks)[diskIndex].size = 0;
    (*disks)[diskIndex].pci = pci;
    (*disks)[diskIndex].pciPartition = 0;
    (*disks)[diskIndex].functions.read = IO_DISK_IF_READ;
    (*disks)[diskIndex].functions.write = IO_DISK_IF_WRITE;

    while (disks[closestFree] != NULL){
        if (closestFree == IO_MAX_DISKS){
            break;
        }
        closestFree++;
    }

    return BT_SUCCESS;
}

BT_STATUS ByteAPI GetDisk(IN CHAR8 symbol, OUT IO_DISK *disk){
    UINT32 i = 0;
    while (i < IO_MAX_DISKS){
        if (++i == IO_MAX_DISKS){
            return BT_INVALID_ARGUMENT;
        }

        if (diskSymbols[i] == symbol){
            break;
        }
    }
    *disk = *disks[i]; 

    return BT_SUCCESS;   
}

BT_STATUS ByteAPI IO_DISK_IF_READ(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, OUT VOID **buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return IO_DISK_IF_SATA(disk, lba, count, buffer, IO_DISK_COMMAND_READ);    
        default:
            return BT_IO_INVALID_PCI;
    }

    return BT_SUCCESS;
}
BT_STATUS ByteAPI IO_DISK_IF_WRITE(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN VOID *buffer){
    if (disk->pci->header.common.bcc != PCI_BCC_MASS_STORAGE_CONTROLLER) return BT_IO_INVALID_PCI;

    switch (disk->pci->header.common.scc)
    {
        case PCI_SCC_0x01_SATA:
            return IO_DISK_IF_SATA(disk, lba, count, &buffer, IO_DISK_COMMAND_WRITE);    
        default:
            return BT_IO_INVALID_PCI;
    }

    return BT_SUCCESS;
}

BT_STATUS ByteAPI IO_DISK_IF_SATA(IN IO_DISK *disk, IN UINT64 lba, IN UINT32 count, IN OUT VOID **buffer, IN UINT16 commandType){
    SATA_GENERIC_HOST_CONTROL *hba = (SATA_GENERIC_HOST_CONTROL*)((UINT64)disk->pci->header.h0.bar5);
    hba->globalHostControl.interruptEnable = TRUE;
    SATA_PORT_REGISTER *port = NULL;
    UINT32 portIndex = 0;

    BT_STATUS status = SATA_FIND_PORT(hba, &port, &portIndex);
    if (BT_ERROR(status)) return status;

    SATA_START_DMA_ENGINE(port);

    switch (commandType)
    {
        // READ
        case 0:
            SATA_READ_DMA_EXT(port, lba, count, buffer);
            break;
        // WRITE
        case 1:        
            SATA_WRITE_DMA_EXT(port, lba, count, *buffer);
            break;
        default:
            break;
    }

    SATA_SAFE_PORT_RUN(port, portIndex);
    
    SATA_STOP_DMA_ENGINE(port);

    return BT_SUCCESS;
}