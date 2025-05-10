#pragma once

#include "byteos.h"

#define X86_CACHE_LINE_SIZE 0x40
extern void CLFLUSH(void* address); // Invalidates and writes all the CPU cache back to memory
extern void CLFLUSHOPT(void* address); // Invalidates and writes all the CPU cache back to memory without serialization
extern void LFENCE(void); // Serializes all the memory READ operation called before
extern void SFENCE(void); // Serializes all the memory WRITE operation called before
extern void MFENCE(void); // Serializes all the memory operation called before
