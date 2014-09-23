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

method(void, setString, RCString), char *string) {
    object->baseString = string;
    object->size = RStringLenght(string);
}

method(byte, compareWith, RCString), RCString *checkString){
    if(checkString == NULL || object == NULL) {
        printf("Warning. One of compare strings is empty, please delete function call, or fix it.\n");
    } else {
        if (checkString == object) {
            return equals;
        } else {
            if (checkString->size == object->size) {
                if (RStringCompare(checkString->baseString, object->baseString) == 0) {
                    return equals;
                }
            }
        }
    }
    return not_equals;
}