#include "ByteOS.h"

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap);

BT_STATUS ByteAPI Alloc(IN OUT VOID *buffer, IN OUT UINTN size);
BT_STATUS ByteAPI DeAlloc(IN OUT VOID *buffer, IN OUT UINTN size);