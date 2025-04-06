#pragma once

#include "byteos.h"
#include "disk.h"
#include "ahci.h"

typedef volatile struct SATA_HOST_CAPABILITIES{
    UINT32 numberOfPorts : 5;
    UINT32 reserved0 : 3;
    UINT32 numeberOfSlots : 5;
    UINT32 partialStateCapable : 1;
    UINT32 slumberStateCapable : 1;
    UINT32 reserved1 : 1;
    UINT32 supportsPortMultiplierFIS : 1;
    UINT32 supportsPortMultiplier : 1;
    UINT32 supportsPortSelectorAcceleration : 1;
    UINT32 supportsNZdma : 1;
    UINT32 interfaceSpeedSupport : 4;
    UINT32 supportsRawFIS : 1;
    UINT32 supportsActivityLED : 1;
    UINT32 supportsAggresiveLinkPM : 1;
    UINT32 supportsStaggeredSPU : 1;
    UINT32 supportsInterlockSwitch : 1;
    UINT32 supportsColdPresenceDetect : 1;
    UINT32 supportsCmdQueueAcceleration : 1;
    UINT32 supports64Addressing : 1;
} SATA_HOST_CAPABILITIES;
typedef volatile struct SATA_GLOBAL_HOST_CONTROL{
    UINT32 hbaReset : 1;
    UINT32 interruptEnable : 1;
    UINT32 reserved0 : 29;
    UINT32 ahciEnable : 1;
} SATA_GLOBAL_HOST_CONTROL;
typedef volatile struct SATA_GENERIC_HOST_CONTROL{
    SATA_HOST_CAPABILITIES hostCapabilities;
    SATA_GLOBAL_HOST_CONTROL globalHostControl;
    UINT32 interruptPendingStatus;
    UINT32 portsImplemented;
    UINT16 minorVersionNumber;
    UINT16 majorVersionNumber;
} SATA_GENERIC_HOST_CONTROL;

typedef volatile struct SATA_PORT_REGISTER_INTERRUPT_STATUS{
    UINT32 d2hRegisterInterrupt : 1;
    UINT32 pioSetupFisInterrupt : 1;
    UINT32 dmaSetupFisInterrupt : 1;
    UINT32 setDeviceBitsInterrupt : 1;
    UINT32 unknownFisInterrupt : 1;
    UINT32 descriptorProcessed : 1;
    UINT32 portConnectChangeStatus : 1;
    UINT32 deviceInterlockStatus : 1;
    UINT32 reserved0 : 15;
    UINT32 incorrectPortMultiplierStatus : 1;
    UINT32 overflowStatus : 1;
    UINT32 reserved1 : 1;
    UINT32 interfaceNonfatalErrorStatus : 1;
    UINT32 interfaceFatalErrorStatus : 1;
    UINT32 hostBusDataErrorStatus : 1;
    UINT32 hostBusFatalErrorStatus : 1;
    UINT32 taskFileErrorStatus : 1;
    UINT32 coldPortDetectStatus : 1;
} SATA_PORT_REGISTER_INTERRUPT_STATUS;
typedef volatile struct SATA_PORT_REGISTER_INTERRUPT_ENABLE{
    UINT32 deviceToHostRegisterInterruptEnable : 1;
    UINT32 pioSetupFisInterruptEnable : 1;
    UINT32 dmaSetupFisInterruptEnable : 1;
    UINT32 setDeviceBitsInterruptEnable : 1;
    UINT32 unknownFisInterruptEnable : 1;
    UINT32 descriptorProcessedEnable : 1;
    UINT32 portConnectChangeStatusEnable : 1;
    UINT32 deviceInterlockStatusEnable : 1;
    UINT32 reserved0 : 15;
    UINT32 incorrectPortMultiplierStatusEnable : 1;
    UINT32 overflowStatusEnable : 1;
    UINT32 reserved1 : 1;
    UINT32 interfaceNonfatalErrorStatusEnable : 1;
    UINT32 interfaceFatalErrorStatusEnable : 1;
    UINT32 hostBusDataErrorStatusEnable : 1;
    UINT32 hostBusFatalErrorStatusEnable : 1;
    UINT32 taskFileErrorStatusEnable : 1;
    UINT32 coldPortDetectStatusEnable : 1;
} SATA_PORT_REGISTER_INTERRUPT_ENABLE;

