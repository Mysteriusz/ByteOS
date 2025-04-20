#include "byteos.h"

BT_STATUS ByteAPI Crc32Initialize();
BT_STATUS ByteAPI Crc32(IN UINT8 *byteArray, IN UINT32 length, OUT UINT32 *remainder);
