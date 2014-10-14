/**
 * @file RCString.c
 * @brief Implementation of wrapper on '\0' - terminated C-strings, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include <string.h>
#include "RCString.h"

#pragma mark RRange

RRange makeRRange(uint64_t from, uint64_t count) {
    RRange range;
    range.from = from;
    range.count = count;
    return range;
}

RRange makeRRangeTo(uint64_t from, uint64_t to) {
    RRange range;
    range.from = from;
    range.count = to - from;
    return range;
}

#pragma mark RBounds

RBounds makeRBounds(char startSymbol, char endSymbol) {
    RBounds bounds;
    bounds.startSymbol = startSymbol;
    bounds.endSymbol = endSymbol;
    return bounds;
}

#pragma mark Basics

uint64_t indexOfFirstCharacterCString(const char *string, uint64_t size, char character) {
    uint64_t iterator = 0;
    while(iterator < size) {
        if(string[iterator] == character) {
            break;
        } else {
            ++iterator;
        }
    }
    return iterator;
}

uint64_t indexOfLastCharacterCString(const char *string, uint64_t size, char character) {
    uint64_t iterator = 0;
    uint64_t last = 0;
    while(iterator < size) {
        if(string[iterator] == character) {
            last = iterator;
        }
        ++iterator;
    }
    return last;
}

#pragma mark constructor - destructor - reallocation

constructor(RCString)) {
    object = allocator(RCString);
    if(object) {
        // 1 - it's for RCString
        object->classId = 1;
        object->size = 0;
    }
    return object;
}

destructor(RCString) {
    deallocator(object->baseString);
    object->size = 0;
}

method(void, flush, RCString)) {
    if(object->size != 0 && object->baseString != NULL) {
        deallocator(object->baseString);
        object->size = 0;
    }
}

#pragma mark Setters

method(RCString *, setString, RCString), const char *string) {
    if(string != NULL) {
        uint64_t stringSize = RStringLenght(string) + 1;

        // checking, if exist and size like copying
        if(object->size != 0 && object->baseString != NULL
                || object->size < stringSize) {
            deallocator(object->baseString);
            object->baseString = RAlloc(stringSize * sizeof(char));
        }

        // final copying
        object->size = stringSize;
        RMemCpy(object->baseString, string, object->size);
        --object->size;
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

method(RCString *, setConstantString, RCString), char *string) {
    if(string != NULL) {
        // copy pointer, and compute length
        object->baseString = string;
        object->size       = RStringLenght(string);
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

#pragma mark Options

method(uint64_t, numberOfRepetitions, RCString), char character) {
    uint64_t reps     = 0;
    uint64_t iterator;

    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            ++reps;
        }
    }
    return reps;
}

method(RCString *, deleteAllCharacters, RCString), char character) {
    uint64_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            RMemCpy(object->baseString + iterator, object->baseString + iterator + 1, object->size + 1 - iterator);
            --iterator;
            --object->size;
        }
    }
    return object;
}

method(RCString *, deleteAllSubstrings, RCString), const RCString *substring) {
    uint64_t iterator;
    uint64_t inner;
    byte flag = 1;

    if(substring->size != 0
            || substring->baseString == NULL) {
        forAll(iterator, object->size - substring->size) {

            // compare to substring
            if(object->baseString[iterator] == substring->baseString[0]) {
                for(inner = 1; inner < substring->size; ++inner) {

                    if(object->baseString[iterator + 1] != substring->baseString[inner]) {
                        flag = 0;
                        break;
                    }

                }
            } else {
                flag = 0;
            }

            // moving
            if(flag == 1) {
                RMemCpy(object->baseString + iterator, object->baseString + iterator + substring->size, object->size + 1 - iterator - substring->size);
                --iterator;
                object->size -= substring->size;
            } else {
                flag = 1;
            }
        }
        return object;
    } else {
        RPrintf("Warning. RCS. Substring size is 0, or, substring is NULL.\n");
    }
    return NULL;
}

method(RCString *, deleteCharacterAt, RCString), uint64_t index) {
    if(index > object->size) {
        RPrintf("Error. RCS. Bad index!");
    } else {
        RMemMove(object->baseString + index, object->baseString + index + 1, object->size - index);
    }
    return object;
}

method(void, deleteInRange, RCString), RRange range) {
    if(range.count != 0 && range.from < object->size) {
        RMemMove(object->baseString + range.from, object->baseString + range.from + range.count, object->size - range.count - range.from + 1);
        object->size -= range.count;
        object->baseString[object->size + 1] = 0;
    } else {
        RPrintf("ERRROR. RCS. deleteInRange, bad range, do nothing.\n");
    }
}

#pragma mark Substrings and Copies

method(RCString *, getSubstringInRange, RCString), RRange range) {
    if(range.count != 0 && range.from < object->size) {
        char *cstring = RAlloc(range.count + 1 * sizeof(char));
        RMemMove(cstring, object->baseString + range.from, range.count);
        cstring[range.count + 1] = 0;

        RCString *rcString   = $(NULL, c(RCString)) );
        rcString->size       = range.count;
        rcString->baseString = cstring;

        return rcString;
    } else {
        RPrintf("ERRROR. RCS. BAD RANGE!\n");
        return NULL;
    }

}

method(RCString *, getSubstringByBounds, RCString), RBounds bounds) {
    RCString *result;
    RRange    range;

    range.from  = indexOfFirstCharacterCString(object->baseString, object->size, bounds.startSymbol) + 1;
    range.count = indexOfLastCharacterCString (object->baseString, object->size, bounds.endSymbol) - range.from;

    result = $(object, m(getSubstringInRange, RCString)), range);
    return result;
}

method(RCString *, copy, RCString)){
    RCString *copy = $(object, m(getSubstringInRange, RCString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

method(RCompareFlags, compareWith, RCString), const RCString *checkString) {
    static uint64_t iterator;
    if(checkString == NULL || object == NULL) {
        RPrintf("Warning. RCS. One of compare strings is empty, please delete function call, or fix it.\n");
        return not_equals;
    } else {
        if (checkString == object) {
            return equals;
        } else {
            if (checkString->size == object->size) {
                forAll(iterator, object->size) {
                    if(object->baseString[iterator] != checkString->baseString[iterator]){
                        return not_equals;
                    }
                }
                return equals;
            } else {
                if(checkString->size == object->size) {
                    return shorter;
                } else {
                    return longer;
                }
            }
        }
    }
    return not_equals;
}

#pragma mark With file

method(void, fromFile, RCString), const RCString *filename) {
    FILE *file = fopen(filename->baseString, "rb");
    char *buffer;
    long fileSize;

    if(file != NULL) {
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);
        buffer = RAlloc(fileSize * (sizeof(char)));
        fread(buffer, sizeof(char), fileSize, file);
        fclose(file);
        $(object, m(setConstantString, RCString)), buffer);
    } else {
        RPrintf("Warning. RCS. Cannot open file.\n");
    }
}

printer(RCString) {
    RPrintf("%s\n", object->baseString);
}

staticMethod(char , randomCharacter, RCString)) {
    char character = ((char)rand());
    while(character < 34 ||
            character > 126) {
        character = ((char)rand());
    }
    return character;
}

staticMethod(RCString *, randomString, RCString)) {
    static uint64_t iterator;
    RCString *string = makeRCString();
    uint64_t size = ((uint64_t)rand()) % 50;
    char *cstring;

    while(size == 0) {
        size = ((uint64_t)rand()) % 50;
    }

    cstring = RAlloc(size * sizeof(char));

    forAll(iterator, size){
        cstring[iterator] = sm(randomCharacter, RCString)(NULL);
    }

    $(string, m(setConstantString, RCString)), cstring);
    return string;
}