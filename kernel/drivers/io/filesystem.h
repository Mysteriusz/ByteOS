#pragma once

#include "fat32.h"

typedef enum FS_TYPE{
    FAT32,
} FS_TYPE;

BT_STATUS FS_Setup(){
    return 0;
}