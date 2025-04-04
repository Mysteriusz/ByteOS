#include "ahci.h"

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_AHCI_COMMAND_SESSION *cmd, IN OUT BYTE *buffer){   
    BT_STATUS status;
    
    UINT64 commandListAddress = ((UINT64)cmd->port->commandListBaseAddressUpper << 32) | (cmd->port->commandListBaseAddress << 10);
    PCI_HBA_AHCI_COMMAND_LIST *commandList = (PCI_HBA_AHCI_COMMAND_LIST*)commandListAddress;

    UINT64 commandTableAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    PCI_HBA_AHCI_COMMAND_TABLE *commandTable = (PCI_HBA_AHCI_COMMAND_TABLE*)commandTableAddress;

    commandList->commandHeader.commandFisLength = sizeof(PCI_HBA_AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = 1;

    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(commandTableAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)(commandTableAddress >> 39);    

    commandTable->entries[0].dataBaseAddress = (UINT32)buffer;
    commandTable->entries[0].dataByteCount = 511;
    commandTable->entries[0].interruptOnCompletion = 1;

    PCI_HBA_AHCI_FIS_H2D *fis = (PCI_HBA_AHCI_FIS_H2D*)commandTable->commandFis;
    fis->fisType = 0x27;
    fis->commandControl = 1;
    fis->command = 0xec;

    status = PCI_HBA_START_DMA_ENGINE(cmd->port);
    if (BT_ERROR(status)){
        return status;
    }

    cmd->port->commandIssued |= 1 << cmd->portIndex;

    return BT_SUCCESS;
}
