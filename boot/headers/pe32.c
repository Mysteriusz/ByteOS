#include "pe32.h"

UINT16 GetPeVersion(VOID* buffer) {
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER* peHeader = (IMAGE_PE_HEADER*)((UINT8*)buffer + dosHeader->lfaNew);
    return peHeader->peBase.standardFields.magic;
}
UINT32 GetRawEntryPointOffset(VOID* buffer) {
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER* peHeader = (IMAGE_PE_HEADER*)((UINT8*)buffer + dosHeader->lfaNew);

    IMAGE_SECTION_HEADER* sectionHeader = (IMAGE_SECTION_HEADER*)((UINT8*)&peHeader->peBase + peHeader->coffHeader.sizeOfOptionalHeader);

    UINT32 offset = peHeader->peBase.standardFields.addressOfEntryPoint - sectionHeader->virtualAddress;

    return sectionHeader->pointerToRawData + offset;
}
UINT32 GetVirutalEntryPointOffset(VOID* buffer) {
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)buffer;
    IMAGE_PE_HEADER* peHeader = (IMAGE_PE_HEADER*)((UINT8*)buffer + dosHeader->lfaNew);
    return peHeader->peBase.standardFields.addressOfEntryPoint;
}

