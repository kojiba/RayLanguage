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
        master(object, RCString) = makeRCString();
        // 2 - it's for RClassNamePair
        object->classId = 2;
    }
    return object;
}

destructor(RClassNamePair) {
}

printer(RClassNamePair) {
    RPrintf("%p , { %lu : %s }\n", object, object->idForClassName, master(object, RCString)->baseString);
}

method(RCompareFlags, compareWith, RClassNamePair), RClassNamePair *checkPair) {
    if(object != nil) {
        if(checkPair->idForClassName != 0) {
            if (checkPair->idForClassName == object->idForClassName) {
                return equals;
            }
        }
        if(master(checkPair, RCString) != nil && master(object, RCString) != nil) {
            if($(master(object, RCString), m(compareWith, RCString)), master(checkPair, RCString)) == equals){
                return equals;
            }
        }
    } else {
        RWarning("RCNP. Name and Id (or Object), is nil, please delete function call, or fix it", object);
        return not_equals;
    }
    return not_equals;
}