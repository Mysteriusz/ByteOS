#pragma once

#include "byteos.h"


#pragma region PCI_CC_DEFINES

// ==================================== |
//             UNCALSSIFIED             |
// ==================================== |

#define PCI_BCC_UNCLASSIFIED 0x00 // Device was built before Class Code definitions were finalized

#define PCI_SCC_0x00_NON_VGA 0x00 
#define PCI_PI_0x00_0x00_AI 0x00 // All currently implemented devices except VGAcompatible devices 

#define PCI_SCC_0x00_VGA 0x01 
#define PCI_PI_0x00_0x01_VGAC 0x00 // VGA-compatible device 

// ==================================== |
//             MASS_STORAGE             |
// ==================================== |

#define PCI_BCC_MASS_STORAGE_CONTROLLER 0x01 // Mass storage controller

#define PCI_SCC_0x01_SCSI 0x00 
#define PCI_PI_0x01_0x00_VS 0x00 // SCSI controller - vendor-specific interface
#define PCI_PI_0x01_0x00_SD 0x11 // SCSI storage device
#define PCI_PI_0x01_0x00_C 0x12 // SCSI controller
#define PCI_PI_0x01_0x00_SDC 0x13 // SCSI storage device and SCSI controller
#define PCI_PI_0x01_0x00_SDPCI 0x21 // SCSI storage device - SCSI over PCI Express

#define PCI_SCC_0x01_IDE 0x01
#define PCI_PI_0x01_0x01_C 0x00 // IDE controller

#define PCI_SCC_0x01_FLOPPY 0x02
#define PCI_PI_0x01_0x02_CVS 0x00 // Floppy disk controller - vendor-specific interface

#define PCI_SCC_0x01_IPI 0x03
#define PCI_PI_0x01_0x03_CVS 0x00 // IPI bus controller - vendor-specific interface

#define PCI_SCC_0x01_RAID 0x04
#define PCI_PI_0x01_0x04_CVS 0x00 // RAID controller - vendor-specific interface 

#define PCI_SCC_0x01_ATA 0x05
#define PCI_PI_0x01_0x05_CSS 0x20 // ATA controller with ADMA interface - single stepping
#define PCI_PI_0x01_0x05_CCO 0x30 // ATA controller with ADMA interface - continuous operation

#define PCI_SCC_0x01_SATA 0x06
#define PCI_PI_0x01_0x06_CVS 0x00 // Serial ATA controller - vendor-specific interface 
#define PCI_PI_0x01_0x06_CAHCI 0x01 // Serial ATA controller - AHCI interface
#define PCI_PI_0x01_0x06_CBUS 0x02 // Serial Storage Bus Interface 

#define PCI_SCC_0x01_SAS 0x07
#define PCI_PI_0x01_0x07_CVS 0x00 // Serial Attached SCSI (SAS) controller - vendor-specific interface
#define PCI_PI_0x01_0x07_OBS 0x01 // Obsolete

#define PCI_SCC_0x01_NVM 0x08
#define PCI_PI_0x01_0x08_NVMVS 0x00 // Non-volatile memory subsystem - vendor-specific interface 
#define PCI_PI_0x01_0x08_NVMI 0x01 // Non-volatile memory subsystem - NVMHCI interface
#define PCI_PI_0x01_0x08_NVMC 0x02 // NVM Express (NVMe) I/O controller
#define PCI_PI_0x01_0x08_NVMAC 0x03 // NVM Express (NVMe) administrative controller

#define PCI_SCC_0x01_UFS 0x09
#define PCI_PI_0x01_0x09_CVS 0x00 // Universal Flash Storage (UFS) controller - vendor-specific interface 
#define PCI_PI_0x01_0x09_C 0x01 // Universal Flash Storage (UFS) controller

#define PCI_SCC_0x01_UNKOWN 0x80
#define PCI_PI_0x01_0x80_CVS 0x00 // Other mass storage controller - vendor-specific interface

// ==================================== |
//               NETWORK                |
// ==================================== |

#define PCI_BCC_NETWORK_CONTROLLER 0x02 // Network controller

#define PCI_SCC_0x02_ETHERNET 0x00
#define PCI_PI_0x02_0x00_C 0x00 // Ethernet controller

#define PCI_SCC_0x02_TOKEN_RING 0x01
#define PCI_PI_0x02_0x01_C 0x00 // Token Ring controller

