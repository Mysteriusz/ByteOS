#include "sata.h"
#include "x86.h"

BT_STATUS SataFindFreePort(IN SATA_GENERIC_HOST_CONTROL *hba, OUT SATA_PORT_REGISTER **port, OUT UINT32 *index){
    if (hba == NULL){
        return BT_INVALID_ARGUMENT;
    }
    if (port == NULL){
        return BT_INVALID_BUFFER;
    }
    if (index == NULL){
        return BT_INVALID_BUFFER;
    }

    for (UINT32 i = 0; i < SATA_PORT_BASE_COUNT; i++){
        if ((hba->portsImplemented & (1 << i)) == TRUE){
            SATA_PORT_REGISTER *temp = (SATA_PORT_REGISTER*)((PHYSICAL_ADDRESS)hba + SATA_PORT_OFFSET(i));
            
            if (SATA_PORT_FREE(temp) == FALSE){
                continue;
            }

            *port = temp;
            *index = i;
            break;
        }

        if (i == SATA_PORT_BASE_COUNT - 1){
            *port = NULL;
        }
    }

    return BT_SUCCESS;
}
BT_STATUS SataStartDmaEngine(IN SATA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == TRUE){
        return BT_ENGINE_RUNNING_ERROR;
    }
    
    port->command.start = 1;
    port->command.fisReceiveEnable = 1;
    return BT_SUCCESS;
}
BT_STATUS SataStopDmaEngine(IN SATA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }

    if (port->command.commandListRunning == FALSE){
        return BT_ENGINE_NOT_RUNNING_ERROR;
    }
    
    port->command.start = 0;
    port->command.fisReceiveEnable = 1;
    return BT_SUCCESS;
}

