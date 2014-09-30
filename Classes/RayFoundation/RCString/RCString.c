#include <string.h>
#include <Foundation/Foundation.h>
#include "RCString.h"

RRange* makeRRange(uint64_t from, uint64_t count){
    RRange *range = allocator(RRange);
    range->from = from;
    range->count = count;
    return range;
}

RRange* makeRRangeTo(uint64_t from, uint64_t to){
    RRange *range = allocator(RRange);
    range->from = from;
    range->count = to - from;
    return range;
}

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
}

method(RCString *, setString, RCString), char *string) {
    if(string != NULL) {
        uint64_t stringSize = RStringLenght(string) + 1;

        // checking, if exist and size like copying
        if(object->size != 0 && object->baseString != NULL
                || object->size < stringSize) {
            deallocator(object->baseString);
            object->baseString = RAlloc(stringSize);
        }

        // final copying
        object->size = stringSize;
        RMemCpy(object->baseString, string, object->size);
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

method(RCString *, setConstantString, RCString), char *string){
    if(string != NULL) {
        // checking, if exist and size like copying
        if(object->size != 0 && object->baseString != NULL){
            deallocator(object->baseString);
        }
        // copy pointer, and compute length
        object->baseString = string;
        object->size = RStringLenght(string);
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

method(RCompareFlags, compareWith, RCString), RCString *checkString){
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
            }
        }
    }
    return not_equals;
}

method(RCString *, getSubstringInRange, RCString), RRange *range){
    if(range->count != 0 && range->from < object->size) {
        char *cstring = RAlloc(range->count + 1);
        RMemCpy(cstring, object->baseString[range->from], range->count);
        cstring[count + 1] = 0;
        return RS(substring);
    } else {
        RPrintf("ERRROR. RCS. BAD RANGE!\n");
        return NULL;
    }

}

printer(RCString){
    RPrintf("%s\n", object->baseString);
}

staticMethod(char , randomCharacter, RCString)){
    char character = ((char)rand());
    while(character < 34 ||
            character > 126) {
        character = ((char)rand());
    }
    return character;
}

staticMethod(RCString *, randomString, RCString)){
    static uint64_t iterator;
    RCString *string = makeRCString();
    uint64_t size = ((uint64_t)rand()) % 50;
    char *cstring;

    while(size == 0) {
        size = ((uint64_t)rand()) % 50;
    }

    cstring = RAlloc(size);

    forAll(iterator, size){
        cstring[iterator] = sm(randomCharacter, RCString)(NULL);
    }

    $(string, m(setConstantString, RCString)), cstring);
    return string;
}