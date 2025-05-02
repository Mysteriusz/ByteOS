#include "linked_list.h"

#define LINKED_UNSAFE_VALUE_PTR(curr, rva)((VOID*)((PHYSICAL_ADDRESS)curr + (PHYSICAL_ADDRESS)rva))

BT_STATUS ByteAPI LinkedUnsafeCreate(OUT UNSAFE_LINKED_LIST** list) {
	BT_STATUS status = 0;

	UINTN size = sizeof(UNSAFE_LINKED_LIST);
	status = AllocPhysicalPool((VOID**)list, &size, BT_MEMORY_KERNEL_RW);

	return status;
}
BT_STATUS ByteAPI LinkedUnsafeDelete(IN UNSAFE_LINKED_LIST** list) {
	if (list == NULL) return BT_INVALID_ARGUMENT;

	BT_STATUS status = 0;

	UINTN size = sizeof(UNSAFE_LINKED_LIST);

	status = LinkedUnsafeClear(*list);
	status = FreePhysicalPool((VOID**)list, &size);

	return status;
}

BT_STATUS ByteAPI LinkedUnsafeAdd(IN UNSAFE_LINKED_LIST* list, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 valueRva) {
	if (value == NULL) return BT_INVALID_ARGUMENT;
	if (sizeOfValue == 0) return BT_INVALID_ARGUMENT;
	if (list->sizeOfNode == 0) return BT_INVALID_ARGUMENT;
	if (value == 0) return BT_INVALID_ARGUMENT;

	BT_STATUS status = 0;

	VOID* prev = NULL;
	VOID* curr = list->root;

	while (curr != NULL) {
		prev = curr;
		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
	}

	UINTN size = (UINTN)list->sizeOfNode;

	VOID* newBlock = curr;

	status = AllocPhysicalPool(&newBlock, &size, BT_MEMORY_KERNEL_RW);
	if (BT_ERROR(status)) goto CLEANUP;

	status = CopyPhysicalMemory(value, (UINTN)sizeOfValue, (VOID*)((PHYSICAL_ADDRESS)newBlock + valueRva));
	if (BT_ERROR(status)) goto CLEANUP;

	if (prev != NULL) {
		*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)prev + list->nextRva) = (PHYSICAL_ADDRESS)newBlock;
	}
	else{
		list->root = newBlock;
	}

	CLEANUP:
	if (BT_ERROR(status) && newBlock) FreePhysicalPool(&newBlock, &size);

	return status;
}
BT_STATUS ByteAPI LinkedUnsafeRemove(IN UNSAFE_LINKED_LIST* list, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 valueRva) {
	if (value == NULL) return BT_INVALID_ARGUMENT;
	if (sizeOfValue == 0) return BT_INVALID_ARGUMENT;
	if (list->sizeOfNode == 0) return BT_INVALID_ARGUMENT;
	if (value == 0) return BT_INVALID_ARGUMENT;

	BT_STATUS status = 0;

	UINTN size = (UINTN)list->sizeOfNode;

	VOID* prev = NULL;
	VOID* next = NULL;
	VOID* curr = list->root;
	VOID* toRemove = NULL;
	
	while (curr != NULL) {
		if (ComparePhysicalMemory((VOID*)((PHYSICAL_ADDRESS)curr + valueRva), (UINTN)sizeOfValue, value) == BT_COMPARE_EQUAL) {
			toRemove = curr;
			next = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
			break;
		}

		prev = curr;
		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
	}

	if (toRemove == NULL) return BT_INVALID_ARGUMENT;

	if (prev != NULL) {
		*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)prev + list->nextRva) = (PHYSICAL_ADDRESS)next;
	}
	else{
		list->root = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)toRemove + list->nextRva));
	}

	status = FreePhysicalPool(&toRemove, &size);
	if (BT_ERROR(status)) goto CLEANUP;

	CLEANUP:

	return status;
}
BT_STATUS ByteAPI LinkedUnsafeFind(IN UNSAFE_LINKED_LIST* list, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 valueRva, OUT VOID** buffer) {
	if (value == NULL) return BT_INVALID_ARGUMENT;
	if (sizeOfValue == 0) return BT_INVALID_ARGUMENT;
	if (list->sizeOfNode == 0) return BT_INVALID_ARGUMENT;
	if (value == 0) return BT_INVALID_ARGUMENT;
	if (buffer == NULL) return BT_INVALID_BUFFER;

	VOID* curr = list->root;
	VOID* found = NULL;

	while (curr != NULL) {
		if (ComparePhysicalMemory((VOID*)((PHYSICAL_ADDRESS)curr + valueRva), (UINTN)sizeOfValue, value) == BT_COMPARE_EQUAL) {
			found = curr;
			break;
		}

		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
	}

	if (found == NULL) return BT_NOT_FOUND;

	*buffer = found;

	return BT_SUCCESS;
}
BT_STATUS ByteAPI LinkedUnsafeGetAt(IN UNSAFE_LINKED_LIST* list, IN UINT32 index, OUT VOID** buffer) {
	if (list == NULL) return BT_INVALID_ARGUMENT;
	if (buffer == NULL) return BT_INVALID_BUFFER;

	VOID* curr = list->root;
	VOID* found = NULL;

	while (curr != NULL) {
		if (index-- == 0) {
			found = curr;
			break;
		}

		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
	}

	if (found == NULL) return BT_NOT_FOUND;

	*buffer = found;

	return BT_SUCCESS;
}
BT_STATUS ByteAPI LinkedUnsafeClear(IN UNSAFE_LINKED_LIST* list) {
	if (list == NULL) return BT_INVALID_ARGUMENT;

	VOID* curr = list->root;
	UINTN nodeSize = list->sizeOfNode;

	while (curr != NULL) {
		VOID* next = (VOID*)*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva);
		FreePhysicalPool(&curr, &nodeSize);
		curr = next;
	}

	list->root = NULL;

	return BT_SUCCESS;
}
BT_STATUS ByteAPI LinkedUnsafeSort(IN UNSAFE_LINKED_LIST* list, IN UINT32 sizeOfValue, IN UINT32 valueRva) {
	if (list == NULL) return BT_INVALID_ARGUMENT;
	if (sizeOfValue == 0) return BT_INVALID_ARGUMENT;
	
	VOID* sorted = NULL;

	while (list->root != NULL) {
		VOID* head = list->root;
		VOID** trail = &sorted;

		list->root = (VOID*)*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)list->root + list->nextRva);

		while (*trail != NULL && ComparePhysicalMemory(LINKED_UNSAFE_VALUE_PTR(head, valueRva), sizeOfValue, LINKED_UNSAFE_VALUE_PTR(*trail, valueRva)) != BT_COMPARE_SMALLER) {
			trail = (VOID**)((PHYSICAL_ADDRESS)*trail + list->nextRva);
		}

		*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)head + list->nextRva) = (PHYSICAL_ADDRESS)*trail;
		*trail = head;
	}

	list->root = sorted;
	return BT_SUCCESS;
}

BT_STATUS ByteAPI LinkedUnsafeSize(IN UNSAFE_LINKED_LIST* list, OUT UINT32* size) {
	if (size == NULL) return BT_INVALID_BUFFER;

	VOID* curr = list->root;

	while (curr != NULL) {
		curr = (VOID*)(*(PHYSICAL_ADDRESS*)((PHYSICAL_ADDRESS)curr + list->nextRva));
		(*size)++;
	}

	return BT_SUCCESS;
}
