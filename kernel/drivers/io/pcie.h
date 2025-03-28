#include "byteos.h"

#define PCIE_BCC_UNCLASSIFIED 0x00

#define PCIE_BCC_MASS_STORAGE_CONTROLLER 0x01 // Mass storage controller
#define PCIE_SCC_0x01_SCSI 0x00 
#define PCIE_PI_0x01_0x00_VS 0x00 // SCSI controller - vendor-specific interface
#define PCIE_PI_0x01_0x00_SD 0x11 // SCSI storage device
#define PCIE_PI_0x01_0x00_C 0x12 // SCSI controller
#define PCIE_PI_0x01_0x00_SDC 0x13 // SCSI storage device and SCSI controller
#define PCIE_PI_0x01_0x00_SDPCI 0x21 // SCSI storage device - SCSI over PCI Express

#define PCIE_SCC_0x01_IDE 0x01
#define PCIE_PI_0x01_0x01_C 0x00 // IDE controller

#define PCIE_SCC_0x01_FLOPPY 0x02
#define PCIE_PI_0x01_0x02_CVS 0x00 // Floppy disk controller - vendor-specific interface

#define PCIE_SCC_0x01_IPI 0x03
#define PCIE_PI_0x01_0x03_CVS 0x00 // IPI bus controller - vendor-specific interface

#define PCIE_SCC_0x01_RAID 0x04
#define PCIE_PI_0x01_0x04_CVS 0x00 // RAID controller - vendor-specific interface 

#define PCIE_SCC_0x01_ATA 0x05
#define PCIE_PI_0x01_0x05_CSS 0x20 // ATA controller with ADMA interface - single stepping
#define PCIE_PI_0x01_0x05_CCO 0x30 // ATA controller with ADMA interface - continuous operation

#define PCIE_SCC_0x01_SATA 0x06
#define PCIE_PI_0x01_0x06_CVS 0x00 // Serial ATA controller - vendor-specific interface 
#define PCIE_PI_0x01_0x06_CAHCI 0x01 // Serial ATA controller - AHCI interface
#define PCIE_PI_0x01_0x06_CBUS 0x02 // Serial Storage Bus Interface 

#define PCIE_SCC_0x01_SAS 0x07
#define PCIE_PI_0x01_0x07_CVS 0x00 // Serial Attached SCSI (SAS) controller - vendor-specific interface
#define PCIE_PI_0x01_0x07_OBS 0x01 // Obsolete

#define PCIE_SCC_0x01_NVM 0x08
#define PCIE_PI_0x01_0x08_NVMVS 0x00 // Non-volatile memory subsystem - vendor-specific interface 
#define PCIE_PI_0x01_0x08_NVMI 0x01 // Non-volatile memory subsystem - NVMHCI interface
#define PCIE_PI_0x01_0x08_NVMC 0x02 // NVM Express (NVMe) I/O controller
#define PCIE_PI_0x01_0x08_NVMAC 0x03 // NVM Express (NVMe) administrative controller

#define PCIE_SCC_0x01_UFS 0x09
#define PCIE_PI_0x01_0x09_CVS 0x00 // Universal Flash Storage (UFS) controller - vendor-specific interface 
#define PCIE_PI_0x01_0x09_C 0x01 // Universal Flash Storage (UFS) controller

#define PCIE_SCC_0x01_UNKOWN 0x80
#define PCIE_PI_0x01_0x80_CVS 0x00 // Other mass storage controller - vendor-specific interface

#define PCIE_BCC_NETWORK_CONTROLLER 0x02
#define PCIE_BCC_DISPLAY_CONTROLLER 0x03
#define PCIE_BCC_MULTIMEDIA_CONTROLLER 0x04
#define PCIE_BCC_MEMORY_CONTROLLER 0x05
#define PCIE_BCC_BRIDGE_DEVICE 0x06
#define PCIE_BCC_SIMPLE_COMM_CONTROLLER 0x07
#define PCIE_BCC_BASE_SYSTEM_PERIPHERAL 0x08
#define PCIE_BCC_INPUT_DEVICE_CONTROLLER 0x09
#define PCIE_BCC_DOCKING_STATION 0x0a
#define PCIE_BCC_PROCESSOR 0x0b
#define PCIE_BCC_SERIAL_BUS_CONTROLLER 0x0c
#define PCIE_BCC_WIRELESS_CONTROLLER 0x0d
#define PCIE_BCC_INTELLIGENT_IO_CONTROLLER 0x0e
#define PCIE_BCC_SATELLITE_COMM_CONTROLLER 0x0f
#define PCIE_BCC_ENCRYPTION_CONTROLLER 0x10
#define PCIE_BCC_DATA_ACQ_SIGNAL_PROC 0x11
#define PCIE_BCC_RESERVED_START 0x12
#define PCIE_BCC_RESERVED_END 0xfe
#define PCIE_BCC_UNKNOWN 0xff

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