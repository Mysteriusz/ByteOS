#include "byteos.h"

#pragma pack(1)
typedef struct PCI_HEADER { // PCI Header
    UINT32 id; // Identifiers
    UINT16 cmd; // Command Register
    UINT16 sts; // Device Status
    UINT8 rid; // Revision ID
    UINT8 cc[3]; // Class Codes
    UINT8 cls; // Cache Line Size
    UINT8 mlt; // Master Latency Timer
    UINT8 htype; // Header Type
    UINT8 bist; // Built-In Self Test (Optional)
    UINT32 mlbar; // Memory Register Base Address, lower 32-bits <BAR0>
    UINT32 mubar; // Memory Register Base Address, upper 32-bits <BAR1>
    UINT32 bar2; // Refer to section 2.1.12
    UINT32 bar3; // Vendor Specific
    UINT32 bar4; // Vendor Specific
    UINT32 bar5; // Vendor Specific
    UINT32 ccptr; // CardBus CIS Pointer
    UINT32 ss; // Subsystem Identifiers
    UINT32 erom; // Expansion ROM Base Address (Optional)
    UINT8 cap; // Capabilities Pointer
    UINT8 r[7]; // Reserved
    UINT16 intr; // Interrupt Information
    UINT8 mgnt; // Minimum Grant (Optional)
    UINT8 mlat; // Maximum Latency (Optional)
} PCI_HEADER;
typedef struct PCI_PMC { // PCI Power Management Capability
    UINT16 pid;   // PCI Power Management Capability ID
    UINT16 pc;    // PCI Power Management Capabilities
    UINT16 pmcs;  // PCI Power Management Control and Status
} PCI_PMC;
typedef struct PCI_MSIC{ // Message Signaled Interrupt Capability
    UINT16 mid; // Message Signaled Interrupt Capability ID
    UINT16 mc; // Message Signaled Interrupt Message Control
    UINT32 ma; // Message Signaled Interrupt Message Address
    UINT32 mua; // Message Signaled Interrupt Upper Address 
    UINT16 md; // Message Signaled Interrupt Message Data
    UINT32 mmask; // Message Signaled Interrupt Mask Bits (Optional)
    UINT32 mpend; // Message Signaled Interrupt Pending Bits (Optional)
} PCI_MSIC;
typedef struct PCI_MSIX{ // MSI-X Capability
    UINT16 mxid; // MSI-X Capability ID
    UINT16 mxc; // MSI-X Message Control
    UINT32 mtab; // MSI-X Table Offset and Table BIR
    UINT32 mpba; // MSI-X PBA Offset and PBA BIR
} PCI_MSIX;
typedef struct PCI_EC{ // PCI Express Capability
    UINT16 pxid; // PCI Express Capability ID
    UINT16 pxcap; // PCI Express Capabilities
    UINT32 pxdcap; // PCI Express Device Capabilities
    UINT16 pxdc; // PCI Express Device Control
    UINT16 pxds; // PCI Express Device Status
    UINT32 pxlcap; // PCI Express Link Capabilities
    UINT16 pxlc; // PCI Express Link Control
    UINT16 pxls; // PCI Express Link Status
    UINT32 pxdcap2; // PCI Express Device Capabilities 2
    UINT32 pxdc2; // PCI Express Device Control 2
} PCI_EC;
typedef struct PCI_AERC{ // Advanced Error Reporting Capability
    UINT32 aerid; // AER Capability ID
    UINT32 aeruces; // AER Uncorrectable Error Status Register
    UINT32 aerucem; // AER Uncorrectable Error Mask Register
    UINT32 aerucesev; // AER Uncorrectable Error Severity Register
    UINT32 aerces; // AER Correctable Error Status Register
    UINT32 aercem; // AER Correctable Error Mask Register
    UINT32 aercc; // AER Advanced Error Capabilities and Control Reg.
    UINT8 aerhl[16]; // AER Header Log Register
    UINT8 aertlp[16]; // AER TLP Prefix Log Register (Optional)
} PCI_AERC;
typedef struct PCIE{ // PCI-e Structure
    PCI_HEADER header;
    PCI_PMC pmc;
    PCI_MSIC msic;
    PCI_MSIX msix;
    PCI_EC ec;
    PCI_AERC aerc;
} PCIE;
#pragma pack()