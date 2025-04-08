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

BT_STATUS SATA_IDENTIFY_DEVICE(IN SATA_PORT_REGISTER *port, OUT SATA_IDENTIFY_DEVICE_DATA **buffer){
    BT_STATUS status;
    
    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((PHYSICAL_ADDRESS)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    SetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE));

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
    fis->fisType = REG_H2D;
    fis->command = IDENTIFY_DEVICE;
    fis->commandControl = 1;
        
    return BT_SUCCESS;
}
BT_STATUS SATA_READ_DMA_EXT(IN SATA_PORT_REGISTER *port, IN PHYSICAL_ADDRESS address, IN UINT32 count, OUT BYTE **buffer){
    BT_STATUS status;
    
    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((PHYSICAL_ADDRESS)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    SetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));

    // Allocate buffer pages
    UINTN s = count * SATA_BASE_SECTOR_SIZE;
    status = AllocPhysicalPages((VOID**)buffer, &s, BT_MEMORY_KERNEL_RW);
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)*buffer;

    if (count > AHCI_PRDTL_TABLE_ENTRY_SECTORS){
        count = (SATA_BASE_SECTOR_SIZE * count) / SATA_BASE_SECTOR_SIZE;
    }

    // Setup command`s header
    commandList->commandHeader.commandFisLength = sizeof(AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = count;
    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(ctAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)(ctAddress >> 32);
    
    // Setup entries
    for (UINT32 i = 0; i < (UINT32)commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)(bufferAddress >> 32);
        commandTable->entries[i].dataByteCount = SATA_BASE_SECTOR_SIZE - 1;
        commandTable->entries[i].interruptOnCompletion = 1;        

        bufferAddress += SATA_BASE_SECTOR_SIZE;
    }

    // Queue READ_DMA_EXT
    AHCI_FIS_H2D *fis = (AHCI_FIS_H2D*)&commandTable->commandFis;
    fis->fisType = REG_H2D;
    fis->command = READ_DMA_EXT;
    fis->commandControl = 1;
    
    fis->lba0 = (UINT8)(address);
    fis->lba1 = (UINT8)(address >> 8);
    fis->lba2 = (UINT8)(address >> 16);
    fis->lba3 = (UINT8)(address >> 24);
    fis->lba4 = (UINT8)(address >> 32);
    fis->lba5 = (UINT8)(address >> 40);    
    fis->device = (1 << 6);

    fis->count0 = count & 0xff;
    fis->count1 = (count >> 8) & 0xff;

    return BT_SUCCESS;
}
