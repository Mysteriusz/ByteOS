#pragma once

#include "../efi/efi.h"

#define MAGIC_PE32 0x10b
#define MAGIC_PE32_PLUS 0x20b

#pragma pack(push, 1)

typedef struct {
    UINT16 magic;
    UINT8 majorLinkerVersion;    
    UINT8 minorLinkerVersion;    
    UINT32 sizeOfCode;    
    UINT32 sizeOfInitializedData;    
    UINT32 sizeOfUninitializedData;    
    UINT32 addressOfEntryPoint;    
    UINT32 baseOfCode;    
    UINT32 baseOfData;    
} STANDARD_PE;
typedef struct {
    UINT16 magic;
    UINT8 majorLinkerVersion;    
    UINT8 minorLinkerVersion;    
    UINT32 sizeOfCode;    
    UINT32 sizeOfInitializedData;    
    UINT32 sizeOfUninitializedData;    
    UINT32 addressOfEntryPoint;    
    UINT32 baseOfCode;   
} STANDARD_PE_PLUS;

typedef struct {
    UINT32 imageBase;
    UINT32 sectionAlignment;
    UINT32 fileAlignment;
    UINT16 majorOperatingSystemVersion;
    UINT16 minorOperatingSystemVersion;
    UINT16 majorImageVersion;
    UINT16 minorImageVersion;
    UINT16 majorSubsystemVersion;
    UINT16 minorSubsystemVersion;
    UINT32 win32VersionValue;
    UINT32 sizeOfImage;
    UINT32 sizeOfHeaders;
    UINT32 checkSum;
    UINT16 subsystem;
    UINT16 dllCharacteristics;
    UINT32 sizeOfStackReserve;
    UINT32 sizeOfStackCommit;
    UINT32 sizeOfHeapReserve;
    UINT32 sizeOfHeapCommit;
    UINT32 loaderFlags;
    UINT32 numberOfRvaAndSizes;
} OPTIONAL_PE;
typedef struct {
    UINT64 imageBase;
    UINT32 sectionAlignment;
    UINT32 fileAlignment;
    UINT16 majorOperatingSystemVersion;
    UINT16 minorOperatingSystemVersion;
    UINT16 majorImageVersion;
    UINT16 minorImageVersion;
    UINT16 majorSubsystemVersion;
    UINT16 minorSubsystemVersion;
    UINT32 win32VersionValue;
    UINT32 sizeOfImage;
    UINT32 sizeOfHeaders;
    UINT32 checkSum;
    UINT16 subsystem;
    UINT16 dllCharacteristics;
    UINT64 sizeOfStackReserve;
    UINT64 sizeOfStackCommit;
    UINT64 sizeOfHeapReserve;
    UINT64 sizeOfHeapCommit;
    UINT32 loaderFlags;
    UINT32 numberOfRvaAndSizes;
} OPTIONAL_PE_PLUS;

typedef struct {
    UINT32 virtualAddress;
    UINT32 size;
} DATA_DIRECTORY;

typedef struct {
    STANDARD_PE standardFields;
    OPTIONAL_PE winSpecific;
    DATA_DIRECTORY dataDirectories;
} PE_BASE;
typedef struct {
    STANDARD_PE_PLUS standardFields;
    OPTIONAL_PE_PLUS winSpecific;
    DATA_DIRECTORY dataDirectories;
} PE_PLUS_BASE;

typedef struct IMAGE_SECTION_HEADER {
    UINT8 name[8];
    union {
      UINT32 physicalAddress;
      UINT32 virtualSize;
    } misc;
    UINT32 virtualAddress;
    UINT32 sizeOfRawData;
    UINT32 pointerToRawData;
    UINT32 pointerToRelocations;
    UINT32 pointerToLinenumbers;
    UINT16 numberOfRelocations;
    UINT16 numberOfLinenumbers;
    UINT32 characteristics;
} IMAGE_SECTION_HEADER;
typedef struct COFF_HEADER{
    UINT16 machine;
    UINT16 numberOfSections;
    UINT32 timeDateStamp;
    UINT32 pointerToSymbolTable;
    UINT32 numberOfSymbols;
    UINT16 sizeOfOptionalHeader;
    UINT16 characteristics;
} COFF_HEADER;
typedef struct IMAGE_DOS_HEADER{
    UINT16 magic;
    UINT16 cblp;
    UINT16 cp;
    UINT16 crlc;
    UINT16 cparhdr;
    UINT16 minAlloc;
    UINT16 maxAlloc;
    UINT16 ss;
    UINT16 sp;
    UINT16 csum;
    UINT16 ip;
    UINT16 cs;
    UINT16 lfarlc;
    UINT16 ovno;
    UINT16 res[4];
    UINT16 oemId;
    UINT16 oemInfo;
    UINT16 res2[10];
    UINT32 lfaNew;
} IMAGE_DOS_HEADER;
typedef struct IMAGE_PE_HEADER{
    UINT32 signature;
    COFF_HEADER coffHeader;
    PE_BASE peBase;
} IMAGE_PE_HEADER;

#pragma pack(pop)

UINT16 GetPeVersion(VOID* buffer);
UINT32 GetRawEntryPointOffset(VOID* buffer);
UINT32 GetVirutalEntryPointOffset(VOID* buffer);
