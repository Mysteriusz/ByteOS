#pragma once

#include "byteos.h"

#define LINKED_LIST_MAX_COUNT 0x10000;

typedef struct DOUBLE_LINKED_LIST_NODE DOUBLE_LINKED_LIST_NODE;
typedef struct LINKED_LIST_NODE LINKED_LIST_NODE;

typedef struct LINKED_LIST_NODE{
    VOID *value;
    UINT32 sizeOfValue;
    LINKED_LIST_NODE *next;
} LINKED_LIST_NODE;

typedef struct DOUBLE_LINKED_LIST_NODE{
    VOID *value;
    UINT32 sizeOfValue;
    DOUBLE_LINKED_LIST_NODE *next;
    DOUBLE_LINKED_LIST_NODE *previous;
} DOUBLE_LINKED_LIST_NODE;

typedef struct LINKED_LIST{
    LINKED_LIST_NODE root;
    UINT32 size;
} LINKED_LIST;

typedef struct DOUBLE_LINKED_LIST{
    DOUBLE_LINKED_LIST_NODE root;
    UINT32 size;
} DOUBLE_LINKED_LIST;

BT_STATUS ByteAPI LinkedCreate(OUT VOID** root, IN UINT32 sizeOfNode);
BT_STATUS ByteAPI LinkedAdd(IN LINKED_LIST* list, IN VOID* value, IN UINT32 sizeofValue);

BT_STATUS ByteAPI LinkedUnsafeCreate(OUT VOID** root, IN UINT32 sizeOfNode);
BT_STATUS ByteAPI LinkedUnsafeAdd(IN VOID* root, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 sizeOfNode, IN UINT32 valueRva, IN UINT32 nextRva);
