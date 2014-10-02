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