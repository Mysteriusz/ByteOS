#include "linked_list.h"

BT_STATUS ByteAPI LinkedUnsafeCreate(OUT VOID** root, IN UINT32 sizeOfNode) {
	if (sizeOfNode == 0) return BT_INVALID_ARGUMENT;
	if (root == NULL) return BT_INVALID_BUFFER;

	BT_STATUS status = 0;

	status = AllocPhysicalPool(root, (UINTN*)&sizeOfNode, BT_MEMORY_KERNEL_RW);

	return status;
}
BT_STATUS ByteAPI LinkedUnsafeAdd(IN VOID* root, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 sizeOfNode, IN UINT32 valueRva, IN UINT32 nextRva) {
	if (root == NULL) return BT_INVALID_ARGUMENT;
	if (value == NULL) return BT_INVALID_ARGUMENT;

	BT_STATUS status = 0;
	UINTN size = (UINTN)sizeOfNode;

	VOID* prev = NULL;
	VOID* curr = root;

	while (curr != NULL) {
		prev = curr;
		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + nextRva));
	}

	status = AllocPhysicalPool(&curr, &size, BT_MEMORY_KERNEL_RW);

	status = CopyPhysicalMemory(value, (UINTN)sizeOfValue, (VOID*)((PHYSICAL_ADDRESS)curr + valueRva));

	if (prev != NULL) {
		*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)prev + nextRva) = (PHYSICAL_ADDRESS)curr;
	}

	return value;
}