#define PCI_SCC_0x02_FDDI 0x02
#define PCI_PI_0x02_0x02_C 0x00 // FDDI controller

#define PCI_SCC_0x02_ATM 0x03
#define PCI_PI_0x02_0x03_C 0x00 // ATM controller 

#define PCI_SCC_0x02_ISDN 0x04
#define PCI_PI_0x02_0x04_C 0x00 // ISDN controller

#define PCI_SCC_0x02_WORLD_FIP 0x05
#define PCI_PI_0x02_0x05_C 0x00 // WorldFip controller

#define PCI_SCC_0x02_PICMG 0x06
#define PCI_PI_0x02_0x06_MC 0x00 // PICMG 2.14 Multi Computing 

#define PCI_SCC_0x02_INFIBAND 0x07 
#define PCI_PI_0x02_0x07_C 0x00 // InfiniBand* Controller

#define PCI_SCC_0x02_HOST_FABRIC 0x08 
#define PCI_PI_0x02_0x08_CVS 0x00 // Host fabric controller – vendor-specific

#define PCI_SCC_0x02_UNKNOWN 0x80 
#define PCI_PI_0x02_0x80_C 0x00 // Other network controller 

// ==================================== |
//               DISPLAY                |
// ==================================== |

#define PCI_BCC_DISPLAY_CONTROLLER 0x03 // Display controller 

#define PCI_SCC_0x03_VGA 0x00 
#define PCI_PI_0x03_0x00_C 0x00 // VGA-compatible controller
#define PCI_PI_0x03_0x00_8514C 0x01 // 8514-compatible controller 

#define PCI_SCC_0x03_XGA 0x01 
#define PCI_PI_0x03_0x01_C 0x00 // XGA controller 

#define PCI_SCC_0x03_3D 0x02 
#define PCI_PI_0x03_0x02_C 0x00 // 3D controller 

#define PCI_SCC_0x03_UNKNOWN 0x80 
#define PCI_PI_0x03_0x80_C 0x00 // Other display controller

// ==================================== |
//              MULTIMEDIA              |
// ==================================== |

#define PCI_BCC_MULTIMEDIA_CONTROLLER 0x04 // Multimedia device

#define PCI_SCC_0x04_VIDEO 0x00 
#define PCI_PI_0x04_0x00_DVS 0x00 // Video device – vendor specific interface 

#define PCI_SCC_0x04_AUDIO 0x01 
#define PCI_PI_0x04_0x01_DVS 0x00 // Audio device – vendor specific interface 

#define PCI_SCC_0x04_COMTEL 0x02 
#define PCI_PI_0x04_0x02_DVS 0x00 // Computer telephony device – vendor specific interface

#define PCI_SCC_0x04_HDA 0x03
#define PCI_PI_0x04_0x03_CM 0x00 // High Definition Audio (HD-A) 1.0 compatible
#define PCI_PI_0x04_0x03_CMVS 0x80 // High Definition Audio (HD-A) 1.0 compatible with additional vendor specific extensions 

#define PCI_SCC_0x04_UNKNOWN 0x80 
#define PCI_PI_0x04_0x80_DVS 0x00 // Other multimedia device – vendor specific interface

// ==================================== |
//                MEMORY                |
// ==================================== |

#define PCI_BCC_MEMORY_CONTROLLER 0x05 // Memory controller 

#define PCI_SCC_0x05_RAM 0x00 
#define PCI_PI_0x05_0x00_C 0x00 // RAM

#define PCI_SCC_0x05_FLASH 0x01
#define PCI_PI_0x05_0x01_C 0x00 // Flash

#define PCI_SCC_0x05_UNKNOWN 0x80 
#define PCI_PI_0x05_0x80_C 0x00 // Other memory controller

// ==================================== |
//                BRIDGE                |
// ==================================== |

#define PCI_BCC_BRIDGE_DEVICE 0x06 // Bridge device 

#define PCI_SCC_0x06_HOST 0x00 
#define PCI_PI_0x06_0x00_B 0x00 // Host bridge

#define PCI_SCC_0x06_ISA 0x01 
#define PCI_PI_0x06_0x01_B 0x00 // ISA bridge

#define PCI_SCC_0x06_EISA 0x02 
#define PCI_PI_0x06_0x02_B 0x00 // EISA bridge

