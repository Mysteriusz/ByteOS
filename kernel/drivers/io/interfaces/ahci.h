#pragma once

#include "byteos.h"
#include "pci.h"

#pragma region STRUCTURES

#define AHCI_COMMAND_TABLE_MAX_ENTRY_COUNT 0xffff
#define AHCI_COMMAND_TABLE_ENTRY_OFFSET 0x80

typedef enum AHCI_FIS_COMMAND{
    CHECK_POWER_MODE = 0xe5,
    CHECK_CONFIGURE_STREAM = 0x51,
    DATA_SET_MANAGEMENT = 0x06,
    DEVICE_RESET = 0x08,
    DOWNLOAD_MICROCODE = 0x92,
    DOWNLOAD_MICROCODE_DMA = 0x93,
    EXECUTE_DEVICE_DIAGNOSTICS = 0x90,
    FLUSH_CACHE = 0xe7,
    FLUSH_CACHE_EXT = 0xea,
    IDENTIFY_DEVICE = 0xec,
    IDENTIFY_PACKET_DEVICE = 0xa1,
    IDLE = 0xe3,
    IDLE_IMMEDIATE = 0xe1,
    NCQ_QUEUE_MANAGEMENT = 0x63,
    NOP = 0x00,
    PACKET = 0xa0,
    READ_BUFFER = 0xe4,
    READ_BUFFER_DMA = 0xe9,
    READ_DMA = 0xc8,
    READ_DMA_EXT = 0x25,
    READ_FPDMA_QUEUED = 0x60,
    READ_LOG_EXT = 0x2f,
    READ_LOG_DMA_EXT = 0x47,
    READ_LOG_MULTIPLE = 0xc4,
    READ_LOG_MULTIPLE_EXT = 0x29,
    READ_SECTORS_S = 0x20,
    READ_SECTORS_S_EXT = 0x24,
    READ_STREAM_DMA_EXT = 0x2a,
    READ_STREAM_EXT = 0x2b,
    READ_VERIFY_SECTORS = 0x40,
    READ_VERIFY_SECTORS_EXT = 0x42,
    RECEIVE_FPDMA_QUEUED = 0x65,
    REQUEST_SENSE_DATA_EXT = 0x0b,
    SECURITY_DISABLE_PASSWORD = 0xf6,
    SECURITY_ERASE_PREPARE = 0xf3,
    SECURITY_ERASE_UNIT = 0xf4,
    SECURITY_FREEZE_LOCK = 0xf5,
    SECURITY_SET_PASSWORD = 0xf1,
    SECURITY_UNLOCK = 0xf2,
    SEND_FPDMA_QUEUED = 0x64,
    SET_DATE_TIME_EXT = 0x77,
    SET_FEATURES = 0xef,
    SET_MULTIPLE_MODE = 0xc6,
    SLEEP = 0xe6,
    SMART = 0xb0,
    STANDBY = 0xe2,
    STANDBY_IMMIDIATE = 0xe0,
    TRUSTED_NON_DATA = 0x5b,
    TRUSTED_RECEIVE = 0x5c,
    TRUSTED_RECEIVE_DMA = 0x5d,
    TRUSTED_SEND = 0x5e,
    TRUSTED_SEND_DMA = 0x5f,
    WRITE_BUFFER = 0xe8,
    WRITE_BUFFER_DMA = 0xeb,
    WRITE_DMA = 0xca,
    WRITE_DMA_EXT = 0x35,
    WRITE_DMA_FUA_EXT = 0x3d,
    WRITE_FPDMA_QUEUED = 0x61,
    WRITE_LOG_EXT = 0x3f,
    WRITE_LOG_DMA_EXT = 0x57,
    WRITE_MULTIPLE = 0xc5,
    WRITE_MULTIPLE_EXT = 0x39,
    WRITE_MULTIPLE_FUA_EXT = 0xce,
    WRITE_SECTORS = 0x30,
    WRITE_SECTORS_EXT = 0x34,
    WRITE_STREAM_DMA_EXT = 0x3a,
    WRITE_STREAM_EXT = 0x3b,
    WRITE_UNCORRECTABLE_EXT = 0x45,
} AHCI_FIS_COMMAND;
typedef enum AHCI_FIS_TYPE{
	REG_H2D = 0x27,
	REG_D2H = 0x34,
	DMA_ACT = 0x39,
	DMA_SETUP = 0x41, 
	DATA = 0x46,
	BIST = 0x58,
	PIO_SETUP = 0x5f,
	DEV_BITS = 0xa1
} AHCI_FIS_TYPE;

