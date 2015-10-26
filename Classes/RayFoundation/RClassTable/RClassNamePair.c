/**
 * RClassNamePair.c
 * ClassNamePair(name and id) for ClassTable.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RClassNamePair.h"

constructor(RClassNamePair)) {
    object = allocator(RClassNamePair);

    if (object != nil) {
        master(object, RString) = allocator(RString);
        if(master(object, RString) != nil) {
            // 2 - it's for RClassNamePair
            object->classId = 2;
            object->idForClassName = 0;
        }
    }
    return object;
}

destructor(RClassNamePair) {
    deallocator(master(object, RString));
}

void RClassNamePairDeleter(pointer ptr) {
    deleter(ptr, RClassNamePair);
}

printer(RClassNamePair) {
    if(master(object, RString) != nil) {
        RPrintf("\"%s\"\n", master(object, RString)->data);
    } else {
        RPrintf("error nil\n");
    }
}

constMethod(RCompareFlags, compareWith, RClassNamePair), RClassNamePair *checkPair) {
    if(object == checkPair) {
        return equals;
    }
    if(object != nil) {
        if(checkPair->idForClassName != 0) {
            if (checkPair->idForClassName == object->idForClassName) {
                return equals;
            }
        }
        if(master(checkPair, RString) != nil && master(object, RString) != nil) {
            return $(master(object, RString), m(compareWith, RString)), master(checkPair, RString));
        }
    } else {
        RWarning("RClassNamePair. Name and Id (or Object), is nil.", object);
        return not_equals;
    }
    return not_equals;
}