BT_STATUS SataIdentifyDevice(IN SATA_PORT_REGISTER *port, IN OUT SATA_IDENTIFY_DEVICE_DATA **buffer){
    if (port == NULL) return BT_INVALID_ARGUMENT;
    if (*buffer == NULL) return BT_INVALID_BUFFER;
    
    BT_STATUS status = 0;
    status = PhysicalCheckPermission(buffer, BT_MEMORY_WRITE, PAGE_SIZE);
    if (BT_ERROR(status)) return status;

    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    status = ForceSetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));
    if (BT_ERROR(status)) return status;

    // Setup command`s header
    commandList->commandHeader.commandFisLength = sizeof(AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = 1;
    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(ctAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)((UINT64)ctAddress >> 32);
    
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)*buffer;
    
    // Setup entries
    for (UINT32 i = 0; i < commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)((UINT64)bufferAddress >> 32);
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
BT_STATUS SataReadDmaExt(IN SATA_PORT_REGISTER *port, IN UINT64 lba, IN UINT32 count, IN OUT VOID **buffer){
    if (buffer == NULL) return BT_INVALID_BUFFER;
    if (port == NULL) return BT_INVALID_ARGUMENT;
    if (count == 0) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;
    status = PhysicalCheckPermission(buffer, BT_MEMORY_WRITE, PAGE_SIZE);
    if (BT_ERROR(status)) return status;

    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    status = ForceSetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));
    if (BT_ERROR(status)) return status;

    // Setup command`s header
    commandList->commandHeader.commandFisLength = sizeof(AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 0;
    commandList->commandHeader.physicalRegionDescriptorTableLength = (count + AHCI_PRDTL_TABLE_ENTRY_SECTORS - 1) / AHCI_PRDTL_TABLE_ENTRY_SECTORS;
    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(ctAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)((UINT64)ctAddress >> 32);
    
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)*buffer;

    UINT32 toAdd = count;
    // Setup entries
    for (UINT32 i = 0; i < (UINT32)commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        UINT32 added = AHCI_PRDTL_TABLE_ENTRY_SECTORS;
        if (toAdd < AHCI_PRDTL_TABLE_ENTRY_SECTORS){
            added = toAdd;
        }
        
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)((UINT64)bufferAddress >> 32);
        commandTable->entries[i].dataByteCount = (added * SATA_BASE_SECTOR_SIZE) - 1;
        commandTable->entries[i].interruptOnCompletion = 1;

        bufferAddress += AHCI_PRDTL_TABLE_ENTRY_CAPACITY;
        toAdd -= added;
    }

    // Queue READ_DMA_EXT
    AHCI_FIS_H2D *fis = (AHCI_FIS_H2D*)&commandTable->commandFis;
    fis->fisType = REG_H2D;
    fis->command = READ_DMA_EXT;
    fis->commandControl = 1;
    
    fis->lba0 = (UINT8)(lba);
    fis->lba1 = (UINT8)(lba >> 8);
    fis->lba2 = (UINT8)(lba >> 16);
    fis->lba3 = (UINT8)(lba >> 24);
    fis->lba4 = (UINT8)(lba >> 32);
    fis->lba5 = (UINT8)(lba >> 40);    
    fis->device = (1 << 6);

    fis->count0 = count & 0xff;
    fis->count1 = (count >> 8) & 0xff;

    return BT_SUCCESS;
}
BT_STATUS SataWriteDmaExt(IN SATA_PORT_REGISTER *port, IN UINT64 lba, IN UINT32 count, IN VOID *buffer){
    if (buffer == NULL) return BT_INVALID_BUFFER;
    if (port == NULL) return BT_INVALID_ARGUMENT;
    if (count == 0) return BT_INVALID_ARGUMENT;

    BT_STATUS status = 0;
    status = PhysicalCheckPermission(buffer, BT_MEMORY_WRITE, PAGE_SIZE);
    if (BT_ERROR(status)) return status;
    
    (*(UINT32*)&port->interruptStatus) = (UINT32)-1;

    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    status = ForceSetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));
    if (BT_ERROR(status)) return status;

    // Setup command`s header
    commandList->commandHeader.commandFisLength = sizeof(AHCI_FIS_H2D) / 4;
    commandList->commandHeader.write = 1;
    commandList->commandHeader.physicalRegionDescriptorTableLength = (count + AHCI_PRDTL_TABLE_ENTRY_SECTORS - 1) / AHCI_PRDTL_TABLE_ENTRY_SECTORS;
    commandList->commandHeader.commandTableDescriptorBaseAddress = (UINT32)(ctAddress >> 7);
    commandList->commandHeader.commandTableDescriptorBaseAddressUpper = (UINT32)((UINT64)ctAddress >> 32);
    
    PHYSICAL_ADDRESS bufferAddress = (PHYSICAL_ADDRESS)buffer;

    UINT32 toAdd = count;
    // Setup entries
    for (UINT32 i = 0; i < (UINT32)commandList->commandHeader.physicalRegionDescriptorTableLength; i++){
        UINT32 added = AHCI_PRDTL_TABLE_ENTRY_SECTORS;
        if (toAdd < AHCI_PRDTL_TABLE_ENTRY_SECTORS){
            added = toAdd;
        }
        
        commandTable->entries[i].dataBaseAddress = (UINT32)(bufferAddress >> 1);
        commandTable->entries[i].dataBaseAddressUpper = (UINT32)((UINT64)bufferAddress >> 32);
        commandTable->entries[i].dataByteCount = (added * SATA_BASE_SECTOR_SIZE) - 1;
        commandTable->entries[i].interruptOnCompletion = 1;

        bufferAddress += AHCI_PRDTL_TABLE_ENTRY_CAPACITY;
        toAdd -= added;
    }

    // Queue WRITE_DMA_EXT
    AHCI_FIS_H2D *fis = (AHCI_FIS_H2D*)&commandTable->commandFis;
    fis->fisType = REG_H2D;
    fis->command = WRITE_DMA_EXT;
    fis->commandControl = 1;
    
    fis->lba0 = (UINT8)(lba);
    fis->lba1 = (UINT8)(lba >> 8);
    fis->lba2 = (UINT8)(lba >> 16);
    fis->lba3 = (UINT8)(lba >> 24);
    fis->lba4 = (UINT8)(lba >> 32);
    fis->lba5 = (UINT8)(lba >> 40);    
    fis->device = (1 << 6);

    fis->count0 = count & 0xff;
    fis->count1 = (count >> 8) & 0xff;

    return BT_SUCCESS;
}
BT_STATUS SataFlushDmaExt(IN SATA_PORT_REGISTER *port){
    if (port == NULL) return BT_INVALID_ARGUMENT;
    
    BT_STATUS status;
    
    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    status = ForceSetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));
    if (BT_ERROR(status)) return status;

    AHCI_FIS_H2D* fis = (AHCI_FIS_H2D*)commandTable->commandFis;
    fis->fisType = REG_H2D;
    fis->command = FLUSH_CACHE_EXT;
    fis->commandControl = 1;

    return BT_SUCCESS;
}
BT_STATUS SataDeviceReset(IN SATA_PORT_REGISTER *port){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }
    
    BT_STATUS status;
    
    PHYSICAL_ADDRESS clAddress = SATA_PORT_COMMAND_LIST_ADDRESS(port);
    AHCI_COMMAND_LIST *commandList = (AHCI_COMMAND_LIST*)clAddress;
    PHYSICAL_ADDRESS ctAddress = ((UINT64)commandList->commandHeader.commandTableDescriptorBaseAddressUpper << 32) | (commandList->commandHeader.commandTableDescriptorBaseAddress << 7);
    AHCI_COMMAND_TABLE *commandTable = (AHCI_COMMAND_TABLE*)ctAddress;
    status = ForceSetPhysicalMemory((VOID*)commandTable, 0, sizeof(AHCI_COMMAND_TABLE) + (commandList->commandHeader.physicalRegionDescriptorTableLength - 1) * sizeof(AHCI_COMMAND_TABLE_ENTRY));
    if (BT_ERROR(status)){
        return status;
    }

    AHCI_FIS_H2D* fis = (AHCI_FIS_H2D*)commandTable->commandFis;
    fis->fisType = REG_H2D;
    fis->command = DEVICE_RESET;
    fis->commandControl = 1;

    return BT_SUCCESS;
}
BT_STATUS SataSafePortRun(IN SATA_PORT_REGISTER *port, IN UINT32 portIndex){
    if (port == NULL){
        return BT_INVALID_ARGUMENT;
    }
    
    SATA_ISSUE_PORT(port->commandIssued, portIndex);

    UINT32 sfc = 0x1000000;
    while (SATA_PORT_FREE(port) == FALSE || (port->commandIssued & (1 << portIndex))){
        if (--sfc == 0){
            break;
        }
    }
    if (sfc == 0) return BT_IO_INVALID_PCI;

    return BT_SUCCESS;
}
