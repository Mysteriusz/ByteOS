#pragma once

#include "byteos.h"

typedef VOID* PROCESS_HANDLE;

typedef struct PROCESS_GENERAL_INFO{
    UINT32 pid;
    STRING16 name;
} PROCESS_GENERAL_INFO;
typedef struct PROCESS_MEMORY_INFO{
    UINTN physicalUsage;
    // UINTN virtualUsage;
    // UINTN pageFaults;
} PROCESS_MEMORY_INFO;
typedef struct PROCESS_HEADER{
    PROCESS_GENERAL_INFO generalInfo;
    PROCESS_MEMORY_INFO memoryInfo;
} PROCESS_HEADER;

BT_STATUS CreateProcess(IN STRING16 path, OUT PROCESS_HANDLE processHandle);