#define PCI_SCC_0x06_MCA 0x03
#define PCI_PI_0x06_0x03_B 0x00 // MCA bridge 

#define PCI_SCC_0x06_PTP 0x04 
#define PCI_PI_0x06_0x04_B 0x00 // PCI-to-PCI bridge
#define PCI_PI_0x06_0x04_SDB 0x01 // Subtractive Decode PCI-to-PCI bridge

#define PCI_SCC_0x06_PCMCIA 0x05 
#define PCI_PI_0x06_0x05_B 0x00 // PCMCIA bridge 

#define PCI_SCC_0x06_NUBUS 0x06
#define PCI_PI_0x06_0x06_B 0x00 // NuBus bridge 

#define PCI_SCC_0x06_CARDBUS 0x07 
#define PCI_PI_0x06_0x07_B 0x00 // CardBus bridge 

#define PCI_SCC_0x06_RACE 0x08
#define PCI_PI_0x06_0x08_B 0x00 // RACEway bridge

#define PCI_SCC_0x06_STPTP 0x09 
#define PCI_PI_0x06_0x09_BPRI 0x40 // Semi-transparent PCI-to-PCI bridge with the primary PCI 
#define PCI_PI_0x06_0x09_BSEC 0x80 // Semi-transparent PCI-to-PCI bridge with the secondary PCI 

#define PCI_SCC_0x06_INFTP 0x0a 
#define PCI_PI_0x06_0x0a_B 0x00 // InfiniBand-to-PCI host bridge

#define PCI_SCC_0x06_ASTP 0x0b
#define PCI_PI_0x06_0x0b_BCS 0x00 // Advanced Switching to PCI host bridge–Custom Interface
#define PCI_PI_0x06_0x0b_BASI 0x01 // Advanced Switching to PCI host bridge–ASI-SIG Defined Portal Interface 

#define PCI_SCC_0x06_UNKNOWN 0x80 
#define PCI_PI_0x06_0x80_B 0x00 // Other bridge device

// ==================================== |
//              SIMPLE_COMM             |
// ==================================== |

#define PCI_BCC_SIMPLE_COMM_CONTROLLER 0x07 // Simple communication controllers

#define PCI_SCC_0x07_XT 0x00 
#define PCI_PI_0x07_0x00_GENC 0x00 // Generic XT-compatible serial controller
#define PCI_PI_0x07_0x00_16450C 0x01 // 16450-compatible serial controller     
#define PCI_PI_0x07_0x00_16550C 0x02 // 16550-compatible serial controller
#define PCI_PI_0x07_0x00_16650C 0x03 // 16650-compatible serial controller
#define PCI_PI_0x07_0x00_16750C 0x04 // 16750-compatible serial controller
#define PCI_PI_0x07_0x00_16850C 0x05 // 16850-compatible serial controller
#define PCI_PI_0x07_0x00_16950C 0x06 // 16950-compatible serial controller

#define PCI_SCC_0x07_PORT 0x01
#define PCI_PI_0x07_0x01_PP 0x00 // Parallel port
#define PCI_PI_0x07_0x01_BDPP 0x01 // Bi-directional parallel port 
#define PCI_PI_0x07_0x01_ECP 0x02 // ECP 1.X compliant parallel port 
#define PCI_PI_0x07_0x01_IEEE1284C 0x03 // IEEE1284 controller
#define PCI_PI_0x07_0x01_IEEE1284D 0xfe // IEEE1284 target device

#define PCI_SCC_0x07_MPORT 0x02
#define PCI_PI_0x07_0x02_C 0x00 // Multiport serial controller

#define PCI_SCC_0x07_MODEM 0x03
#define PCI_PI_0x07_0x03_GEN 0x00 // Generic modem 
#define PCI_PI_0x07_0x03_H16450 0x01 // Hayes compatible modem, 16450-compatible interface
#define PCI_PI_0x07_0x03_H16550 0x02 // Hayes compatible modem, 16550-compatible interface
#define PCI_PI_0x07_0x03_H16650 0x03 // Hayes compatible modem, 16650-compatible interface
#define PCI_PI_0x07_0x03_H16750 0x04 // Hayes compatible modem, 16750-compatible interface

#define PCI_SCC_0x07_GPIB 0x04
#define PCI_PI_0x07_0x04_C 0x00 // GPIB (IEEE 488.1/2) controller

