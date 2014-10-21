/**
 * @file RByteOperations.h
 * @brief Realization of some operations on bytes array, like getSubArray, etc.
 * @date 2.10.14 Kharkiv
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_BYTE_OPERATIONS_H__
#define __R_BYTE_OPERATIONS_H__

#include "../RayFoundation.h"
#include "../RCString/RCString.h"
#include "../RArray/RArray.h"

class(RByteArray)
    byte    *array;
    uint64_t size;
endOf(RByteArray)

// basics
byte*          makeByteArray              (uint64_t size);
byte*          flushAllToByte             (byte *array,       uint64_t size, byte symbol); // not copy
void           printByteArrayInHex        (const byte *array, uint64_t size);
byte*          getByteArrayCopy           (const byte *array, uint64_t size);
byte*          getSubArray                (const byte *array, RRange range );              // sub-array copy
RByteArray*    getSubArrayToFirstSymbol   (const byte *array, uint64_t size, byte symbol); // sub-array copy, or NULL
RArray*        getArraysSeparatedBySymbol (const byte *array, uint64_t size, byte symbol); // RArray with set-upd delegates, or NULL

// RByteArray
constructor (RByteArray), uint64_t size);
destructor  (RByteArray);
printer     (RByteArray);

method(RByteArray*, flushAllToByte, RByteArray),    byte symbol);
method(RByteArray*, copy,           RByteArray));


#define makeRByteArray(size)         $(NULL, c(RByteArray)), size)
#define makeFlushedBytes(size, symbol) flushAllToByte(makeByteArray(size), size, symbol)

#endif /*__R_BYTE_OPERATIONS_H__*/