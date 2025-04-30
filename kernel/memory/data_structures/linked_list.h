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

typedef struct UNSAFE_LINKED_LIST{
    VOID* root;
    UINT32 sizeOfNode; 
    UINT32 nextRva;
} UNSAFE_LINKED_LIST;

BT_STATUS ByteAPI LinkedUnsafeCreate(OUT UNSAFE_LINKED_LIST** list);

BT_STATUS ByteAPI LinkedUnsafeAdd(IN UNSAFE_LINKED_LIST* list, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 valueRva);
BT_STATUS ByteAPI LinkedUnsafeRemove(IN UNSAFE_LINKED_LIST* list, IN VOID* value, IN UINT32 sizeOfValue, IN UINT32 valueRva);
    
BT_STATUS ByteAPI LinkedUnsafeSize(IN UNSAFE_LINKED_LIST* list, OUT UINT32* size);
