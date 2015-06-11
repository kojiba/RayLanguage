/**
 * RStorageObject.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 6/11/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RCString/RString_Consts.h>
#include "RStorageObject.h"
#include "RObjectField.h"

constructor(RStorageObject)) {
    object = allocator(RStorageObject);
    if(object != nil) {
        object->fields = makeRArray();
        if(object->fields != nil) {
            $(object->fields, m(setDestructorDelegate, RArray)), (DestructorDelegate) RObjectFieldDeleter);
        } else  {
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(RStorageObject) {
    deleter(object->fields, RArray);
}

void RStorageObjectDeleter(RStorageObject *object) {
    deleter(object, RStorageObject);
}


RStorageObject * deserializeFromStringRStorageObject(RString *string) {
    RStorageObject *result = nil;

    size_t index = $(string, m(indexOfSubstring, RCString)), RS("{"));
    if(index != string->size) {
        result = c(RStorageObject)(result);
        if(result != nil) {
            RString *name = $(string, m(substringInRange, RCString)), makeRRange(0, index));
            size_t endIndex;

            $(string, m(trimHead, RCString)), index);
            endIndex = $(string, m(indexOfSubstring, RCString)), RS("}"));

            if(endIndex != string->size) {
                RString *tokenBody = $(string, m(substringInRange, RCString)), makeRRange(index, endIndex));
                size_t tokenEnd = $(string, m(indexOfSubstring, RCString)), RS(","));
                RArray  *token = $(string, m(substringInRange, RCString)), makeRRange(index, endIndex));
                #error fixme
                result->name = name;
            }
        }
    }
    return result;
}

method(RString *, serializeToString, RStorageObject)) {
    return nil;
}