#define SATA_PORT_REGISTER_COMMAND_ICC_SL 0x00 // Slumber
#define SATA_PORT_REGISTER_COMMAND_ICC_PR 0x01 // Partial
#define SATA_PORT_REGISTER_COMMAND_ICC_AC 0x02 // Active
#define SATA_PORT_REGISTER_COMMAND_ICC_NI 0x06 // No-Op / Idle
typedef volatile struct SATA_PORT_REGISTER_COMMAND{
    UINT32 start : 1;
    UINT32 spinUpDevice : 1;
    UINT32 powerOnDevice : 1;
    UINT32 portSelectorActive : 1;
    UINT32 fisReceiveEnable : 1;
    UINT32 reserved0 : 3;
    UINT32 currentCommandSlot : 5;
    UINT32 interlockSwitchState : 1;
    UINT32 fisReceiveRunning : 1;
    UINT32 commandListRunning : 1;
    UINT32 portMultiplierFisBasedSwitchingEnable : 1;
    UINT32 portMultiplierAttached : 1;
    UINT32 hotPlugCapablePort : 1;
    UINT32 interlockSwitchAttached2Port : 1;
    UINT32 coldPortLogicAttached2Port : 1;
    UINT32 reserved1 : 3;
    UINT32 deviceIsAtapi : 1;
    UINT32 driveLedOnAtapiEnable : 1;
    UINT32 aggresiveLinkPowerManagementEnable : 1;
    UINT32 aggresiveSlumber : 1;
    UINT32 interfaceCommunicationControl : 4;
} SATA_PORT_REGISTER_COMMAND;

#define SATA_PORT_REGISTER_TFD_BSY 0x80 // Indicates the interface is busy
#define SATA_PORT_REGISTER_TFD_DRQ 0x08 // Indicates a data transfer is requested
#define SATA_PORT_REGISTER_TFD_ERR 0x01 // Indicates an error during the transfer
typedef volatile struct SATA_PORT_REGISTER_TASK_FILE{
    UINT8 status;
    UINT8 error;
    UINT16 reserved;
} SATA_PORT_REGISTER_TASK_FILE;

typedef volatile struct SATA_PORT_REGISTER_SIGNATURE{
    UINT32 sectorCountRegister : 8;
    UINT32 lbaLowRegister : 8;
    UINT32 lbaMidRegister : 8;
    UINT32 lbaHighRegister : 8;
} SATA_PORT_REGISTER_SIGNATURE;

#define SATA_PORT_REGISTER_SSTATUS_DET_NONE 0x00 // No device detected and Phy communication not established 
#define SATA_PORT_REGISTER_SSTATUS_DET_PNC 0x01 // Device presence detected but Phy communication not established 
#define SATA_PORT_REGISTER_SSTATUS_DET_PC 0x03 // Device presence detected and Phy communication established 
#define SATA_PORT_REGISTER_SSTATUS_DET_PHOF 0x04 // Phy in offline mode as a result of the interface being disabled or running in a BIST loopback mode 

#define SATA_PORT_REGISTER_SSTATUS_SPD_NONE 0x00 // Device not present or communication not established
#define SATA_PORT_REGISTER_SSTATUS_SPD_GEN1 0x01 // Generation 1 communication rate negotiated
#define SATA_PORT_REGISTER_SSTATUS_SPD_GEN2 0x02 // Generation 2 communication rate negotiated 

#define SATA_PORT_REGISTER_SSTATUS_IPM_NONE 0x00 // Device not present or communication not established
#define SATA_PORT_REGISTER_SSTATUS_IPM_IA 0x01 // Interface in active state  
#define SATA_PORT_REGISTER_SSTATUS_IPM_IPA 0x02 // Interface in PARTIAL power management state
#define SATA_PORT_REGISTER_SSTATUS_IPM_ISA 0x06 // Interface in SLUMBER power management state
typedef volatile struct SATA_PORT_REGISTER_SSATA_STATUS{
    UINT32 deviceDetection : 4;
    UINT32 currentInterfaceSpeed : 4;
    UINT32 interfacePowerManagement : 4;
    UINT32 reserved : 20;
} SATA_PORT_REGISTER_SSATA_STATUS;

