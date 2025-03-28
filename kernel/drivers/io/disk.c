#include "disk.h"

UINT32 closestFree = 0;
CHAR8 diskCharacters[IO_MAX_DISKS] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
IO_DISK disks[IO_MAX_DISKS];

BT_STATUS RecognizeDisk(IN UINT32 bus[IO_MAX_BUS], OPTIONAL IN CHAR8 *symbol, OUT IO_DISK *io){
    if (closestFree == IO_MAX_DISKS){
        return BT_IO_DISK_OVERFLOW;
    }

    UINT32 diskIndex = closestFree;

    for (UINT32 i = 0; i < IO_MAX_BUS; i++){
        disks[diskIndex].bus[i] = bus[i];
    }

    if (symbol == NULL){
        disks[diskIndex].symbol = diskCharacters[closestFree];
    }
    else{
        UINT32 si = 0;
        while (si < IO_MAX_DISKS){
            if (disks[si].initialized == FALSE){
                disks[diskIndex].symbol = *symbol;
                break;
            }
        }

        return BT_IO_DISK_OVERFLOW;
    }

    disks[diskIndex].initialized = TRUE;
    disks[diskIndex].fileSystem = NONE;
    *io = disks[diskIndex];

    while (disks[closestFree++].initialized == TRUE){
        if (closestFree == IO_MAX_DISKS){
            return BT_IO_DISK_OVERFLOW;
        }
    }

    return BT_SUCCESS;
}