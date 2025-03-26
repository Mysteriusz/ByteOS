#ifndef PE32_H
#define PE32_H

#include "kernel/byteos.h"

#pragma pack(push, 1)

#define MAGIC_PE32 0x10b
#define MAGIC_PE32_PLUS 0x20b

typedef struct {
    USHORT magic;
    BYTE majorLinkerVersion;    
    BYTE minorLinkerVersion;    
    ULONG sizeOfCode;    
    ULONG sizeOfInitializedData;    
    ULONG sizeOfUninitializedData;    
    ULONG addressOfEntryPoint;    
    ULONG baseOfCode;    
    ULONG baseOfData;    
} STANDARD_PE;
typedef struct {
    USHORT magic;
    BYTE majorLinkerVersion;    
    BYTE minorLinkerVersion;    
    ULONG sizeOfCode;    
    ULONG sizeOfInitializedData;    
    ULONG sizeOfUninitializedData;    
    ULONG addressOfEntryPoint;    
    ULONG baseOfCode;   
} STANDARD_PE_PLUS;

typedef struct {
    ULONG imageBase;
    ULONG sectionAlignment;
    ULONG fileAlignment;
    USHORT majorOperatingSystemVersion;
    USHORT minorOperatingSystemVersion;
    USHORT majorImageVersion;
    USHORT minorImageVersion;
    USHORT majorSubsystemVersion;
    USHORT minorSubsystemVersion;
    ULONG win32VersionValue;
    ULONG sizeOfImage;
    ULONG sizeOfHeaders;
    ULONG checkSum;
    USHORT subsystem;
    USHORT dllCharacteristics;
    ULONG sizeOfStackReserve;
    ULONG sizeOfStackCommit;
    ULONG sizeOfHeapReserve;
    ULONG sizeOfHeapCommit;
    ULONG loaderFlags;
    ULONG numberOfRvaAndSizes;
} OPTIONAL_PE;
typedef struct {
    ULONGLONG imageBase;
    ULONG sectionAlignment;
    ULONG fileAlignment;
    USHORT majorOperatingSystemVersion;
    USHORT minorOperatingSystemVersion;
    USHORT majorImageVersion;
    USHORT minorImageVersion;
    USHORT majorSubsystemVersion;
    USHORT minorSubsystemVersion;
    ULONG win32VersionValue;
    ULONG sizeOfImage;
    ULONG sizeOfHeaders;
    ULONG checkSum;
    USHORT subsystem;
    USHORT dllCharacteristics;
    ULONGLONG sizeOfStackReserve;
    ULONGLONG sizeOfStackCommit;
    ULONGLONG sizeOfHeapReserve;
    ULONGLONG sizeOfHeapCommit;
    ULONG loaderFlags;
    ULONG numberOfRvaAndSizes;
} OPTIONAL_PE_PLUS;

typedef struct {
    ULONG virtualAddress;
    ULONG size;
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
    BYTE name[8];
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
    USHORT machine;
    USHORT numberOfSections;
    UINT32 timeDateStamp;
    UINT32 pointerToSymbolTable;
    UINT32 numberOfSymbols;
    USHORT sizeOfOptionalHeader;
    USHORT characteristics;
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

UINT16 GetPeVersion(VOID *buffer) {
    IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER *peHeader = (IMAGE_PE_HEADER*)((BYTE*)buffer + dosHeader->lfaNew);
    return peHeader->peBase.standardFields.magic;
}
UINT32 GetRawEntryPointOffset(VOID *buffer){
    IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER *peHeader = (IMAGE_PE_HEADER*)((BYTE*)buffer + dosHeader->lfaNew);

    IMAGE_SECTION_HEADER *sectionHeader = (IMAGE_SECTION_HEADER*)((BYTE*)&peHeader->peBase + peHeader->coffHeader.sizeOfOptionalHeader);

    UINT32 offset = peHeader->peBase.standardFields.addressOfEntryPoint - sectionHeader->virtualAddress;

    return sectionHeader->pointerToRawData + offset;
}
UINT32 GetVirutalEntryPointOffset(VOID *buffer){
    IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER *peHeader = (IMAGE_PE_HEADER*)((BYTE*)buffer + dosHeader->lfaNew);
    return peHeader->peBase.standardFields.addressOfEntryPoint;
}

#endif
