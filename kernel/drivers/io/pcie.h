#include "byteos.h"

typedef struct PCI_HEADER{
    UINT32 id;
    UINT16 cmd;
    UINT16 sts;
    UINT8 rid;
    UINT32 cc;
    UINT8 cls;
    UINT8 mlt;
    UINT8 htype;
    UINT8 bist;
    UINT32 mlbar;
    UINT32 mubar;
    UINT32 bar2;
    UINT32 bar3;
    UINT32 bar4;
    UINT32 bar5;
    UINT32 ccptr;
    UINT32 erom;
    UINT8 cap;
    UINT8 r[6];
    UINT16 intr;
    UINT8 mgnt;
    UINT8 mlat;
} PCI_HEADER;
typedef struct PCIE{
    PCI_HEADER header;
} PCIE;
