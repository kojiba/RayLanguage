/**
 * @file RByteOperations.c
 * @brief Implementation of some operations on bytes array, like getSubArray, etc.
 * @date 2.10.14 Kharkiv
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RByteOperations.h"

byte* makeByteArray(uint64_t size) {
    byte* array;
    array = RAlloc(size);
    return array;
}

void printByteArrayInHex(const byte *array, uint64_t size) {
    uint64_t iterator;
    forAll(iterator, size) {
        if(iterator % 80 == 0) {
            RPrintf("\n");
        }
        RPrintf("%02x ", array[iterator]);
    }
}

byte* getByteArrayCopy(const byte *array, uint64_t size) {
    byte *arrayCopy = RAlloc(size);
    RMemMove(arrayCopy, array, size);
    return arrayCopy;
}

byte* getSubArray(const byte *array, RRange range) {
    byte *subArray = RAlloc(range.count);
    RMemMove(subArray, array[range.from], range.count);
    return subArray;
}

byte* getSubArrayToFirstSymbol(const byte *array, byte symbol) {
    byte *subArray;
    uint64_t iterator = 0;
    while(array[iterator] != symbol) {
        ++iterator;
    }
    subArray = RAlloc(iterator);
    RMemMove(subArray, array, iterator);
    return subArray;
}
