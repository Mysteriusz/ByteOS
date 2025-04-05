#include "byteos.h"
#include "pci.h"

#pragma region STRUCTURES

#define PCI_HBA_AHCI_COMMAND_TABLE_MAX_ENTRY_COUNT 0xffff
#define PCI_HBA_AHCI_COMMAND_TABLE_ENTRY_OFFSET 0x80

typedef enum PCI_FIS_TYPE{
	FIS_TYPE_REG_H2D = 0x27,
	FIS_TYPE_REG_D2H = 0x34,
	FIS_TYPE_DMA_ACT = 0x39,
	FIS_TYPE_DMA_SETUP = 0x41, 
	FIS_TYPE_DATA = 0x46,
	FIS_TYPE_BIST = 0x58,
	FIS_TYPE_PIO_SETUP = 0x5f,
	FIS_TYPE_DEV_BITS = 0xa1
} PCI_FIS_TYPE;

typedef volatile struct PCI_HBA_AHCI_FIS_H2D{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 3;
    UINT8 commandControl : 1;
    UINT8 command;
    UINT8 feature0;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 feature1;
    UINT8 count0;
    UINT8 count1;
    UINT8 iCommandCompletion;
    UINT8 control;
    UINT8 reserved1[4];
} PCI_HBA_AHCI_FIS_H2D;
typedef volatile struct PCI_HBA_AHCI_FIS_D2H{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 2;
    UINT8 interrupt : 1;
    UINT8 reserved1 : 1;
    UINT8 status;
    UINT8 error;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 reserved2;
    UINT8 count0;
    UINT8 count1;
    UINT8 reserved3[2];
    UINT8 reserved4[4];
} PCI_HBA_AHCI_FIS_D2H;
typedef volatile struct PCI_HBA_AHCI_FIS_DATA{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 4;
    UINT8 reserved1[2];
    UINT32 data[1];
} PCI_HBA_AHCI_FIS_DATA;
typedef volatile struct PCI_HBA_AHCI_FIS_PIO{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 1;
    UINT8 dataDirection : 1;
    UINT8 interrupt : 1;
    UINT8 reserved1 : 1;
    UINT8 status;
    UINT8 error;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 reserved2;
    UINT8 count0;
    UINT8 count1;
    UINT8 reserved3;
    UINT8 newStatus;
    UINT16 transferCount;
    UINT8 reserved4[2];
} PCI_HBA_AHCI_FIS_PIO;
typedef volatile struct PCI_HBA_AHCI_FIS_DMA{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 1;
    UINT8 dataDirection : 1;
    UINT8 interrupt : 1;
    UINT8 autoActive : 1;
    UINT8 reserved1[2];
    UINT64 dmaBufferId;
    UINT32 reserved2;
    UINT32 dmaBufferOffset;
    UINT32 transferCount;
    UINT32 reserved3;
} PCI_HBA_AHCI_FIS_DMA;
typedef volatile struct PCI_HBA_AHCI_FIS{
    PCI_HBA_AHCI_FIS_DMA dmaSetup;
    UINT8 reserved0[0x04];
    PCI_HBA_AHCI_FIS_PIO pioSetup;
    UINT8 reserved1[0x0c];
    PCI_HBA_AHCI_FIS_D2H d2hSetup;
    UINT8 reserved2[0x04];
    UINT8 setBits[0x02];
    UINT8 unknownFis[0x40];
    UINT8 reserved3[0x100 - 0xa0];
} PCI_HBA_AHCI_FIS;

typedef volatile struct PCI_HBA_AHCI_COMMAND_TABLE_ENTRY{
    UINT32 reserved0 : 1;
    UINT32 dataBaseAddress : 31;
    UINT32 dataBaseAddressUpper;
    UINT32 reserved1;
    UINT32 dataByteCount : 22;
    UINT32 reserved : 9;
    UINT32 interruptOnCompletion : 1;
} PCI_HBA_AHCI_COMMAND_TABLE_ENTRY;
typedef volatile struct PCI_HBA_AHCI_COMMAND_TABLE{
    UINT8 commandFis[0x40];
    UINT8 atapiCommand[0x10];
    UINT8 reserved0[0x30];
    PCI_HBA_AHCI_COMMAND_TABLE_ENTRY entries[1];
} PCI_HBA_AHCI_COMMAND_TABLE;
typedef volatile struct PCI_HBA_AHCI_COMMAND_LIST_HEADER{
    UINT16 commandFisLength : 5;
    UINT16 atapi : 1;
    UINT16 write : 1;
    UINT16 prefetchable : 1;
    UINT16 reset : 1;
    UINT16 bist : 1;
    UINT16 clearBusUponR_OK : 1;
    UINT16 reserved0 : 1;
    UINT16 portMultiplierPort : 4;
    UINT16 physicalRegionDescriptorTableLength;
    UINT32 physicalRegionDescriptorByteCount;
    UINT32 reserved1 : 7;
    UINT32 commandTableDescriptorBaseAddress : 25;
    UINT32 commandTableDescriptorBaseAddressUpper;
} PCI_HBA_AHCI_COMMAND_LIST_HEADER;
typedef volatile struct PCI_HBA_AHCI_COMMAND_LIST{
    PCI_HBA_AHCI_COMMAND_LIST_HEADER commandHeader;
} PCI_HBA_AHCI_COMMAND_LIST;
typedef volatile struct PCI_HBA_AHCI_COMMAND_SESSION{
    PCI_HBA_PORT_REGISTER *port;
    UINT32 portIndex;
} PCI_HBA_AHCI_COMMAND_SESSION;

#pragma endregion STRUCTURES
#pragma region COMMANDS

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_AHCI_COMMAND_SESSION *cmd, IN BYTE *buffer);

#pragma endregion COMMANDS