#define SATA_PORT_REGISTER_SCONTROL_DET_NONE 0x00 // No device detection or initialization action requested
#define SATA_PORT_REGISTER_SCONTROL_DET_INIT 0x01 // Perform interface communication initialization sequence to establish communication
#define SATA_PORT_REGISTER_SCONTROL_DET_DIS 0x04 // Disable the Serial ATA interface and put Phy in offline mode

#define SATA_PORT_REGISTER_SCONTROL_SPD_NSP 0x00 // No speed negotiation restrictions 
#define SATA_PORT_REGISTER_SCONTROL_SPD_LSP1 0x01 // Limit speed negotiation to Generation 1 communication rate 
#define SATA_PORT_REGISTER_SCONTROL_SPD_LSP2 0x02 // Limit speed negotiation to Generation 2 communication rate 

#define SATA_PORT_REGISTER_SCONTROL_IPM_NONE 0x00 // No interface restrictions
#define SATA_PORT_REGISTER_SCONTROL_IPM_TPOF 0x01 // Transitions to the PARTIAL state disabled
#define SATA_PORT_REGISTER_SCONTROL_IPM_TSOF 0x02 // Transitions to the SLUMBER state disabled
#define SATA_PORT_REGISTER_SCONTROL_IPM_TPSOF 0x03 // Transitions to both PARTIAL and SLUMBER states disabled
typedef volatile struct SATA_PORT_REGISTER_SSATA_CONTROL{
    UINT32 deviceDetection : 12;
    UINT32 PortMultiplierPort : 4;
    UINT32 selectPowerManagement : 4;
    UINT32 interfacePowerManagementAllowed : 4;
    UINT32 speedAllowed : 4;
    UINT32 deviceDetectionInitialization : 4;
} SATA_PORT_REGISTER_SSATA_CONTROL;

#define SATA_PORT_REGISTER_SERROR_ERR_I 0x0001 // Recovered Data Integrity Error (I)
#define SATA_PORT_REGISTER_SERROR_ERR_M 0x0002 // Recovered Communications Error (M)
#define SATA_PORT_REGISTER_SERROR_ERR_T 0x0100 // Transient Data Integrity Error (T)
#define SATA_PORT_REGISTER_SERROR_ERR_C 0x0200 // Persistent Communication or Data Integrity Error (C)
#define SATA_PORT_REGISTER_SERROR_ERR_P 0x0400 // Protocol Error (P)
#define SATA_PORT_REGISTER_SERROR_ERR_E 0x0800 // Internal Error (E)

#define SATA_PORT_REGISTER_SERROR_DIAG_N 0x0001 // PhyRdy Change (N)
#define SATA_PORT_REGISTER_SERROR_DIAG_I 0x0002 // Phy Internal Error (I)
#define SATA_PORT_REGISTER_SERROR_DIAG_W 0x0004 // Comm Wake (W)
#define SATA_PORT_REGISTER_SERROR_DIAG_B 0x0008 // 10B to 8B Decode Error (B)
#define SATA_PORT_REGISTER_SERROR_DIAG_D 0x0010 // Disparity Error (D)
#define SATA_PORT_REGISTER_SERROR_DIAG_C 0x0020 // CRC Error (C)
#define SATA_PORT_REGISTER_SERROR_DIAG_H 0x0040 // Handshake Error (H)
#define SATA_PORT_REGISTER_SERROR_DIAG_S 0x0080 // Link Sequence Error (S)
#define SATA_PORT_REGISTER_SERROR_DIAG_T 0x0100 // Transport state transition error (T)
#define SATA_PORT_REGISTER_SERROR_DIAG_F 0x0200 // Unrecognized FIS Type (F)
#define SATA_PORT_REGISTER_SERROR_DIAG_X 0x0400 // Exchanged (X)
typedef volatile struct SATA_PORT_REGISTER_SSATA_ERROR{
    UINT32 error : 16;
    UINT32 diagnostics : 16;
} SATA_PORT_REGISTER_SSATA_ERROR;

