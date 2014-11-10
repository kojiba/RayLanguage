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

void Xor(const pointer data,
         const pointer key,
               size_t  sizeOfData,
               size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] ^ ((byte*)key)[iterator % sizeOfKey];
    }
}

void Add(const pointer data,
         const pointer key,
               size_t  sizeOfData,
               size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] + ((byte*)key)[iterator % sizeOfKey];
    }
}

void Sub(const pointer data,
         const pointer key,
               size_t  sizeOfData,
               size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] - ((byte*)key)[iterator % sizeOfKey];
    }
}

#pragma mark Basics

byte* flushAllToByte(byte *array, size_t size, byte symbol) {
    size_t iterator;
    forAll(iterator, size) {
        array[iterator] = symbol;
    }
    return array;
}

void printByteArrayInHex(const byte *array, size_t size) {
    size_t iterator;
    forAll(iterator, size) {
        if (iterator % 20 == 0 && iterator != 0) {
            RPrintf("\n");
        }
        RPrintf("%02X ", array[iterator]);
    }
    RPrintf("\n");
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
    RByteArray *result   = nil;
    size_t    iterator = 0;

    while(array[iterator] != symbol
            && iterator < size) {
        ++iterator;
    }

    if(iterator != 0) {
        result = $(nil, c(RByteArray)), iterator);
        RMemMove(result->array, array, iterator);
    }
    return result;
}

RArray* getArraysSeparatedBySymbol(const byte *array, size_t size, byte symbol) {
    RByteArray         *subArray    = nil;
    RArray             *resultArray = nil;
    byte               *tempArray   = array;

    subArray = getSubArrayToFirstSymbol(array, size, symbol);

    if(subArray != nil) {
        resultArray = makeRArray();
        // init RArray
        resultArray->destructorDelegate = d(RByteArray);
        resultArray->printerDelegate    = p(RByteArray);
    }

    while(subArray != nil && size > 0) {
        addObjectToRA(resultArray, subArray);
        size = size - subArray->size - 1;
        tempArray += subArray->size + 1;
        subArray = getSubArrayToFirstSymbol(tempArray, size, symbol);
    }

    // size to fit RArray
    if(resultArray != nil) {
        $(resultArray, m(sizeToFit, RArray)) );
    }
    return resultArray;
}

#pragma mark RByteArray

constructor(RByteArray), size_t size) {
    object = allocator(RByteArray);
    if(object != nil) {
        object->array = RAlloc(size * sizeof(byte));
        if(object->array != nil) {
            object->classId = registerClassOnce(toString(RByteArray));
            object->size    = size;
        } else {
            RError("RBA. Array allocation failed", object);
            return nil;
        }
    }
    return object;
}

destructor(RByteArray) {
    deallocator(object->array);
}

printer(RByteArray) {
    RPrintf("%s object - %p {\n", toString(RByteArray), object);
    printByteArrayInHex(object->array, object->size);
    RPrintf("} - %p \n\n", object);
}

method(RByteArray*, flushAllToByte, RByteArray), byte symbol) {
    object->array = flushAllToByte(object->array, object->size, symbol);
    return object;
}

method(RByteArray*, copy, RByteArray)) {
    RByteArray *copy = allocator(RByteArray);
    copy->array      = getByteArrayCopy(object->array, object->size);
    copy->size       = object->size;
    return copy;
}

method(RByteArray*, fromRCString, RByteArray), RCString *string) {
    if(object->size <= string->size && object->size != 0) {
        RMemMove(object->array, string->baseString, string->size);
        return object;
    }
    return nil;
}