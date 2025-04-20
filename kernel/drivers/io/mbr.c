#include "mbr.h"

BT_STATUS ByteAPI MbrReadPartitonEntry(IN IO_DISK_PARTITION *partition, IN VOID *buffer){
    if (buffer == NULL) return BT_INVALID_BUFFER;
    return BT_SUCCESS;
}
