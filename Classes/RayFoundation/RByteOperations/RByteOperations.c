/**
 * @file RByteOperations.c
 * @brief Implementation of some operations on bytes array, like getSubArray, etc.
 * @date 2.10.14 Kharkiv
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RByteOperations.h"

#pragma mark Basics

byte* makeByteArray(uint64_t size) {
    return RAlloc(size);
}

byte* flushAllToByte(byte *array, uint64_t size, byte symbol) {
    uint64_t iterator;
    forAll(iterator, size) {
        array[iterator] = symbol;
    }
    return array;
}

void printByteArrayInHex(const byte *array, uint64_t size) {
    if(array != NULL) {
        uint64_t iterator;
        forAll(iterator, size) {
            if (iterator % 20 == 0) {
                RPrintf("\n");
            }
            RPrintf("%02x ", array[iterator]);
        }
    } else {
        RPrintf("Empty byte array.\n");
    }
}

byte* getByteArrayCopy(const byte *array, uint64_t size) {
    byte *arrayCopy = RAlloc(size);
    RMemMove(arrayCopy, array, size);
    return arrayCopy;
}

byte* getSubArray(const byte *array, RRange range) {
    byte *subArray = RAlloc(range.count);
    RMemMove(subArray, &array[range.from], range.count);
    return subArray;
}

RByteArray* getSubArrayToFirstSymbol(const byte *array, uint64_t size, byte symbol) {
    RByteArray *result   = NULL;
    uint64_t    iterator = 0;
    byte       *subArray;

    while(array[iterator] != symbol
            && iterator < size) {
        ++iterator;
    }

    if(iterator != 0) {
        result = allocator(RByteArray);
        result->size = iterator;
        subArray = RAlloc(iterator);
        RMemMove(subArray, array, iterator);
        result->array = subArray;
    }
    return result;
}

RArray* getArraySeparatedBySymbol(const byte *array, uint64_t size, byte symbol) {
    RByteArray         *subArray    = NULL;
    RArray             *resultArray = makeRArray();

    // init RArray
    resultArray->destructorDelegate = d(RByteArray);
    resultArray->printerDelegate = p(RByteArray);

    subArray = getSubArrayToFirstSymbol(array, size, symbol);
    while(subArray != NULL) {
        addObjectToRA(resultArray, subArray);
        subArray = getSubArrayToFirstSymbol(array, size, symbol);
    }



    return resultArray;
}

#pragma mark RByteArray

constructor(RByteArray), uint64_t size) {
    object = allocator(RByteArray);
    if(object != NULL) {
        object->array = makeByteArray(size);
        object->size = size;
    }
    return object;
}

destructor(RByteArray) {
    if(object != NULL) {
        RFree(object->array);
    } else {
        RPrintf("Warning. RBA. Destructor of NULL.\n");
    }
}

printer(RByteArray) {
    printByteArrayInHex(object->array, object->size);
}

RByteArray* flushAllToByteRByteArray(RByteArray *array, byte symbol) {
    array->array = flushAllToByte(array->array, array->size, symbol);
    return array;
}

method (RByteArray*, copy, RByteArray)) {
    RByteArray *copy = allocator(RByteArray);
    copy->array = getByteArrayCopy(object->array, object->size);
    copy->size = object->size;
    return copy;
}