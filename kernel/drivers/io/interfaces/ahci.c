#include "ahci.h"

BT_STATUS AHCI_IDENTIFY_DEVICE(IN PCI_HBA_PORT_REGISTER *port){   
    UINT64 commandListAddress = ((UINT64)port->commandListBaseAddressUpper << 32) | (port->commandListBaseAddress << 10);
    PCI_HBA_AHCI_COMMAND_LIST *commandList = (PCI_HBA_AHCI_COMMAND_LIST*)commandListAddress;

    UINT64 commandTableAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    PCI_HBA_AHCI_COMMAND_TABLE *commandTable = (PCI_HBA_AHCI_COMMAND_TABLE*)commandTableAddress;
    
    commandTable->commandFis[0] = PCI_HBA_AHCI_FIS_H2D_TYPE;
    commandTable->commandFis[2] = 0xec;

    BT_STATUS status = HBA_START_DMA(port);
    if (BT_ERROR(status)){
        return status;
    }

    return BT_SUCCESS;
}
