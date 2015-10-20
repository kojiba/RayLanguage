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

// Memory operations
void           Xor (      byte* data, const byte* key,       size_t  sizeOfData,        size_t  sizeOfKey);

byte*  flushAllToByte(byte *array, size_t size, byte symbol); // returns reference
size_t compareMemory(const byte *array, const byte *toCompare, size_t size);


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

#define isMemEqual(one, another, size) (compareMemory(one, another, size) == 0)
#define ifMemEqual(one, another, size) if(compareMemory(one, another, size))

// Basics
void           printByteArrayInHexWithScreenSize(const byte *array,   size_t size, size_t screenSize);
extern void    printByteArrayInHex        (const byte *array,   size_t size);
void           printByteArrayInBin        (const byte *array,   size_t size);

byte*          getByteArrayCopy           (const byte *array,   size_t size);

size_t indexOfFirstByte(const byte *array, size_t size, byte symbol);
size_t indexOfFirstSubArray(const byte *array, size_t size, const byte *sub, size_t subSize);
extern rbool isContainsSubArray(const byte *array, size_t size, const byte *sub, size_t subSize);

size_t numberOfBytes(const byte *array, size_t size, byte symbol);
size_t numberOfSubArrays(const byte *array, size_t size, const byte *sub, size_t subSize);

byte* deleteInRange(byte *array, size_t *size, RRange range);
extern byte* deleteAtIndex(byte *array, size_t *size, size_t index);

byte* deleteAllBytes(byte *array, size_t *size, byte symbol);
byte* deleteRepetitionsOfByte(byte *array, size_t *size, const byte symbol);

byte* deleteAllSubArrays(byte *array, size_t *size, const byte *sub, size_t subSize);
byte* deleteRepetitionsOfSubArray(byte *array, size_t *size, const byte *sub, size_t subSize);

extern byte* trimTail(byte *array, size_t *size, size_t count);
extern byte* trimHead(byte *array, size_t *size, size_t count);

extern byte* trimAfterSubArray(byte *array, size_t *size, const byte *sub, size_t subSize);
extern byte* trimBeforeSubArray(byte *array, size_t *size,const byte *sub, size_t subSize);

byte* replaceByteWithByte(byte *array, size_t size, byte toReplace, byte replacer);
byte* replaceBytesWithBytes(byte *array, size_t *size, byte *toReplace, size_t toReplaceSize, byte *replacer, size_t replacerSize);

byte* insertSubArray(byte *array, size_t *size, const byte *sub, size_t subSize, size_t place);

byte* subArrayInRange(const byte *array, size_t size, RRange range);
byte* setSubArrayInRange(byte *array, size_t size, const byte *sub, size_t subSize, RRange range);

#endif /*__R_BYTE_OPERATIONS_H__*/