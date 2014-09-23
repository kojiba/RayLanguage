#include <stdio.h>
#include "RClassTable.h"
#include "RClassNamePair/RClassNamePair.h"

constructor(RClassTable)) {

    // alloc RClassTable
    object = allocator(RClassTable);
    if (object != NULL) {

        // alloc RDynamicArray
        master(object, RDynamicArray) = makeRDArray();
        if (master(object, RDynamicArray) != NULL) {
            master(object, RFinderDelegate) = allocator(RFinderDelegate);
            if(master(object, RFinderDelegate) != NULL) {
                // overload delegate function
                master(object, RFinderDelegate)->virtualMethodcheckObjectOfRFinderDelegate = m(checkObject, RClassTable);
            } else {
                RPrintf("Warning. RCT. Bad allocation on delegate.");
            }
            // we store pairs, and set destructor for pair, and printer for pair
            master(object, RDynamicArray)->destructorDelegate = d(RClassNamePair);
            master(object, RDynamicArray)->printerDelegate = p(RClassNamePair);

            // register classes
            $(object, m(registerClassWithName, RClassTable)), toString(RDynamicArray));
            $(object, m(registerClassWithName, RClassTable)), toString(RCString));
            $(object, m(registerClassWithName, RClassTable)), toString(RClassNamePair));

            // register self
            object->classId = $(object, m(registerClassWithName, RClassTable)), toString(RClassTable));
        }
    }

    return object;
}

destructor(RClassTable) {
    if (object != NULL) {
        // destructor for RDynamicArray
        $(master(object, RDynamicArray), d(RDynamicArray)));
        deallocator(master(object, RDynamicArray));
    }
}

method(uint64_t, registerClassWithName, RClassTable), char *name) {

    // RDynamicArray pair
    RClassNamePair *pair = $(NULL, c(RClassNamePair)));

    if (pair != NULL) {
        $(master(pair, RCString), m(setString, RCString)), name);
        pair->idForClassName = master(object, RDynamicArray)->count;

        // successfully register new class
        if ($(master(object, RDynamicArray), m(addObject, RDynamicArray)), pair)) {
            return pair->idForClassName;
        } else {
            return 0;
        }

    // alloc error
    } else {
        return 0;
    }
}

method(uint64_t, getNumberOfClasses, RClassTable)) {
    return master(object, RDynamicArray)->count;
}

printer(RClassTable) {
    RPrintf("\n%s object %p: { \n", toString(RClassTable), object);
    $(master(object, RDynamicArray), p(RDynamicArray)));
    RPrintf("\t--- TOTAL: %qu classes registered ---\n", master(object, RDynamicArray)->count);
    RPrintf("} end of %s object %p \n\n", toString(RClassTable), object);
}

method(uint64_t, getIdentifierByClassName, RClassTable), char *name) {
    RClassNamePair *pair = $(NULL, c(RClassNamePair)));

    $(master(pair, RCString), m(setString, RCString)), name);
    master(object, RFinderDelegate)->etaloneObject = pair;

    RClassNamePair *foundedObject = $(master(object,RDynamicArray), m(findObjectWithDelegate, RDynamicArray)), master(object, RFinderDelegate));
    if(foundedObject == NULL){
        return 0;
    } else {
        return foundedObject->idForClassName;
    }
}

singleton(RClassTable) {
    static RClassTable *instance;
    if (instance == NULL) {
        instance = $(NULL, c(RClassTable)));
    }
    return instance;
}

method(byte, checkObject, RClassTable), RClassNamePair *pairToCheck){
    return $(master(object, RFinderDelegate)->etaloneObject, m(compareWith, RClassNamePair)), pairToCheck);
}