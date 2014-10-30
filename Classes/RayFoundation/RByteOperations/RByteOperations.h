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

#include "../RContainers/RArray.h"

class(RByteArray)
    byte   *array;
    size_t  size;
endOf(RByteArray)

// Memory operations
pointer        Xor (const pointer data,       // ATTENTION! for all operations:
                    const pointer key,        // returns dynamically allocated buffer
                          size_t  sizeOfData, // or NULL if allocation fails
                          size_t  sizeOfKey); // all sizes are in bytes

pointer        Add (const pointer data,
                    const pointer key,
                          size_t  sizeOfData,
                          size_t  sizeOfKey);

pointer        Sub (const pointer data,
                    const pointer key,
                          size_t  sizeOfData,
                          size_t  sizeOfKey);

// basics
byte*          makeByteArray              (size_t size);
byte*          flushAllToByte             (byte *array,       size_t size, byte symbol); // not copy
void           printByteArrayInHex        (const byte *array, size_t size);
byte*          getByteArrayCopy           (const byte *array, size_t size);
byte*          getSubArray                (const byte *array, RRange range );            // sub-array copy
RByteArray*    getSubArrayToFirstSymbol   (const byte *array, size_t size, byte symbol); // sub-array copy, or NULL
RArray*        getArraysSeparatedBySymbol (const byte *array, size_t size, byte symbol); // size-to-fit RArray with set-upd delegates, or NULL

// RByteArray
constructor (RByteArray), size_t size);
destructor  (RByteArray);
printer     (RByteArray);

method(RByteArray*, flushAllToByte, RByteArray),    byte symbol);
method(RByteArray*, copy,           RByteArray));


#define makeRByteArray(size)         $(NULL, c(RByteArray)), size)
#define makeFlushedBytes(size, symbol) flushAllToByte(makeByteArray(size), size, symbol)

#endif /*__R_BYTE_OPERATIONS_H__*/