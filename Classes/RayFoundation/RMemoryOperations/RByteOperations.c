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

#define byteToBinaryPatern "%d%d%d%d%d%d%d%d "

#define byteToBinary(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0)

#pragma mark Memory Operations

void Xor(      pointer data,
         const pointer key,
               size_t  sizeOfData,
               size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] ^ ((byte*)key)[iterator % sizeOfKey];
    }
}

void Add_8(      pointer data,
           const pointer key,
                 size_t  sizeOfData,
                 size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] + ((byte*)key)[iterator % sizeOfKey];
    }
}

void Sub_8(      pointer data,
           const pointer key,
                 size_t  sizeOfData,
                 size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        ((byte*)data)[iterator] = ((byte*)data)[iterator] - ((byte*)key)[iterator % sizeOfKey];
    }
}

#pragma mark Basics

byte* flushAllToByte(pointer array, size_t size, byte symbol) {
    size_t iterator;
    forAll(iterator, size / sizeof(size_t)) {
        ((size_t*) array)[iterator] = symbol;
    }
    for(iterator *= sizeof(size_t); iterator < size; ++iterator){
        ((byte*) array)[iterator] = symbol;
    }
    return array;
}

inline void printByteArrayInHex(const byte *array, size_t size) {
    printByteArrayInHexWithScreenSize(array, size, 32);
}

void printByteArrayInHexWithScreenSize(const byte *array, size_t size, size_t screenSize) {
    size_t iterator;
    forAll(iterator, size) {
        if (iterator % screenSize == 0 && iterator != 0) {
            RPrintf("\n");
        }
        RPrintf("%02X ", array[iterator]);
    }
    RPrintf("\n");
}

void printByteArrayInBin(const byte *array, size_t size) {
    size_t iterator;
    forAll(iterator, size) {
        if (iterator % 8 == 0 && iterator != 0) {
            RPrintf("\n");
        }
        RPrintf(byteToBinaryPatern, byteToBinary(array[size - iterator - 1]));
    }
    RPrintf("\n");
}

byte* getByteArrayCopy(const byte *array, size_t size) {
    byte *arrayCopy = RAlloc(size);
    if(arrayCopy != nil) {
        RMemMove(arrayCopy, array, size);
    }
    return arrayCopy;
}

byte* getSubArray(const byte *array, RRange range) {
    byte *subArray = RAlloc(range.size);
    if(subArray != nil) {
        RMemMove(subArray, array + range.start, range.size);
    }
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
    byte               *tempArray   = (byte *) array;

    subArray = getSubArrayToFirstSymbol(array, size, symbol);

    if(subArray != nil) {
        resultArray = makeRArray();
        // init RArray
        resultArray->destructorDelegate = (void (*)(pointer)) d(RByteArray);
        resultArray->printerDelegate    = (void (*)(pointer)) p(RByteArray);
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

RByteArray* makeRByteArray(byte *array, size_t size) {
    RByteArray* object = allocator(RByteArray);
    if(object != nil) {
        object->array   = array;
        object->classId = 7;
        object->size    = size;
    }
    return object;
}

inline constructor(RByteArray), size_t size) {
    byte *array = RAlloc(size);
    if(array != nil) {
        return makeRByteArray(array, size);
    } elseError(
            RError("RByteArray. Array allocation failed", object)
    );
    return nil;
}

destructor(RByteArray) {
    deallocator(object->array);
}

printer(RByteArray) {
    RPrintf("%s object - %p [%lu] {\n", toString(RByteArray), object, object->size);
    printByteArrayInHex(object->array, object->size);
    RPrintf("} - %p \n\n", object);
}

method(RByteArray*, flushAllToByte, RByteArray), byte symbol) {
    object->array = flushAllToByte(object->array, object->size, symbol);
    return object;
}

constMethod(RByteArray*, copy, RByteArray)) {
    RByteArray *copy = $(nil, c(RByteArray)), object->size);
    if(copy != nil) {
        RMemCpy(copy->array, object->array, object->size);
    }
    return copy;
}

method(RByteArray*, fromRCString, RByteArray), RCString *string) {
    if(object->size <= string->size && object->size != 0) {
        RMemCpy(object->array, string->baseString, string->size);
        return object;
    }
    return nil;
}

method(RByteArray*, insertInBeginBytes, RByteArray), pointer data, size_t size) {
    object->array = RReAlloc(object->array, arraySize(byte, object->size + size));
    if(object->array != nil) {
        RMemCpy(object->array + size, object->array, object->size);
        RMemCpy(object->array, data, size);
    }
    return object;
}

method(RByteArray*, insertInBegin, RByteArray), RByteArray *array) {
    object->array = RReAlloc(object->array, arraySize(byte, object->size + array->size));
    if(object->array != nil) {
        RMemCpy(object->array + array->size, object->array, object->size);
        RMemCpy(object->array, array->array, array->size);
    }
    return object;
}

RByteArray* contentOfFile(const char *filename) {
    FILE *file = RFOpen(filename, "rb");
    byte *buffer;
    ssize_t fileSize;

    if(file != nil) {
        RFSeek(file, 0, SEEK_END);
        fileSize = RFTell(file);
        if(fileSize > 0) {
            RRewind(file);
            buffer = arrayAllocator(byte, (fileSize + 1));
            if(buffer != nil) {
                if(RFRead(buffer, sizeof(byte), (size_t) fileSize, file) > 0) {
                    RFClose(file);
                    return  makeRByteArray(buffer, (size_t) fileSize);
                } else {
                    RError2("contentOfFile. Can't read file \"%s\" of size \"%lu\".\n",
                            nil, filename, fileSize);
                    RFClose(file);
                }

            } elseError(
                    RError2("contentOfFile. Bad allocation buffer for file \"%s\" of size \"%lu\".\n",
                    nil, filename, fileSize
            ));

        } elseError(
                RError1("contentOfFile. Error read (ftell) file \"%s\".\n", nil, filename)
        );

    } elseError(
            RError1( "contentOfFile. Cannot open file \"%s\".\n", nil, filename)
    );

    return nil;
}