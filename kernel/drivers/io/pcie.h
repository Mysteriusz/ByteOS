#pragma once

#include "byteos.h"

#pragma region PCIE_BCC_DEFINES

// ==================================== |
//             UNCALSSIFIED             |
// ==================================== |

#define PCIE_BCC_UNCLASSIFIED 0x00 // Device was built before Class Code definitions were finalized

#define PCIE_SCC_0x00_NON_VGA 0x00 
#define PCIE_PI_0x00_0x00_AI 0x00 // All currently implemented devices except VGAcompatible devices 

#define PCIE_SCC_0x00_VGA 0x01 
#define PCIE_PI_0x00_0x01_VGAC 0x00 // VGA-compatible device 

// ==================================== |
//             MASS_STORAGE             |
// ==================================== |

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

// ==================================== |
//               NETWORK                |
// ==================================== |

#define PCIE_BCC_NETWORK_CONTROLLER 0x02 // Network controller

#define PCIE_SCC_0x02_ETHERNET 0x00
#define PCIE_PI_0x02_0x00_C 0x00 // Ethernet controller

#define PCIE_SCC_0x02_TOKEN_RING 0x01
#define PCIE_PI_0x02_0x01_C 0x00 // Token Ring controller

#define PCIE_SCC_0x02_FDDI 0x02
#define PCIE_PI_0x02_0x02_C 0x00 // FDDI controller

#define PCIE_SCC_0x02_ATM 0x03
#define PCIE_PI_0x02_0x03_C 0x00 // ATM controller 

#define PCIE_SCC_0x02_ISDN 0x04
#define PCIE_PI_0x02_0x04_C 0x00 // ISDN controller

#define PCIE_SCC_0x02_WORLD_FIP 0x05
#define PCIE_PI_0x02_0x05_C 0x00 // WorldFip controller

#define PCIE_SCC_0x02_PICMG 0x06
#define PCIE_PI_0x02_0x06_MC 0x00 // PICMG 2.14 Multi Computing 

#define PCIE_SCC_0x02_INFIBAND 0x07 
#define PCIE_PI_0x02_0x07_C 0x00 // InfiniBand* Controller

#define PCIE_SCC_0x02_HOST_FABRIC 0x08 
#define PCIE_PI_0x02_0x08_CVS 0x00 // Host fabric controller – vendor-specific

#define PCIE_SCC_0x02_UNKNOWN 0x80 
#define PCIE_PI_0x02_0x80_C 0x00 // Other network controller 

// ==================================== |
//               DISPLAY                |
// ==================================== |

#define PCIE_BCC_DISPLAY_CONTROLLER 0x03 // Display controller 

#define PCIE_SCC_0x03_VGA 0x00 
#define PCIE_PI_0x03_0x00_C 0x00 // VGA-compatible controller
#define PCIE_PI_0x03_0x00_8514C 0x01 // 8514-compatible controller 

#define PCIE_SCC_0x03_XGA 0x01 
#define PCIE_PI_0x03_0x01_C 0x00 // XGA controller 

#define PCIE_SCC_0x03_3D 0x02 
#define PCIE_PI_0x03_0x02_C 0x00 // 3D controller 

#define PCIE_SCC_0x03_UNKNOWN 0x80 
#define PCIE_PI_0x03_0x80_C 0x00 // Other display controller

// ==================================== |
//              MULTIMEDIA              |
// ==================================== |

#define PCIE_BCC_MULTIMEDIA_CONTROLLER 0x04 // Multimedia device

#define PCIE_SCC_0x04_VIDEO 0x00 
#define PCIE_PI_0x04_0x00_DVS 0x00 // Video device – vendor specific interface 

#define PCIE_SCC_0x04_AUDIO 0x01 
#define PCIE_PI_0x04_0x01_DVS 0x00 // Audio device – vendor specific interface 

#define PCIE_SCC_0x04_COMTEL 0x02 
#define PCIE_PI_0x04_0x02_DVS 0x00 // Computer telephony device – vendor specific interface

#define PCIE_SCC_0x04_HDA 0x03
#define PCIE_PI_0x04_0x03_CM 0x00 // High Definition Audio (HD-A) 1.0 compatible
#define PCIE_PI_0x04_0x03_CMVS 0x80 // High Definition Audio (HD-A) 1.0 compatible with additional vendor specific extensions 