#define PCI_SCC_0x07_SMART 0x05
#define PCI_PI_0x07_0x05_CARD 0x00 // Smart Card 

#define PCI_SCC_0x07_UNKNOWN 0x80
#define PCI_PI_0x07_0x80_O 0x00 // Other communications device

// ==================================== |
//              PERIPHERAL              |
// ==================================== |

#define PCI_BCC_BASE_SYSTEM_PERIPHERAL 0x08

#define PCI_SCC_0x08_XT 0x00 
#define PCI_PI_0x08_0x00_GENC 0x00 // Generic XT-compatible system peripheral

#define PCI_SCC_0x08_PIC 0x00
#define PCI_PI_0x08_0x00_GPIC 0x00 // Generic 8259 PIC
#define PCI_PI_0x08_0x00_ISA_PIC 0x01 // ISA PIC
#define PCI_PI_0x08_0x00_EISA_PIC 0x02 // EISA PIC
#define PCI_PI_0x08_0x00_IO_APIC 0x10 // I/O APIC interrupt controller
#define PCI_PI_0x08_0x00_IOX_APIC 0x20 // I/O(x) APIC interrupt controller

#define PCI_SCC_0x08_DMA 0x01
#define PCI_PI_0x08_0x01_GDMA 0x00 // Generic 8237 DMA controller
#define PCI_PI_0x08_0x01_ISA_DMA 0x01 // ISA DMA controller
#define PCI_PI_0x08_0x01_EISA_DMA 0x02 // EISA DMA controller

#define PCI_SCC_0x08_TIMER 0x02
#define PCI_PI_0x08_0x02_GT 0x00 // Generic 8254 system timer
#define PCI_PI_0x08_0x02_ISAT 0x01 // ISA system timer
#define PCI_PI_0x08_0x02_EISAT 0x02 // EISA system timers (two timers)
#define PCI_PI_0x08_0x02_HPET 0x03 // High Performance Event Timer

#define PCI_SCC_0x08_RTC 0x03
#define PCI_PI_0x08_0x03_GC 0x00 // Generic RTC controller
#define PCI_PI_0x08_0x03_ISAC 0x01 // ISA RTC controller

#define PCI_SCC_0x08_PCI_HP 0x04
#define PCI_PI_0x08_0x04_GPCIC 0x00 // Generic PCI Hot-Plug controller

#define PCI_SCC_0x08_SDH 0x05
#define PCI_PI_0x08_0x05_C 0x00 // SD Host controller

#define PCI_SCC_0x08_IOMMU 0x06
#define PCI_PI_0x08_0x06_G 0x00 // IOMMU

#define PCI_SCC_0x08_ROOT_COMPLEX 0x07
#define PCI_PI_0x08_0x07_C 0x00 // Root Complex Event Collector

#define PCI_SCC_0x08_UNKNOWN 0x80
#define PCI_PI_0x08_0x80_OS 0x00 // Other system peripheral

// ==================================== |
//        INPUT_DEVICE_CONTROLLER       |
// ==================================== |

#define PCI_BCC_INPUT_DEVICE_CONTROLLER 0x09

#define PCI_SCC_0x09_KB 0x00
#define PCI_PI_0x09_0x00_C 0x00 // Keyboard controller

#define PCI_SCC_0x09_DG 0x01
#define PCI_PI_0x09_0x01_PEN 0x00 // Digitizer (pen)

#define PCI_SCC_0x09_MS 0x02
#define PCI_PI_0x09_0x02_C 0x00 // Mouse controller

#define PCI_SCC_0x09_SCANNER 0x03
#define PCI_PI_0x09_0x03_C 0x00 // Scanner controller

#define PCI_SCC_0x09_GAMEPORT 0x04
#define PCI_PI_0x09_0x04_C 0x00 // Gameport controller
#define PCI_PI_0x09_0x04_C1 0x10 // Gameport controller

#define PCI_SCC_0x09_UNKNOWN 0x80
#define PCI_PI_0x09_0x80_OC 0x00 // Other input controller

// ==================================== |
//            DOCKING_STATION           |
// ==================================== |

#define PCI_BCC_DOCKING_STATION 0x0a // Docking stations

#define PCI_SCC_0x0a_GENERIC 0x00
#define PCI_PI_0x0a_0x00_CPU 0x00 // Generic docking station

