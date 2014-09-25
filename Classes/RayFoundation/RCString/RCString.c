#include <string.h>
#include "RCString.h"

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
}

method(RCString *, setString, RCString), char *string) {
    if(string != NULL) {
        object->baseString = string;
        object->size = RStringLenght(string);
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

method(RFinderDelegateFlags, compareWith, RCString), RCString *checkString){
    static uint64_t iterator;
    if(checkString == NULL || object == NULL) {
        RPrintf("Warning. RCS. One of compare strings is empty, please delete function call, or fix it.\n");
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