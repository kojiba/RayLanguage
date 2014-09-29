#include <string.h>
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
        // not deleting old string, only copy pointer, and compute length
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

printer(RCString){
    RPrintf(object->baseString);
}