typedef volatile struct AHCI_FIS_H2D{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 3;
    UINT8 commandControl : 1;
    UINT8 command;
    UINT8 feature0;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 feature1;
    UINT8 count0;
    UINT8 count1;
    UINT8 iCommandCompletion;
    UINT8 control;
    UINT8 reserved1[4];
} AHCI_FIS_H2D;
typedef volatile struct AHCI_FIS_D2H{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 2;
    UINT8 interrupt : 1;
    UINT8 reserved1 : 1;
    UINT8 status;
    UINT8 error;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 reserved2;
    UINT8 count0;
    UINT8 count1;
    UINT8 reserved3[2];
    UINT8 reserved4[4];
} AHCI_FIS_D2H;
typedef volatile struct AHCI_FIS_DATA{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 4;
    UINT8 reserved1[2];
    UINT32 data[1];
} AHCI_FIS_DATA;
typedef volatile struct AHCI_FIS_PIO{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 1;
    UINT8 dataDirection : 1;
    UINT8 interrupt : 1;
    UINT8 reserved1 : 1;
    UINT8 status;
    UINT8 error;
    UINT8 lba0;
    UINT8 lba1;
    UINT8 lba2;
    UINT8 device;
    UINT8 lba3;
    UINT8 lba4;
    UINT8 lba5;
    UINT8 reserved2;
    UINT8 count0;
    UINT8 count1;
    UINT8 reserved3;
    UINT8 newStatus;
    UINT16 transferCount;
    UINT8 reserved4[2];
} AHCI_FIS_PIO;
typedef volatile struct AHCI_FIS_DMA{
    UINT8 fisType;
    UINT8 portMultiplier : 4;
    UINT8 reserved0 : 1;
    UINT8 dataDirection : 1;
    UINT8 interrupt : 1;
    UINT8 autoActive : 1;
    UINT8 reserved1[2];
    UINT64 dmaBufferId;
    UINT32 reserved2;
    UINT32 dmaBufferOffset;
    UINT32 transferCount;
    UINT32 reserved3;
} AHCI_FIS_DMA;
typedef volatile struct AHCI_FIS{
    AHCI_FIS_DMA dmaSetup;
    UINT8 reserved0[0x04];
    AHCI_FIS_PIO pioSetup;
    UINT8 reserved1[0x0c];
    AHCI_FIS_D2H d2hSetup;
    UINT8 reserved2[0x04];
    UINT8 setBits[0x02];
    UINT8 unknownFis[0x40];
    UINT8 reserved3[0x100 - 0xa0];
} AHCI_FIS;

typedef volatile struct AHCI_COMMAND_TABLE_ENTRY{
    UINT32 reserved0 : 1;
    UINT32 dataBaseAddress : 31;
    UINT32 dataBaseAddressUpper;
    UINT32 reserved1;
    UINT32 dataByteCount : 22;
    UINT32 reserved : 9;
    UINT32 interruptOnCompletion : 1;
} AHCI_COMMAND_TABLE_ENTRY;
typedef volatile struct AHCI_COMMAND_TABLE{
    UINT8 commandFis[0x40];
    UINT8 atapiCommand[0x10];
    UINT8 reserved0[0x30];
    AHCI_COMMAND_TABLE_ENTRY entries[1];
} AHCI_COMMAND_TABLE;
typedef volatile struct AHCI_COMMAND_LIST_HEADER{
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
} AHCI_COMMAND_LIST_HEADER;
typedef volatile struct AHCI_COMMAND_LIST{
    AHCI_COMMAND_LIST_HEADER commandHeader;
} AHCI_COMMAND_LIST;

#pragma endregion STRUCTURES