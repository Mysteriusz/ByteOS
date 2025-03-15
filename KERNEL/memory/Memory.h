#include "ByteOS.h"

BT_STATUS ByteAPI InitializeMemory(KERNEL_MEMORY_MAP *memMap);

BT_STATUS ByteAPI AllocPages(IN OUT VOID **buffer, IN OUT UINTN size);