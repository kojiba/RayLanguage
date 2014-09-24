#include "RClassNamePair.h"
#include "../../RDynamicArray/RDynamicArray.h"

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

method(byte, compareWith, RClassNamePair), RClassNamePair *checkPair) {
    if(object != NULL) {
        if(checkPair->idForClassName != 0) {
            if (checkPair->idForClassName == object->idForClassName) {
                return object_founded;
            }
        }
        if(master(checkPair, RCString) != NULL && master(object, RCString) != NULL) {
            if($(master(object, RCString), m(compareWith, RCString)), master(checkPair, RCString)) == equals){
                return object_founded;
            }
        }
    } else {
        RPrintf("Warning. RCNP. Name and Id (or Object), is NULL, please delete function call, or fix it\n");
        return 0;
    }
}