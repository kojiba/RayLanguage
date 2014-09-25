#include <stdio.h>
#include "RClassTable.h"

constructor(RClassTable)) {

    // alloc RClassTable
    object = allocator(RClassTable);
    if (object != NULL) {

        // alloc RArray
        master(object, RArray) = makeRArray();
        if (master(object, RArray) != NULL) {
            master(object, RFinderDelegate) = allocator(RFinderDelegate);
            if(master(object, RFinderDelegate) != NULL) {
                // overload delegate function
                master(object, RFinderDelegate)->virtualMethodcheckObjectOfRFinderDelegate = m(checkObject, RClassTable);
            } else {
                RPrintf("Warning. RCT. Bad allocation on delegate.");
            }
            // we store pairs, and set destructor for pair, and printer for pair
            master(object, RArray)->destructorDelegate = d(RClassNamePair);
            master(object, RArray)->printerDelegate = p(RClassNamePair);

            // register classes
            $(object, m(registerClassWithName, RClassTable)), toString(RArray));
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
        // destructor for RArray
        $(master(object, RArray), d(RArray)));
        deallocator(master(object, RArray));
    }
}

method(uint64_t, registerClassWithName, RClassTable), char *name) {

    // RArray pair
    RClassNamePair *pair = $(NULL, c(RClassNamePair)));

    if (pair != NULL) {
        $(master(pair, RCString), m(setString, RCString)), name);
        pair->idForClassName = master(object, RArray)->count;

        // successfully register new class
        if ($(master(object, RArray), m(addObject, RArray)), pair)) {
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
    return master(object, RArray)->count;
}

printer(RClassTable) {
    RPrintf("\n%s object %p: { \n", toString(RClassTable), object);
    $(master(object, RArray), p(RArray)));
    RPrintf("\t--- TOTAL: %qu classes registered ---\n", master(object, RArray)->count);
    RPrintf("} end of %s object %p \n\n", toString(RClassTable), object);
}

method(uint64_t, getIdentifierByClassName, RClassTable), char *name) {
    RClassNamePair *pair = $(NULL, c(RClassNamePair)));

    $(master(pair, RCString), m(setString, RCString)), name);
    master(object, RFinderDelegate)->etaloneObject = pair;

    RClassNamePair *foundedObject = $(master(object, RArray), m(findObjectWithDelegate, RArray)), master(object, RFinderDelegate));
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

method(RFinderDelegateFlags, checkObject, RClassTable), RClassNamePair *pairToCheck){
    return $(((RFinderDelegate*)object)->etaloneObject, m(compareWith, RClassNamePair)), pairToCheck);
}