#define PCIE_SCC_0x04_UNKNOWN 0x80 
#define PCIE_PI_0x04_0x80_DVS 0x00 // Other multimedia device – vendor specific interface

// ==================================== |
//                MEMORY                |
// ==================================== |

#define PCIE_BCC_MEMORY_CONTROLLER 0x05 // Memory controller 

#define PCIE_SCC_0x05_RAM 0x00 
#define PCIE_PI_0x05_0x00_C 0x00 // RAM

#define PCIE_SCC_0x05_FLASH 0x01
#define PCIE_PI_0x05_0x01_C 0x00 // Flash

#define PCIE_SCC_0x05_UNKNOWN 0x80 
#define PCIE_PI_0x05_0x80_C 0x00 // Other memory controller

// ==================================== |
//                BRIDGE                |
// ==================================== |

#define PCIE_BCC_BRIDGE_DEVICE 0x06 // Bridge device 

#define PCIE_SCC_0x06_HOST 0x00 
#define PCIE_PI_0x06_0x00_B 0x00 // Host bridge

#define PCIE_SCC_0x06_ISA 0x01 
#define PCIE_PI_0x06_0x01_B 0x00 // ISA bridge

#define PCIE_SCC_0x06_EISA 0x02 
#define PCIE_PI_0x06_0x02_B 0x00 // EISA bridge

#define PCIE_SCC_0x06_MCA 0x03
#define PCIE_PI_0x06_0x03_B 0x00 // MCA bridge 

#define PCIE_SCC_0x06_PTP 0x04 
#define PCIE_PI_0x06_0x04_B 0x00 // PCI-to-PCI bridge
#define PCIE_PI_0x06_0x04_SDB 0x01 // Subtractive Decode PCI-to-PCI bridge

#define PCIE_SCC_0x06_PCMCIA 0x05 
#define PCIE_PI_0x06_0x05_B 0x00 // PCMCIA bridge 

#define PCIE_SCC_0x06_NUBUS 0x06
#define PCIE_PI_0x06_0x06_B 0x00 // NuBus bridge 

#define PCIE_SCC_0x06_CARDBUS 0x07 
#define PCIE_PI_0x06_0x07_B 0x00 // CardBus bridge 

#define PCIE_SCC_0x06_RACE 0x08
#define PCIE_PI_0x06_0x08_B 0x00 // RACEway bridge

#define PCIE_SCC_0x06_STPTP 0x09 
#define PCIE_PI_0x06_0x09_BPRI 0x40 // Semi-transparent PCI-to-PCI bridge with the primary PCI 
#define PCIE_PI_0x06_0x09_BSEC 0x80 // Semi-transparent PCI-to-PCI bridge with the secondary PCI 

#define PCIE_SCC_0x06_INFTP 0x0a 
#define PCIE_PI_0x06_0x0a_B 0x00 // InfiniBand-to-PCI host bridge

#define PCIE_SCC_0x06_ASTP 0x0b
#define PCIE_PI_0x06_0x0b_BCS 0x00 // Advanced Switching to PCI host bridge–Custom Interface
#define PCIE_PI_0x06_0x0b_BASI 0x01 // Advanced Switching to PCI host bridge–ASI-SIG Defined Portal Interface 

#define PCIE_SCC_0x06_UNKNOWN 0x80 
#define PCIE_PI_0x06_0x80_B 0x00 // Other bridge device

// ==================================== |
//              SIMPLE_COMM             |
// ==================================== |

#define PCIE_BCC_SIMPLE_COMM_CONTROLLER 0x07 // Simple communication controllers

#define PCIE_SCC_0x07_XT 0x00 
#define PCIE_PI_0x07_0x00_GENC 0x00 // Generic XT-compatible serial controller
#define PCIE_PI_0x07_0x00_16450C 0x01 // 16450-compatible serial controller     
#define PCIE_PI_0x07_0x00_16550C 0x02 // 16550-compatible serial controller
#define PCIE_PI_0x07_0x00_16650C 0x03 // 16650-compatible serial controller
#define PCIE_PI_0x07_0x00_16750C 0x04 // 16750-compatible serial controller
#define PCIE_PI_0x07_0x00_16850C 0x05 // 16850-compatible serial controller
#define PCIE_PI_0x07_0x00_16950C 0x06 // 16950-compatible serial controller

