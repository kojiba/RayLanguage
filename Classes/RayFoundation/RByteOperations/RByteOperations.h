#ifndef __R_BYTE_OPERATIONS_H__
#define __R_BYTE_OPERATIONS_H__

#include "../RayFoundation.h"
#include "../RCString/RCString.h"

byte* makeByteArray(uint64_t size);
void printByteArrayInHex(const byte *array, uint64_t size);
byte* getByteArrayCopy(const byte *array, uint64_t size);
byte* getSubArray(const byte *array, RRange *range);

#endif __R_BYTE_OPERATIONS_H__