#define SATA_PORT_REGISTER_RAW_FIS_CONTROL_TRS_0 0x00 // No transmissions have occurred
#define SATA_PORT_REGISTER_RAW_FIS_CONTROL_TRS_1 0x01 // Transmission in progress
#define SATA_PORT_REGISTER_RAW_FIS_CONTROL_TRS_2 0x02 // Transmission complete, X_RDY/X_RDY collision encountered
#define SATA_PORT_REGISTER_RAW_FIS_CONTROL_TRS_3 0x03 // Transmission complete, R_OK returned by device
#define SATA_PORT_REGISTER_RAW_FIS_CONTROL_TRS_4 0x04 // Transmission complete, R_ERR returned by device 
typedef volatile struct SATA_PORT_REGISTER_RAW_FIS_CONTROL{
    UINT16 enableRawFisMode : 1;
    UINT16 receiverReturnR_OK : 1;
    UINT16 receiverReturnR_ERR : 1;
    UINT16 reserved0 : 1;
    UINT16 transmitterReceivedStatus : 3;
    UINT16 reserved1 : 9;
    UINT16 lastReceivedFixLength;
} SATA_PORT_REGISTER_RAW_FIS_CONTROL;

typedef volatile struct SATA_PORT_REGISTER{
    UINT32 reserved0 : 10;
    UINT32 commandListBaseAddress : 22;
    UINT32 commandListBaseAddressUpper;
    UINT32 reserved1 : 8;
    UINT32 fisBaseAddress : 24;
    UINT32 fisBaseAddressUpper;
    SATA_PORT_REGISTER_INTERRUPT_STATUS interruptStatus;
    SATA_PORT_REGISTER_INTERRUPT_ENABLE interruptEnable;
    SATA_PORT_REGISTER_COMMAND command;
    UINT32 padding0;
    SATA_PORT_REGISTER_TASK_FILE taskFileData;
    SATA_PORT_REGISTER_SIGNATURE signature;
    SATA_PORT_REGISTER_SSATA_STATUS sstatus;
    SATA_PORT_REGISTER_SSATA_CONTROL scontrol;
    SATA_PORT_REGISTER_SSATA_ERROR serror;
    UINT32 deviceStatus;
    UINT32 commandIssued;
    UINT32 padding1;
    SATA_PORT_REGISTER_RAW_FIS_CONTROL rawFisControlAndStatus;
} SATA_PORT_REGISTER;

#define SATA_PORT_BASE_COUNT 0x20
#define SATA_PORT_BASE_OFFSET 0x100
#define SATA_PORT_BASE_ALIGNMENT 0x80

#define SATA_PORT_OFFSET(portIndex)(SATA_PORT_BASE_OFFSET + ((UINT32)portIndex * SATA_PORT_BASE_ALIGNMENT))
#define SATA_PORT_FREE(port) \
    (((((SATA_PORT_REGISTER*)port)->taskFileData.status & SATA_PORT_REGISTER_TFD_BSY) == 0) && \
    ((((SATA_PORT_REGISTER*)port)->taskFileData.status & SATA_PORT_REGISTER_TFD_DRQ) == 0))

#define SATA_PORT_COMMAND_LIST_ADDRESS(port)(((UINT64)((SATA_PORT_REGISTER*)port)->commandListBaseAddressUpper) << 32) | (((UINT64)((SATA_PORT_REGISTER*)port)->commandListBaseAddress) << 10)

#define SATA_ISSUE_PORT(commandIssued, portIndex)((commandIssued) |= (1 << (portIndex)))
#define SATA_DEISSUE_PORT(commandIssued, portIndex)((commandIssued) &= ~(1 << (portIndex)))

BT_STATUS SATA_FIND_PORT(IN SATA_GENERIC_HOST_CONTROL *hba, OUT SATA_PORT_REGISTER **port);
BT_STATUS SATA_START_DMA_ENGINE(IN SATA_PORT_REGISTER *port);
BT_STATUS SATA_STOP_DMA_ENGINE(IN SATA_PORT_REGISTER *port);

#pragma region COMMANDS

#pragma pack(1)

