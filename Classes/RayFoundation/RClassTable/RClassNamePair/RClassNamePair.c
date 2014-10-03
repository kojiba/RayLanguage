#include "RClassNamePair.h"

constructor(RClassNamePair)) {
    object = allocator(RClassNamePair);

    if (object != NULL) {
        master(object, RCString) = makeRCString();
        // 2 - it's for RClassNamePair
        object->classId = 2;
    }
    return object;
}

destructor(RClassNamePair) {
}

printer(RClassNamePair) {
    RPrintf("%p , { %qu - %s }\n", object, object->idForClassName, master(object, RCString)->baseString);
}

method(RCompareFlags, compareWith, RClassNamePair), RClassNamePair *checkPair) {
    if(object != NULL) {
        if(checkPair->idForClassName != 0) {
            if (checkPair->idForClassName == object->idForClassName) {
                return equals;
            }
        }
        if(master(checkPair, RCString) != NULL && master(object, RCString) != NULL) {
            if($(master(object, RCString), m(compareWith, RCString)), master(checkPair, RCString)) == equals){
                return equals;
            }
        }
    } else {
        RPrintf("Warning. RCNP. Name and Id (or Object), is NULL, please delete function call, or fix it\n");
        return not_equals;
    }
    return not_equals;
}