#pragma once

#include "byteos.h"

#define X86_CACHE_LINE_SIZE 0x40

extern VOID CLFLUSH(VOID *address); // Invalidates and writes all the CPU cache back to memory
extern VOID CLFLUSHOPT(VOID *address); // Invalidates and writes all the CPU cache back to memory without serialization
extern VOID LFENCE(VOID); // Serializes all the memory READ operation called before
extern VOID SFENCE(VOID); // Serializes all the memory WRITE operation called before
extern VOID MFENCE(VOID); // Serializes all the memory operation called before

BT_STATUS X86_FlushLine(IN PHYSICAL_ADDRESS address); // Flushes 64 bytes from CPU cache to memory address
BT_STATUS X86_FlushPage(IN PHYSICAL_ADDRESS address); // Flushes page size from CPU cache to memory address