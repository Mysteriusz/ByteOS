#include "ahci.h"

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_AHCI_COMMAND_SESSION *cmd, IN UINT32 index){   
    UINT64 commandListAddress = ((UINT64)cmd->port->commandListBaseAddressUpper << 32) | (cmd->port->commandListBaseAddress << 10);
    PCI_HBA_AHCI_COMMAND_LIST *commandList = (PCI_HBA_AHCI_COMMAND_LIST*)commandListAddress;

    UINT64 commandTableAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    PCI_HBA_AHCI_COMMAND_TABLE *commandTable = (PCI_HBA_AHCI_COMMAND_TABLE*)commandTableAddress;
    
    commandTable->commandFis[0] = PCI_HBA_AHCI_FIS_H2D_TYPE;
    commandTable->commandFis[2] = 0xec;

    BT_STATUS status = HBA_START_DMA(cmd->port);
    if (status != BT_IO_HBA_RUNNING_ERROR && BT_ERROR(status)){
        return status;
    }

    return BT_SUCCESS;
}
