#include "sata.h"
#include "../pci.h"

BT_STATUS SATA_FIND_PORT(IN SATA_GENERIC_HOST_CONTROL *hba, OUT SATA_PORT_REGISTER **port){
    for (UINT32 i = 0; i < SATA_PORT_BASE_COUNT; i++){
        if ((hba->portsImplemented & (1 << i)) == TRUE){
            SATA_PORT_REGISTER *temp = (SATA_PORT_REGISTER*)((PHYSICAL_ADDRESS)hba + SATA_PORT_OFFSET(i));
            
            if (SATA_PORT_FREE(temp) == FALSE){
                continue;
            }

            *port = temp;
            break;
        }

        if (i == SATA_PORT_BASE_COUNT - 1){
            *port = NULL;
        }
    }

    return BT_SUCCESS;
}
BT_STATUS SATA_START_DMA_ENGINE(IN SATA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == TRUE){
        return BT_ENGINE_RUNNING_ERROR;
    }
    
    port->command.start = 1;
    return BT_SUCCESS;
}
BT_STATUS SATA_STOP_DMA_ENGINE(IN SATA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == FALSE){
        return BT_ENGINE_NOT_RUNNING_ERROR;
    }
    
    port->command.start = 0;
    return BT_SUCCESS;
}

BT_STATUS SATA_IDENTIFY_DEVICE(IN SATA_PORT_REGISTER *port, OUT BYTE **buffer){
    BT_STATUS status;
    
    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((PHYSICAL_ADDRESS)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;

    // Setup command`s header
    commandList->commandHeader.commandFisLength = sizeof(AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = 1;
    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(ctAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)(ctAddress >> 32);
    
    // Allocate buffer pool
    UINTN s = SATA_FIS_IDENTIFY_DEVICE_SIZE;
    status = AllocPhysicalPool((VOID**)buffer, &s, BT_MEMORY_KERNEL_RW);
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)*buffer;
    
    // Setup entries
    for (UINT32 i = 0; i < commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)(bufferAddress >> 32);
        commandTable->entries[i].dataByteCount = SATA_FIS_IDENTIFY_DEVICE_SIZE - 1;
        commandTable->entries[i].interruptOnCompletion = 1;        
    }

    // Queue IDENTIFY_DEVICE
    AHCI_FIS_H2D *fis = (AHCI_FIS_H2D*)&commandTable->commandFis;
    SetPhysicalMemory((VOID*)fis, 0, sizeof(AHCI_FIS_H2D));
    fis->fisType = REG_H2D;
    fis->command = IDENTIFY_DEVICE;
    fis->commandControl = 1;
        
    return BT_SUCCESS;
}
