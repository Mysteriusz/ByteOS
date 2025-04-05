#include "ahci.h"

BT_STATUS AHCI_IDENTIFY_DEVICE(IN AHCI_COMMAND_SESSION *cmd, OUT BYTE **buffer){   
    BT_STATUS status;
    
    (*(UINT32*)&cmd->port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = PCI_HBA_COMMAND_LIST_ADDRESS(cmd->port);
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
    UINTN s = AHCI_FIS_IDENTIFY_DEVICE_SIZE;
    status = AllocPhysicalPool((VOID**)buffer, &s, BT_MEMORY_KERNEL_RW);
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)*buffer;
    
    // Setup entries
    for (UINT32 i = 0; i < commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)(bufferAddress >> 32);
        commandTable->entries[i].dataByteCount = AHCI_FIS_IDENTIFY_DEVICE_SIZE - 1;
        commandTable->entries[i].interruptOnCompletion = 1;        
    }

    // Queue IDENTIFY_DEVICE
    AHCI_FIS_H2D *fis = (AHCI_FIS_H2D*)&commandTable->commandFis;
    SetPhysicalMemory((VOID*)fis, 0, sizeof(AHCI_FIS_H2D));
    fis->fisType = REG_H2D;
    fis->command = IDENTIFY_DEVICE;
    fis->commandControl = 1;
        
    return bufferAddress;
}
