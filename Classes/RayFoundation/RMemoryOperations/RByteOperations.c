/**
 * RByteOperations.c
 * Realization of some operations on bytes array, like getSubArray, etc.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 10/2/14 2014 Ukraine Kharkiv
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

#define byteToBinary(byte)   \
      (byte & 0x80 ? 1 : 0), \
      (byte & 0x40 ? 1 : 0), \
      (byte & 0x20 ? 1 : 0), \
      (byte & 0x10 ? 1 : 0), \
      (byte & 0x08 ? 1 : 0), \
      (byte & 0x04 ? 1 : 0), \
      (byte & 0x02 ? 1 : 0), \
      (byte & 0x01 ? 1 : 0)


void Xor(      byte   *data,
         const byte   *key,
               size_t  sizeOfData,
               size_t  sizeOfKey) {
    size_t iterator;
    forAll(iterator, sizeOfData) {
        data[iterator] ^= key[iterator % sizeOfKey];
    }
}

byte* flushAllToByte(byte *array, size_t size, byte symbol) {
    size_t iterator;
    if(size <= sizeof(size_t)) {
        forAll(iterator, size) {
            array[iterator] = symbol;
        }
    } else {
        size_t footer = symbol;

        // create size_t footer of duplications
        forAll(iterator, sizeof(size_t)) {
            footer <<= 8;
            footer += symbol;
        }

        // fast flushing
        forAll(iterator, size / sizeof(size_t)) {
            ((size_t*)array)[iterator] = footer;
        }
        iterator *= sizeof(size_t);
        // byte-to-byte
        return flushAllToByte(array + iterator, size - iterator, symbol);
    }
    return array;
}

size_t compareMemory(const byte *array, const byte *toCompare, size_t size) {
    size_t iterator;
    if(size <= sizeof(size_t)) {
        forAll(iterator, size) {
            if(array[iterator] != toCompare[iterator]) {
                return iterator;
            }
        }
        return REquals;
    } else {
        // fast compare
        forAll(iterator, size / sizeof(size_t)) {
            if(((size_t*)array)[iterator] != ((size_t*)toCompare)[iterator]) {
                iterator *= sizeof(size_t);
                return iterator + compareMemory(array + iterator, toCompare + iterator, sizeof(size_t));
            }
        }

        for(iterator *= sizeof(size_t); iterator < size; ++iterator){
            if(array[iterator] != toCompare[iterator]) {
                return iterator;
            }
        }

        return REquals;

    }
}

inline void printByteArrayInHex(const byte *array, size_t size) {
    printByteArrayInHexWithScreenSize(array, size, 32);
}

void printByteArrayInHexWithScreenSizeFile(const byte *array, size_t size, size_t screenSize, FILE *file) {
    size_t iterator;
    forAll(iterator, size) {
        if (iterator % screenSize == 0 && iterator != 0) {
            RFPrintf(file, "\n");
        }
        RFPrintf(file, "%02X ", array[iterator]);
    }
    RFPrintf(file, "\n");
}

inline void printByteArrayInHexWithScreenSize(const byte *array, size_t size, size_t screenSize) {
    printByteArrayInHexWithScreenSizeFile(array, size, screenSize, stdout);
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
        RMemCpy(arrayCopy, array, size);
    }
    return arrayCopy;
}

size_t indexOfFirstByte(const byte *array, size_t size, byte symbol) {
    size_t iterator;
    forAll(iterator, size) {
        if (array[iterator] == symbol) {
            return iterator;
        }
    }
    return RNotFound;
}

size_t indexOfFirstByteFromSet(const byte *array, size_t size, byte *set, size_t setSize) {
    size_t iterator;
    size_t setIterator;
    forAll(iterator, size) {
        forAll(setIterator, setSize) {
            if (array[iterator] == set[setIterator]) {
                return iterator;
            }
        }
    }
    return RNotFound;
}

size_t indexOfFirstSubArray(const byte *array, size_t size, const byte *sub, size_t subSize) {
    size_t compared;
    size_t storedIterator = 0;

    if(size >= subSize) {
        // search for first symbol
        size_t iterator = indexOfFirstByte(array, size, sub[0]);
        while (iterator != RNotFound
               && (size - iterator) >= (subSize - 1) ) {
            // compare others
            array += iterator + 1;
            size  -= iterator + 1;
            storedIterator += iterator;

            compared = compareMemory(array, sub + 1, subSize - 1);

            if (compared != REquals) {
                         array += compared;
                          size -= compared;
                storedIterator += compared + 1;
                iterator = indexOfFirstByte(array, size, sub[0]);

            } else {
                return storedIterator;
            }

        }
    }
    return RNotFound;
}

inline rbool isContainsByte(const byte *array, size_t size, byte symbol) {
    return indexOfFirstByte(array, size, symbol) == RNotFound ? no : yes;
}

inline rbool isContainsSubArray(const byte *array, size_t size, const byte *sub, size_t subSize) {
    return indexOfFirstSubArray(array, size, sub, subSize) == RNotFound ? no : yes;
}

rbool isStartsOnArray(const byte *array, size_t size, const byte *sub, size_t subSize) {
    if(subSize <= size) {
        return compareMemory(array, sub, subSize) == REquals ? yes : no;
    } else {
        return no;
    }
}

rbool isEndsOnArray(const byte *array, size_t size, const byte *sub, size_t subSize) {
    if(subSize <= size) {
        return compareMemory(array + size - subSize, sub, subSize) == REquals ? yes : no;
    } else {
        return no;
    }
}

size_t numberOfBytes(const byte *array, size_t size, byte symbol) {
    size_t reps = 0;
    size_t iterator;

    forAll(iterator, size) {
        if(array[iterator] == symbol) {
            ++reps;
        }
    }
    return reps;
}

size_t numberOfSubArrays(const byte *array, size_t size, const byte *sub, size_t subSize) {
    if(subSize == 1) {
        return numberOfBytes(array, size, sub[0]);
    } else {
            size_t iterator;
            size_t count    = 0;
            forAll(iterator, size - subSize + 1) {
                // search for first symbol
                if(array[iterator] == sub[0]) {
                    // compare others
                    ifMemEqual(array + iterator + 1, sub + 1, subSize - 1) {
                        ++count;
                        iterator += subSize - 2;
                    }
                }
            }
            return count;
    }
}

byte* deleteInRange(byte *array, size_t *size, RRange range) {
    size_t tempSize = *size;
    if(range.size != 0
       && ((range.start + range.size) <= tempSize)) {
        RMemMove(array + range.start,
                 array + range.start + range.size,
                 tempSize - range.size - range.start + 1);
        tempSize -= range.size;
        *size = tempSize;
    }
    return array;
}

inline byte* deleteAtIndex(byte *array, size_t *size, size_t index) {
    return deleteInRange(array, size, makeRRange(index, 1));
}

byte* deleteAllBytes(byte *array, size_t *size, byte symbol) {
    size_t storedIndex = 0;
    size_t index = indexOfFirstByte(array, *size, symbol);
    while(index != RNotFound) {
        deleteInRange(array, size, makeRRange(storedIndex + index, 1));
        storedIndex += index;
        index = indexOfFirstByte(array + index, *size, symbol);
    }
    return array;
}


byte* deleteRepetitionsOfByte(byte *array, size_t *size, const byte symbol) {
    size_t iterator;
    size_t length;
    forAll(iterator, *size - 1) {

        // search first occurrence
        if(array[iterator] == symbol) {

            // find length of repetition
            for(length = 0; length < *size - iterator; ++length) {
                if(array[length + iterator + 1] != symbol){
                    break;
                }
            }

            // delete if length > 0
            if(length > 0) {
                deleteInRange(array, size, makeRRange(iterator + 1, length));
            }
        }
    }
    return array;
}

byte* deleteAllSubArrays(byte *array, size_t *size, const byte *sub, size_t subSize) {
    byte *stored = array;
    size_t tempSize = *size;
    size_t index = indexOfFirstSubArray(array, tempSize, sub, subSize);

    while(index != RNotFound
            && tempSize >= subSize) {

        array += index;
        tempSize -= index;
        deleteInRange(array, &tempSize, makeRRange(0, subSize));
        *size -= subSize;

        index = indexOfFirstSubArray(array, tempSize, sub, subSize);
    }
    return stored;
}

byte* deleteRepetitionsOfSubArray(byte *array, size_t *size, const byte *sub, size_t subSize) {
    if(*size >= subSize * 2) {
        size_t iterator;
        size_t count;
        size_t tempSize = *size;

        size_t index = indexOfFirstSubArray(array, *size, sub, subSize);
        while (index != RNotFound
               && tempSize >= subSize) {

            array += index + subSize;
            tempSize -= index + subSize;
            count = 0;

            iterator = indexOfFirstSubArray(array, tempSize, sub, subSize);
            if(iterator != RNotFound) {

                while(iterator == 0
                      && tempSize >= subSize) {
                    array += subSize;
                    tempSize -= subSize;

                    ++count;
                    iterator = indexOfFirstSubArray(array, tempSize, sub, subSize);
                }

                if (count > 0) {
                    deleteInRange(array - subSize * (count + 1), size, makeRRange(index + subSize, subSize * count));
                    tempSize -= subSize * count;
                } else {
                    index = iterator;
                }


            }
        }
    }
    return array;
}

inline byte* trimTail(byte *array, size_t *size, size_t count) {
    return deleteInRange(array, size, makeRRange(0, count));
}

inline byte* trimHead(byte *array, size_t *size, size_t count) {
    return deleteInRange(array, size, makeRRange(*size - count, count));
}

inline byte* trimAfterSubArray(byte *array, size_t *size, const byte *sub, size_t subSize) {
    size_t index = indexOfFirstSubArray(array, *size, sub, subSize);
    if(index != RNotFound) {
        deleteInRange(array, size, makeRRange(index, *size - index));
    }
    return array;
}
inline byte* trimBeforeSubArray(byte *array, size_t *size, const byte *sub, size_t subSize) {
    size_t index = indexOfFirstSubArray(array, *size, sub, subSize);
    if(index != RNotFound) {
        deleteInRange(array, size, makeRRange(0, index));
    }
    return array;
}

byte* replaceByteWithByte(byte *array, size_t size, byte toReplace, byte replacer) {
    size_t iterator = indexOfFirstByte(array, size, toReplace);
    while(iterator != RNotFound) {
        array[iterator] = replacer;
        iterator = indexOfFirstByte(array + iterator, size - iterator, toReplace);
    }
    return array;
}

byte* replaceBytesWithBytes(byte *array, size_t *size, byte *toReplace, size_t toReplaceSize, byte *replacer, size_t replacerSize) {
    size_t index = indexOfFirstSubArray(array, *size, toReplace, toReplaceSize);

    while(index != RNotFound) {
        array = insertSubArray(array, size, replacer, replacerSize, index);
        array = deleteInRange(array, size, makeRRange(index + replacerSize, toReplaceSize));
        index = indexOfFirstSubArray(array + index, *size, toReplace, toReplaceSize);
    }
    return array;
}

byte* insertSubArray(byte *array, size_t *size, const byte *sub, size_t subSize, size_t place) {
    if(place <= *size) {
        array = RReAlloc(array, arraySize(byte, *size + subSize));
        RMemMove(array + place + subSize, array + place, *size - place);
        RMemCpy(array + place, sub, subSize);
        *size += subSize;
    }
    return array;
}

byte* subArrayInRange(const byte *array, size_t size, RRange range) {
    if(range.size + range.start <= size) {
        byte *result = arrayAllocator(byte, range.size);
        if(result != nil) {
            RMemCpy(result, array + range.start, range.size);
        }
        return result;
    }
    return nil;
}

byte* setSubArrayInRange(byte *array, size_t size, const byte *sub, size_t subSize, RRange range) {
    if(range.size + range.start <= size
            && range.size <= subSize) {
        RMemCpy(array + range.start, sub, range.size);
    }
    return array;
}

void appendArray(byte **array, size_t *size, const byte *sub, size_t subSize) {
    byte *temp = *array;
    temp = RReAlloc(temp, arraySize(byte, *size + subSize));
    if(temp != nil) {
        memcpy(temp + *size, sub, subSize);
        *size += subSize;
        *array = temp;
    } elseError( RError("appendArray. Realloc error.", array) );
}