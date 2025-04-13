#pragma once

#include "byteos.h"

#define X86_CACHE_LINE_SIZE 0x40;

extern VOID CLFLUSH(VOID *address);

BT_STATUS X86_FlushLine(IN PHYSICAL_ADDRESS address); // Flushes 64 bytes of memory from address into cache
BT_STATUS X86_FlushPage(IN PHYSICAL_ADDRESS address); // Flushes page size of memory from address into cache