#define PCIE_SCC_0x07_PORT 0x01
#define PCIE_PI_0x07_0x01_PP 0x00 // Parallel port
#define PCIE_PI_0x07_0x01_BDPP 0x01 // Bi-directional parallel port 
#define PCIE_PI_0x07_0x01_ECP 0x02 // ECP 1.X compliant parallel port 
#define PCIE_PI_0x07_0x01_IEEE1284C 0x03 // IEEE1284 controller
#define PCIE_PI_0x07_0x01_IEEE1284D 0xfe // IEEE1284 target device

#define PCIE_SCC_0x07_MPORT 0x02
#define PCIE_PI_0x07_0x02_C 0x00 // Multiport serial controller

#define PCIE_SCC_0x07_MODEM 0x03
#define PCIE_PI_0x07_0x03_GEN 0x00 // Generic modem 
#define PCIE_PI_0x07_0x03_H16450 0x01 // Hayes compatible modem, 16450-compatible interface
#define PCIE_PI_0x07_0x03_H16550 0x02 // Hayes compatible modem, 16550-compatible interface
#define PCIE_PI_0x07_0x03_H16650 0x03 // Hayes compatible modem, 16650-compatible interface
#define PCIE_PI_0x07_0x03_H16750 0x04 // Hayes compatible modem, 16750-compatible interface

#define PCIE_SCC_0x07_GPIB 0x04
#define PCIE_PI_0x07_0x04_C 0x00 // GPIB (IEEE 488.1/2) controller

#define PCIE_SCC_0x07_SMART 0x05
#define PCIE_PI_0x07_0x05_CARD 0x00 // Smart Card 

#define PCIE_SCC_0x07_UNKNOWN 0x80
#define PCIE_PI_0x07_0x80_O 0x00 // Other communications device

// ==================================== |
//              PERIPHERAL              |
// ==================================== |

#define PCIE_BCC_BASE_SYSTEM_PERIPHERAL 0x08

#define PCIE_SCC_0x08_XT 0x00 
#define PCIE_PI_0x08_0x00_GENC 0x00 // Generic XT-compatible system peripheral

#define PCIE_SCC_0x08_PIC 0x00
#define PCIE_PI_0x08_0x00_GPIC 0x00 // Generic 8259 PIC
#define PCIE_PI_0x08_0x00_ISA_PIC 0x01 // ISA PIC
#define PCIE_PI_0x08_0x00_EISA_PIC 0x02 // EISA PIC
#define PCIE_PI_0x08_0x00_IO_APIC 0x10 // I/O APIC interrupt controller
#define PCIE_PI_0x08_0x00_IOX_APIC 0x20 // I/O(x) APIC interrupt controller

#define PCIE_SCC_0x08_DMA 0x01
#define PCIE_PI_0x08_0x01_GDMA 0x00 // Generic 8237 DMA controller
#define PCIE_PI_0x08_0x01_ISA_DMA 0x01 // ISA DMA controller
#define PCIE_PI_0x08_0x01_EISA_DMA 0x02 // EISA DMA controller

#define PCIE_SCC_0x08_TIMER 0x02
#define PCIE_PI_0x08_0x02_GT 0x00 // Generic 8254 system timer
#define PCIE_PI_0x08_0x02_ISAT 0x01 // ISA system timer
#define PCIE_PI_0x08_0x02_EISAT 0x02 // EISA system timers (two timers)
#define PCIE_PI_0x08_0x02_HPET 0x03 // High Performance Event Timer

#define PCIE_SCC_0x08_RTC 0x03
#define PCIE_PI_0x08_0x03_GC 0x00 // Generic RTC controller
#define PCIE_PI_0x08_0x03_ISAC 0x01 // ISA RTC controller

#define PCIE_SCC_0x08_PCI_HP 0x04
#define PCIE_PI_0x08_0x04_GPCIC 0x00 // Generic PCI Hot-Plug controller

#define PCIE_SCC_0x08_SDH 0x05
#define PCIE_PI_0x08_0x05_C 0x00 // SD Host controller

#define PCIE_SCC_0x08_IOMMU 0x06
#define PCIE_PI_0x08_0x06_G 0x00 // IOMMU

#define PCIE_SCC_0x08_ROOT_COMPLEX 0x07
#define PCIE_PI_0x08_0x07_C 0x00 // Root Complex Event Collector

