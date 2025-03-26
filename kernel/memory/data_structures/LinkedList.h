#include "byteos.h"

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