#define SATA_FIS_IDENTIFY_DEVICE_SIZE 0x200
typedef struct SATA_IDENTIFY_DEVICE_DATA_GENERAL{
    UINT16 reserved0 : 1;
    UINT16 retired0 : 1;
    UINT16 incompleteResponse : 1;
    UINT16 retired1 : 3;
    UINT16 obselete0 : 3;
    UINT16 retired2 : 6;
    UINT16 ataDevice : 1;
    UINT16 obselete1;
    UINT16 specificConfiguration;
    UINT16 obselete2;
    UINT32 retired3;
    UINT16 obselete3;
    UINT32 reserved1;
    UINT16 retired4;
    CHAR8 serialNumber[20];
    UINT32 retired5;
    UINT16 obselete4;
    CHAR8 firmwareRevision[8];
    CHAR8 modelNumber[40];
} SATA_IDENTIFY_DEVICE_DATA_GENERAL;
typedef struct SATA_IDENTIFY_DEVICE_DATA_CAPABILITIES{
    UINT16 retired0 : 8;
    UINT16 dmaSupported : 1;
    UINT16 lbaSupported : 1;
    UINT16 alwaysOne1 : 1;
    UINT16 alwaysOne2 : 1;
    UINT16 reserved1 : 1;
    UINT16 standby : 1;
    UINT16 reserved2 : 2;
} SATA_IDENTIFY_DEVICE_DATA_CAPABILITIES;
typedef struct SATA_IDENTIFY_DEVICE_DATA_VALIDITY{
    UINT16 obselete0 : 1;
    UINT16 w64_70valid : 1;
    UINT16 w88valid : 1;
    UINT16 reserved0 : 13;
} SATA_IDENTIFY_DEVICE_DATA_VALIDITY;
typedef struct SATA_IDENTIFY_DEVICE_DATA_MULTIWORD{
    UINT16 multiwordDma0Supported : 1;
    UINT16 multiwordDma1Supported : 1;
    UINT16 multiwordDma2Supported : 1;
    UINT16 reserved0 : 5;
    UINT16 multiwordDma0Selected : 1;
    UINT16 multiwordDma1Selected : 1;
    UINT16 multiwordDma2Selected : 1;
    UINT16 reserved1 : 5;
} SATA_IDENTIFY_DEVICE_DATA_MULTIWORD;
typedef struct SATA_IDENTIFY_DEVICE_DATA{
    // W 0-46
    SATA_IDENTIFY_DEVICE_DATA_GENERAL general;
    // W 47
    UINT16 reserved0 : 8;
    UINT16 maxLogicalPerDrq : 8;
    // W 48
    UINT16 trustedComputingFeatureSupported : 1;
    UINT16 trustedComputingFeatureGroup : 13;
    UINT16 alwaysOne0 : 1;
    UINT16 alwaysZero0 : 1;
    // W 49
    SATA_IDENTIFY_DEVICE_DATA_CAPABILITIES capabilities;
    // W 50
    UINT16 alwaysOne1 : 1;
    UINT16 obselete0 : 1;
    UINT16 reserved1 : 12;
    UINT16 alwaysOne2 : 1;
    UINT16 alwaysZero1 : 1;
    // W 51-52
    UINT16 obselete1[2];
    // W 53
    SATA_IDENTIFY_DEVICE_DATA_VALIDITY fieldValidity;
    // W 54-58
    UINT16 obselete2[5];
    // W 59
    UINT16 logicalPerDrq : 8;
    UINT16 multipleSectorSettingValid : 1;
    UINT16 reserved2 : 7;
    // W 60-61
    UINT32 logicalCount;
    // W 62
    UINT16 obselete3;
    // W 63
    SATA_IDENTIFY_DEVICE_DATA_MULTIWORD multiwordDmaTransfer;
    // W 64
    UINT16 pioTransferModes;
    // W 65-68
    UINT16 minMultiwordTransferCycle;
    UINT16 recMultiwordTransferCycleDma;
    UINT16 minPioTransferCycle;
    UINT16 minPioTransferCycleIordy;
    // W 69-255
    UINT16 unk[0xff - 68];
} SATA_IDENTIFY_DEVICE_DATA;

BT_STATUS SATA_IDENTIFY_DEVICE(IN SATA_PORT_REGISTER *port, OUT SATA_IDENTIFY_DEVICE_DATA **buffer);

#define SATA_FIS_READ_DMA_EXT_SIZE 0x200

BT_STATUS SATA_READ_DMA_EXT(IN SATA_PORT_REGISTER *port, IN UINT16 count, OUT BYTE **buffer);

#pragma pack(0)

#pragma endregion COMMANDS