#define PCIE_SCC_0x08_UNKNOWN 0x80
#define PCIE_PI_0x08_0x80_OS 0x00 // Other system peripheral

// ==================================== |
//        INPUT_DEVICE_CONTROLLER       |
// ==================================== |

#define PCIE_BCC_INPUT_DEVICE_CONTROLLER 0x09

#define PCIE_SCC_0x09_KB 0x00
#define PCIE_PI_0x09_0x00_C 0x00 // Keyboard controller

#define PCIE_SCC_0x09_DG 0x01
#define PCIE_PI_0x09_0x01_PEN 0x00 // Digitizer (pen)

#define PCIE_SCC_0x09_MS 0x02
#define PCIE_PI_0x09_0x02_C 0x00 // Mouse controller

#define PCIE_SCC_0x09_SCANNER 0x03
#define PCIE_PI_0x09_0x03_C 0x00 // Scanner controller

#define PCIE_SCC_0x09_GAMEPORT 0x04
#define PCIE_PI_0x09_0x04_C 0x00 // Gameport controller
#define PCIE_PI_0x09_0x04_C1 0x10 // Gameport controller

#define PCIE_SCC_0x09_UNKNOWN 0x80
#define PCIE_PI_0x09_0x80_OC 0x00 // Other input controller

// ==================================== |
//            DOCKING_STATION           |
// ==================================== |

#define PCIE_BCC_DOCKING_STATION 0x0a // Docking stations

#define PCIE_SCC_0x0a_GENERIC 0x00
#define PCIE_PI_0x0a_0x00_CPU 0x00 // Generic docking station

#define PCIE_SCC_0x0a_UNKNOWN 0x80
#define PCIE_PI_0x0a_0x80_CPU 0x00 // Other type of docking station

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_PROCESSOR 0x0b // Processors

#define PCIE_SCC_0x0b_386 0x00
#define PCIE_PI_0x0b_0x00_CPU 0x00 // 386

#define PCIE_SCC_0x0b_486 0x01
#define PCIE_PI_0x0b_0x01_CPU 0x00 // 486

#define PCIE_SCC_0x0b_PENTIUM 0x02
#define PCIE_PI_0x0b_0x02_CPU 0x00 // Pentium

#define PCIE_SCC_0x0b_ALPHA 0x10
#define PCIE_PI_0x0b_0x10_CPU 0x00 // Alpha

#define PCIE_SCC_0x0b_POWERPC 0x20
#define PCIE_PI_0x0b_0x20_CPU 0x00 // PowerPC

#define PCIE_SCC_0x0b_MIPS 0x30
#define PCIE_PI_0x0b_0x30_CPU 0x00 // MIPS

#define PCIE_SCC_0x0b_CO_PROCESSOR 0x40
#define PCIE_PI_0x0b_0x40_CPU 0x00 // Co-processor

#define PCIE_SCC_0x0b_UNKNOWN 0x80
#define PCIE_PI_0x0b_0x80_CPU 0x00 // Other Processor

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_SERIAL_BUS_CONTROLLER 0x0c // Serial bus controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_WIRELESS_CONTROLLER 0x0d // Wireless controller 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_INTELLIGENT_IO_CONTROLLER 0x0e // Intelligent I/O controllers 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_SATELLITE_COMM_CONTROLLER 0x0f // Satellite communication controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_ENCRYPTION_CONTROLLER 0x10 // Encryption/Decryption controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_DATA_ACQ_SIGNAL_PROC 0x11 // Data acquisition and signal processing controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_PROCESSING_ACCELERATORS 0x12 // Processing accelerators 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_NON_ESSENTIAL 0x13 // Non-Essential Instrumentation 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_RESERVED_START 0x14 // Reserved Start

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_RESERVED_END 0xfe // Reserved End

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCIE_BCC_UNKNOWN 0xff // Device does not fit in any defined classes

#pragma endregion PCIE_BCC_DEFINES

#pragma pack(1)
typedef struct PCI_HEADER { // PCI Header
    UINT32 id; // Identifiers
    UINT16 cmd; // Command Register
    UINT16 sts; // Device Status
    UINT8 rid; // Revision ID
    UINT8 picc; // Class Codes (Programming interface)
    UINT8 scc; // Class Codes (Sub-class code)
    UINT8 bcc; // Class Codes (Class code)
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