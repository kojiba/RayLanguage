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

#include "RayFoundation/RContainers/RArray.h"
#include "RayFoundation/RCString/RCString.h"

class(RByteArray)
    byte   *array;
    size_t  size;
endOf(RByteArray)

// Memory operations
void           Xor (      pointer data,
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

#define rotateLeftU(data,  shift, capasity) (((data) << shift) | ((data) >> (capasity - shift)))
#define rotateRightU(data, shift, capasity) (((data) >> shift) | ((data) << (capasity - shift)))

#define rotateLeft64(data,  shift) rotateLeftU(data, shift, 64)
#define rotateRight64(data, shift) rotateRightU(data, shift, 64)

#define rotateLeft32(data,  shift) rotateLeftU(data, shift, 32)
#define rotateRight32(data, shift) rotateRightU(data, shift, 32)

#define rotateLeft16(data,  shift) rotateLeftU(data, shift, 16)
#define rotateRight16(data, shift) rotateRightU(data, shift, 16)

#define rotateLeft8(data,  shift) rotateLeftU(data, shift, 8)
#define rotateRight8(data, shift) rotateRightU(data, shift, 8)

#define isMemEqual(one, another, size) (RMemCmp(one, another, size) == 0)
#define ifMemEqual(one, another, size) if(isMemEqual(one, another, size))

// Basics
byte*          flushAllToByte             (   pointer  array,   size_t size, byte symbol); // returns reference
void           printByteArrayInHexWithScreenSize
                                          (const byte *array,   size_t size, size_t screenSize);
extern void    printByteArrayInHex        (const byte *array,   size_t size);
void           printByteArrayInBin        (const byte *array,   size_t size);
byte*          getByteArrayCopy           (const byte *array,   size_t size);
byte*          getSubArray                (const byte *array,   RRange range );            // sub-array copy
RByteArray*    getSubArrayToFirstSymbol   (const byte *array,   size_t size, byte symbol); // sub-array copy, or nil
RArray*        getArraysSeparatedBySymbol (const byte *array,   size_t size, byte symbol); // size-to-fit RArray with set-upd delegates, or nil

// RByteArray
RByteArray* makeRByteArray(byte *array, size_t size);
extern
constructor (RByteArray), size_t size);
destructor  (RByteArray);
printer     (RByteArray);

method(RByteArray*,      flushAllToByte, RByteArray),    byte symbol);
constMethod(RByteArray*, copy,           RByteArray));
method(RByteArray*,      fromRCString,   RByteArray),    RCString *string); // not sets size, only copy bytes, returns self

RByteArray* contentOfFile(const char *filename);

#define makeFlushedBytes(size, symbol) flushAllToByte(RAlloc(size), size, symbol)
#define RBfromRCS(string)              $(c(RByteArray)(nil, (string)->size), m(fromRCString, RByteArray)), (string))

#endif /*__R_BYTE_OPERATIONS_H__*/