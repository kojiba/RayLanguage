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

#include <RClassNamePair.h>

constructor(RClassNamePair)) {
    object = allocator(RClassNamePair);

    if (object != nil) {
        master(object, RCString) = makeRCString();
        if(master(object, RCString) != nil) {
            // 2 - it's for RClassNamePair
            object->classId = 2;
            object->idForClassName = 0;
        }
    }
    return object;
}

destructor(RClassNamePair) {
    deallocator(master(object, RCString));
}

void RClassNamePairDeleter(pointer ptr) {
    deleter(ptr, RClassNamePair);
}

printer(RClassNamePair) {
    RPrintf("%p , { %lu : %s }\n", object, object->idForClassName, master(object, RCString)->baseString);
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
        if(master(checkPair, RCString) != nil && master(object, RCString) != nil) {
            return $(master(object, RCString), m(compareWith, RCString)), master(checkPair, RCString));
        }
    } else {
        RError("RCNP. Name and Id (or Object), is nil, please delete function call, or fix it", object);
        return not_equals;
    }
    return not_equals;
}