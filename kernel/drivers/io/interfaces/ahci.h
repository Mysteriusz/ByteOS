#include "byteos.h"
#include "pci.h"

#pragma region STRUCTURES

#define PCI_HBA_AHCI_COMMAND_TABLE_MAX_ENTRY_COUNT 0xffff
#define PCI_HBA_AHCI_COMMAND_TABLE_ENTRY_OFFSET 0x80

#define PCI_HBA_AHCI_FIS_H2D_TYPE 0x27
#define PCI_HBA_AHCI_FIS_D2H_TYPE 0x34

typedef struct PCI_HBA_AHCI_PORT_FIS_H2D{
    UINT8 fisType;
    UINT8 reserved0 : 5;
    UINT8 read0 : 1;
    UINT8 read1 : 1;
    UINT8 update : 1;
    UINT8 command;
    UINT8 features;

    UINT8 sectorNumber;
    UINT8 cylinderLow;
    UINT8 cylinderHigh;
    UINT8 dev;
    
    UINT8 sectorNumberExp;
    UINT8 cylinderLowExp;
    UINT8 cylinderHighExp;
    UINT8 featuresExp;
    
    UINT8 sectorCount;
    UINT8 sectorCountExp;
    UINT8 reserved1;
    UINT8 control;
    
    UINT8 reserved2;
    UINT8 reserved3;
    UINT8 reserved4;
    UINT8 reserved5;
} PCI_HBA_AHCI_PORT_FIS_H2D;

typedef struct PCI_HBA_AHCI_COMMAND_TABLE_ENTRY{
    UINT32 reserved0 : 1;
    UINT32 dataBaseAddress : 31;
    UINT32 dataBaseAddressUpper;
    UINT32 reserved1;
    UINT32 dataByteCount : 22;
    UINT32 reserved : 9;
    UINT32 interruptOnCompletion : 1;
} PCI_HBA_AHCI_COMMAND_TABLE_ENTRY;

typedef struct PCI_HBA_AHCI_COMMAND_TABLE{
    UINT8 commandFis[0x40];
    UINT8 atapiCommand[0x10];
    UINT8 reserved0[0x30];
    PCI_HBA_AHCI_COMMAND_TABLE_ENTRY entries[1];
} PCI_HBA_AHCI_COMMAND_TABLE;

typedef struct PCI_HBA_AHCI_COMMAND_LIST_HEADER{
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

typedef struct PCI_HBA_AHCI_COMMAND_LIST{
    PCI_HBA_AHCI_COMMAND_LIST_HEADER commandHeader;
} PCI_HBA_AHCI_COMMAND_LIST;

#pragma endregion STRUCTURES

#pragma region COMMANDS

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_PORT_REGISTER *port);

#pragma endregion COMMANDS