#define PCI_SCC_0x0a_UNKNOWN 0x80
#define PCI_PI_0x0a_0x80_CPU 0x00 // Other type of docking station

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_PROCESSOR 0x0b // Processors

#define PCI_SCC_0x0b_386 0x00
#define PCI_PI_0x0b_0x00_CPU 0x00 // 386

#define PCI_SCC_0x0b_486 0x01
#define PCI_PI_0x0b_0x01_CPU 0x00 // 486

#define PCI_SCC_0x0b_PENTIUM 0x02
#define PCI_PI_0x0b_0x02_CPU 0x00 // Pentium

#define PCI_SCC_0x0b_ALPHA 0x10
#define PCI_PI_0x0b_0x10_CPU 0x00 // Alpha

#define PCI_SCC_0x0b_POWERPC 0x20
#define PCI_PI_0x0b_0x20_CPU 0x00 // PowerPC

#define PCI_SCC_0x0b_MIPS 0x30
#define PCI_PI_0x0b_0x30_CPU 0x00 // MIPS

#define PCI_SCC_0x0b_CO_PROCESSOR 0x40
#define PCI_PI_0x0b_0x40_CPU 0x00 // Co-processor

#define PCI_SCC_0x0b_UNKNOWN 0x80
#define PCI_PI_0x0b_0x80_CPU 0x00 // Other Processor

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_SERIAL_BUS_CONTROLLER 0x0c // Serial bus controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_WIRELESS_CONTROLLER 0x0d // Wireless controller 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_INTELLIGENT_IO_CONTROLLER 0x0e // Intelligent I/O controllers 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_SATELLITE_COMM_CONTROLLER 0x0f // Satellite communication controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_ENCRYPTION_CONTROLLER 0x10 // Encryption/Decryption controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_DATA_ACQ_SIGNAL_PROC 0x11 // Data acquisition and signal processing controllers

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_PROCESSING_ACCELERATORS 0x12 // Processing accelerators 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_NON_ESSENTIAL 0x13 // Non-Essential Instrumentation 

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_RESERVED_START 0x14 // Reserved Start

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_RESERVED_END 0xfe // Reserved End

// ==================================== |
//           DEVICE_CONTROLLER          |
// ==================================== |

#define PCI_BCC_UNKNOWN 0xff // Device does not fit in any defined classes

#pragma endregion PCI_CC_DEFINES

#define PCI_SIZE 0x48

#define PCI_HEADER_TYPE_STANDARD 0x00
#define PCI_HEADER_TYPE_PCI2PCI 0x01
#define PCI_HEADER_TYPE_CARDBUS 0x02

#pragma pack(1)
typedef struct PCI_STATUS_REGISTER{
    UINT8 reserved0 : 2;
    UINT8 interruptStatus : 1;
    UINT8 capabilitiesList : 1;
    UINT8 ssCapable : 1;
    UINT8 reserved1 : 1;
    UINT8 fastB2BCapable : 1;
    UINT8 masterDataParityError : 1;
    UINT8 devselTiming : 2;
    UINT8 signaledTargetAbort : 1;
    UINT8 receivedMasterAbort : 1;
    UINT8 signaledSystemError : 1;
    UINT8 detectedParityError : 1;
} PCI_STATUS_REGISTER;
typedef struct PCI_COMMAND_REGISTER{
    UINT8 ioSpace : 1;
    UINT8 memorySpace : 1;
    UINT8 busMaster : 1;
    UINT8 specialCycles : 1;
    UINT8 memoryWrite : 1;
    UINT8 vgaSnoop : 1;
    UINT8 parityErrorResponse : 1;
    UINT8 reserved0 : 1;
    UINT8 serrEnable : 1;
    UINT8 fastB2BEnable : 1;
    UINT8 interruptDisable : 1;
    UINT8 reserved1 : 5;
} PCI_COMMAND_REGISTER;
typedef struct PCI_HEADER_TYPE{
    UINT8 headerType : 7;
    UINT8 multipleFunctions : 1;
} PCI_HEADER_TYPE;
typedef struct PCI_BIST_REGISTER{
    UINT8 completionCode : 3;
    UINT8 reserved : 2;
    UINT8 startBist : 1;
    UINT8 bistCapable : 1;
} PCI_BIST_REGISTER;
typedef struct PCI_MEMORY_BAR_LAYOUT{
    UINT32 always0 : 1;
    UINT32 type : 2;
    UINT32 prefetchable : 1;
    UINT32 baseAddress : 28;
} PCI_MEMORY_BAR_LAYOUT;
typedef struct PCI_IO_BAR_LAYOUT{
    UINT32 always1 : 1;
    UINT32 reserved : 1;
    UINT32 baseAddress : 30;
} PCI_IO_BAR_LAYOUT;

