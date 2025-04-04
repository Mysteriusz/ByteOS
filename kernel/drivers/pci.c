#include "pci.h"

BT_STATUS HBA_START_DMA(IN PCI_HBA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == TRUE){
        return BT_IO_HBA_RUNNING_ERROR;
    }
    
    port->command.start = 1;
    port->command.fisReceiveEnable = 1;
    return BT_SUCCESS;
}
BT_STATUS HBA_STOP_DMA(IN PCI_HBA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == FALSE){
        return BT_IO_HBA_NOT_RUNNING_ERROR;
    }
    
    port->command.start = 0;
    port->command.fisReceiveEnable = 0;
    return BT_SUCCESS;
}
