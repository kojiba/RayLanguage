/**
 * RByteOperations.h
 * Realization of some operations on bytes array, like getSubArray, etc.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2.10.14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_BYTE_OPERATIONS_H__
#define __R_BYTE_OPERATIONS_H__

#include <RArray.h>
#include <RCString.h>

class(RByteArray)
    byte   *array;
    size_t  size;
endOf(RByteArray)

// Memory operations
void           Xor (      pointer data,          // buffered xor, operands not bytes, but size_t (architects speed-up)
                    const pointer key,
                          size_t  sizeOfData,
                          size_t  sizeOfKey);

void           Add_8 (       pointer data,       // addition by module 8
                       const pointer key,
                             size_t  sizeOfData,
                             size_t  sizeOfKey);

void           Sub_8 (       pointer data,       // subtraction by module 8
                       const pointer key,
                             size_t  sizeOfData,
                             size_t  sizeOfKey);

// Basics
byte*          flushAllToByte             (      pointer array, size_t size, byte symbol); // returns reference
void           printByteArrayInHex        (const byte *array,   size_t size);
byte*          getByteArrayCopy           (const byte *array,   size_t size);
byte*          getSubArray                (const byte *array,   RRange range );            // sub-array copy
RByteArray*    getSubArrayToFirstSymbol   (const byte *array,   size_t size, byte symbol); // sub-array copy, or nil
RArray*        getArraysSeparatedBySymbol (const byte *array,   size_t size, byte symbol); // size-to-fit RArray with set-upd delegates, or nil

// RByteArray
constructor (RByteArray), size_t size);
destructor  (RByteArray);
printer     (RByteArray);

method(RByteArray*, flushAllToByte, RByteArray),    byte symbol);
method(RByteArray*, copy,           RByteArray));
method(RByteArray*, fromRCString,   RByteArray),    RCString *string); // not sets size, only copy bytes, returns self


#define makeRByteArray(size)           $(nil, c(RByteArray)), size)
#define makeFlushedBytes(size, symbol) flushAllToByte(RAlloc(size), size, symbol)
#define RBfromRCS(string)              $(makeRByteArray(string->size), m(fromRCString, RByteArray)), string)

#endif /*__R_BYTE_OPERATIONS_H__*/