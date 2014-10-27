/**
 * RByteOperations.c
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

#include "RByteOperations.h"
#include "../RClassTable/RClassTable.h"

#pragma mark Basics

byte* makeByteArray(size_t size) {
    return RAlloc(size);
}

byte* flushAllToByte(byte *array, size_t size, byte symbol) {
    size_t iterator;
    forAll(iterator, size) {
        array[iterator] = symbol;
    }
    return array;
}

void printByteArrayInHex(const byte *array, size_t size) {
    if(array != NULL) {
        size_t iterator;
        forAll(iterator, size) {
            if (iterator % 20 == 0 && iterator != 0) {
                RPrintf("\n");
            }
            RPrintf("%02x ", array[iterator]);
        }
        RPrintf("\n");
    } else {
        RPrintf("Empty byte array.\n");
    }
}

byte* getByteArrayCopy(const byte *array, size_t size) {
    byte *arrayCopy = RAlloc(size);
    RMemMove(arrayCopy, array, size);
    return arrayCopy;
}

byte* getSubArray(const byte *array, RRange range) {
    byte *subArray = RAlloc(range.count);
    RMemMove(subArray, &array[range.from], range.count);
    return subArray;
}

RByteArray* getSubArrayToFirstSymbol(const byte *array, size_t size, byte symbol) {
    RByteArray *result   = NULL;
    size_t    iterator = 0;

    while(array[iterator] != symbol
            && iterator < size) {
        ++iterator;
    }

    if(iterator != 0) {
        result = $(NULL, c(RByteArray)), iterator);
        RMemMove(result->array, array, iterator);
    }
    return result;
}

RArray* getArraysSeparatedBySymbol(const byte *array, size_t size, byte symbol) {
    RByteArray         *subArray    = NULL;
    RArray             *resultArray = NULL;
    byte               *tempArray   = array;

    subArray = getSubArrayToFirstSymbol(array, size, symbol);

    if(subArray != NULL) {
        resultArray = makeRArray();
        // init RArray
        resultArray->destructorDelegate = d(RByteArray);
        resultArray->printerDelegate    = p(RByteArray);
    }

    while(subArray != NULL && size > 0) {
        addObjectToRA(resultArray, subArray);
        size = size - subArray->size - 1;
        tempArray += subArray->size + 1;
        subArray = getSubArrayToFirstSymbol(tempArray, size, symbol);
    }

    // size to fit RArray
    if(resultArray != NULL) {
        $(resultArray, m(sizeToFit, RArray)) );
    }
    return resultArray;
}

#pragma mark RByteArray

constructor(RByteArray), size_t size) {
    object = allocator(RByteArray);
    if(object != NULL) {
        object->array   = makeByteArray(size);
        object->size    = size;
        object->classId = registerClassOnce(toString(RByteArray));
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
    copy->array      = getByteArrayCopy(object->array, object->size);
    copy->size       = object->size;
    return copy;
}