#pragma once

#include "byteos.h"
#include "../pci.h"

typedef enum FS_TYPE{
    NONE = 0x00,
    FAT32 = 0x32,
} FS_TYPE;

// ==================================== |
//                 GLOBAL               |
// ==================================== |

BT_STATUS SetupFilesystem(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type);

// ==================================== |
//             SATA_SPECIFIC            |
// ==================================== |

BT_STATUS SetupSataFilesystem(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type);
BT_STATUS SetupSataFilesystemAhci(IN PCI *diskDevice, IN UINT8 partitionIndex, IN FS_TYPE type);