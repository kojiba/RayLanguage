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

#pragma mark Memory Operations

pointer Xor(const pointer data,
            const pointer key,
                  size_t  sizeOfData,
                  size_t  sizeOfKey) {
    byte  *result = RAlloc(sizeOfData);
    size_t iterator;
    if(result != nullPtr) {
        forAll(iterator, sizeOfData) {
            result[iterator] = ((byte*)data)[iterator] ^ ((byte*)key)[iterator % sizeOfKey];
        }
    }
    return result;
}

pointer Add(const pointer data,
            const pointer key,
                  size_t  sizeOfData,
                  size_t  sizeOfKey) {
    byte  *result = RAlloc(sizeOfData);
    size_t iterator;
    if(result != nullPtr) {
        forAll(iterator, sizeOfData) {
            result[iterator] = ((byte*)data)[iterator] + ((byte*)key)[iterator % sizeOfKey];
        }
    }
    return result;
}

pointer Sub(const pointer data,
        const pointer key,
        size_t  sizeOfData,
        size_t  sizeOfKey) {
    byte  *result = RAlloc(sizeOfData);
    size_t iterator;
    if(result != nullPtr) {
        forAll(iterator, sizeOfData) {
            result[iterator] = ((byte*)data)[iterator] - ((byte*)key)[iterator % sizeOfKey];
        }
    }
    return result;
}

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
    if(array != nullPtr) {
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
    RByteArray *result   = nullPtr;
    size_t    iterator = 0;

    while(array[iterator] != symbol
            && iterator < size) {
        ++iterator;
    }

    if(iterator != 0) {
        result = $(nullPtr, c(RByteArray)), iterator);
        RMemMove(result->array, array, iterator);
    }
    return result;
}

RArray* getArraysSeparatedBySymbol(const byte *array, size_t size, byte symbol) {
    RByteArray         *subArray    = nullPtr;
    RArray             *resultArray = nullPtr;
    byte               *tempArray   = array;

    subArray = getSubArrayToFirstSymbol(array, size, symbol);

    if(subArray != nullPtr) {
        resultArray = makeRArray();
        // init RArray
        resultArray->destructorDelegate = d(RByteArray);
        resultArray->printerDelegate    = p(RByteArray);
    }

    while(subArray != nullPtr && size > 0) {
        addObjectToRA(resultArray, subArray);
        size = size - subArray->size - 1;
        tempArray += subArray->size + 1;
        subArray = getSubArrayToFirstSymbol(tempArray, size, symbol);
    }

    // size to fit RArray
    if(resultArray != nullPtr) {
        $(resultArray, m(sizeToFit, RArray)) );
    }
    return resultArray;
}

#pragma mark RByteArray

constructor(RByteArray), size_t size) {
    object = allocator(RByteArray);
    if(object != nullPtr) {
        object->array   = makeByteArray(size);
        object->size    = size;
        object->classId = registerClassOnce(toString(RByteArray));
    }
    return object;
}

destructor(RByteArray) {
    if(object != nullPtr) {
        RFree(object->array);
    } else {
        RPrintf("Warning. RBA. Destructor of nullPtr.\n");
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