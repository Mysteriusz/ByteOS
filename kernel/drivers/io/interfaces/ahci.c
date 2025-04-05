#include "ahci.h"

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_AHCI_COMMAND_SESSION *cmd, IN BYTE *buffer){   
    BT_STATUS status;
    
    (*(UINT32*)&cmd->port->interruptStatus) = (UINT32)-1;

    UINT64 commandListAddress = ((UINT64)cmd->port->commandListBaseAddressUpper << 32) | (cmd->port->commandListBaseAddress << 10);
    PCI_HBA_AHCI_COMMAND_LIST *commandList = (PCI_HBA_AHCI_COMMAND_LIST*)commandListAddress;

    UINT64 commandTableAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    PCI_HBA_AHCI_COMMAND_TABLE *commandTable = (PCI_HBA_AHCI_COMMAND_TABLE*)commandTableAddress;

    commandList->commandHeader.commandFisLength = sizeof(PCI_HBA_AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = 1;

    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(commandTableAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)(commandTableAddress >> 32);
    
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)buffer;

    for (UINT32 i = 0; i < commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        commandTable->entries[0].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[0].dataBaseAddressUpper = (UINT32)(bufferAddress >> 32);
        commandTable->entries[0].dataByteCount = 0x200 - 1;
        commandTable->entries[0].interruptOnCompletion = 1;        
    }

    PCI_HBA_AHCI_FIS_H2D *fis = (PCI_HBA_AHCI_FIS_H2D*)(&commandTable->commandFis);
    SetPhysicalMemory((VOID*)fis, 0, sizeof(PCI_HBA_AHCI_FIS_H2D));
    fis->fisType = FIS_TYPE_REG_H2D;
    fis->command = 0xec;
    fis->commandControl = 1;
        
    UINT64 d = ((UINT64)commandTable->entries[0].dataBaseAddressUpper << 32) | (commandTable->entries[0].dataBaseAddress << 1);
    return d;
    return BT_SUCCESS;
}