typedef struct PCI_HEADER_COMMON { // PCI Common Fields 0x00
    UINT16 vendorId;
    UINT16 deviceId;
    PCI_COMMAND_REGISTER command;
    PCI_STATUS_REGISTER status;
    UINT8 revisionId;
    UINT8 pi;
    UINT8 scc;
    UINT8 bcc;
    UINT8 cacheLineSize;
    UINT8 latencyTimer;
    PCI_HEADER_TYPE headerType;
    PCI_BIST_REGISTER bist;
} PCI_HEADER_COMMON;
typedef struct PCI_HEADER_0 { // PCI Header Type 0x00 (Standard)
    PCI_HEADER_COMMON common;
    UINT32 bar0;
    UINT32 bar1;
    UINT32 bar2;
    UINT32 bar3;
    UINT32 bar4;
    UINT32 bar5;
    UINT32 cardbusPointer;
    UINT16 ssVendorId;
    UINT16 ssId;
    UINT32 eRomBaseAddress;
    UINT8 capabilities;
    UINT8 reserved[7];
    UINT8 interruptLine;
    UINT8 interruptPin;
    UINT8 minGrant;
    UINT8 maxLatency;
} PCI_HEADER_0;
typedef struct PCI_HEADER_1 { // Header Type 0x1 (PCI-to-PCI bridge)
    PCI_HEADER_COMMON common;
    UINT32 bar0;
    UINT32 bar1;
    UINT8 primaryBusNumber;
    UINT8 secondaryBusNumber;
    UINT8 subordinateBusNumber;
    UINT8 secondaryLatencyTimer;
    UINT8 ioBase;
    UINT8 ioLimit;
    UINT16 secondaryStatus;
    UINT16 memoryBase;
    UINT16 memoryLimit;
    UINT16 prefetchableMemoryBase;
    UINT16 prefetchableMemoryLimit;
    UINT32 prefetchableBaseUpper;
    UINT32 prefetchableLimitUpper;
    UINT16 ioBaseUpper;
    UINT16 ioLimitUpper;
    UINT8 capability;
    UINT8 reserved[3];
    UINT32 eRomBaseAddress;
    UINT8 interruptLine;
    UINT8 interruptPin;
    UINT16 bridgeControl;
} PCI_HEADER_1;
typedef struct PCI_HEADER_2 { // Header Type 0x2 (PCI-to-CardBus bridge)
    PCI_HEADER_COMMON common;
    UINT32 cardBusSocket;
    UINT8 offsetOfCapabilitiesList;
    UINT8 reserved;
    UINT16 secondaryStatus;
    UINT8 pciBusNumber;
    UINT8 cardBusNumber;
    UINT8 subordinateBusNumber;
    UINT8 cardBusTimer;
    PCI_MEMORY_BAR_LAYOUT memoryBaseAddress0;
    UINT32 memoryBaseLimit0;
    PCI_MEMORY_BAR_LAYOUT memoryBaseAddress1;
    UINT32 memoryBaseLimit1;
    PCI_IO_BAR_LAYOUT ioBaseAddress0;
    UINT32 ioLimit0;
    PCI_IO_BAR_LAYOUT ioBaseAddress1;
    UINT32 ioLimit1;
    UINT8 interruptLine;
    UINT8 interruptPin;
    UINT16 bridgeControl;
    UINT16 subsystemDeviceId;
    UINT16 subsystemVendorId;
    UINT32 legacyModeBaseAddress;
} PCI_HEADER_2;
typedef union PCI_HEADER{
    PCI_HEADER_COMMON common; // 0x00 - 0x10
    PCI_HEADER_0 h0; // 0x00 - 0x40
    PCI_HEADER_1 h1; // 0x00 - 0x40
    PCI_HEADER_2 h2; // 0x00 - 0x48
} PCI_HEADER;
typedef struct PCI{ // TODO
    PCI_HEADER header;
} PCI;
#pragma pack()