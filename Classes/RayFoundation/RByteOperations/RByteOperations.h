#ifndef __R_BYTE_OPERATIONS_H__
#define __R_BYTE_OPERATIONS_H__

#include "../RayFoundation.h"
#include "../RCString/RCString.h"

byte* makeByteArray            (uint64_t size);
void  printByteArrayInHex      (const byte *array, uint64_t size);
byte* getByteArrayCopy         (const byte *array, uint64_t size);
byte* getSubArray              (const byte *array, RRange range);
byte* getSubArrayToFirstSymbol (const byte *array, byte symbol);

#endif __R_BYTE_OPERATIONS_H__