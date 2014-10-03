/**
 * @file RStringDictionary.c
 * @brief Implementation of C dictionary(map) for RCString, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 * @created 3.10.14 9:48:30
 */

#include "RStringDictionary.h"
#include "../RClassTable/RClassTable.h"

constructor(RStringDictionary)) {
    object = allocator(RStringDictionary);
    if(object != NULL) {
        master(object, RDictionary) = makeRDictionary();
        if(master(object, RDictionary) != NULL) {

            // register self
            object->classId = registerClassOnce(toString(RStringDictionary));

            // set-up comparator
            master(master(object, RDictionary), RCompareDelegate)->virtualCompareMethod = m(compareWith, RCString);

            // set-up delegates destructors
            master(object, RDictionary)->keys->destructorDelegate = d(RCString);
            master(object, RDictionary)->values->destructorDelegate = d(RCString);
        }
    }
    return object;
}

destructor(RStringDictionary) {
    if(object != NULL) {
        $(master(object, RDictionary), d(RDictionary)) );
        deallocator(master(object, RDictionary));
    } else {
        RPrintf("Warning. RSD. Destructing a NULL, do nothing, please delete function call, or fix it.\n");
    }
}

printer(RStringDictionary) {
    uint64_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RStringDictionary), object);
    RPrintf(" Count : %qu \n", master(object, RDictionary)->keys->count);
    RPrintf(" Free  : %qu \n", master(object, RDictionary)->keys->freePlaces);
    forAll(iterator, master(object, RDictionary)->keys->count) {
        RPrintf("\t %qu - {", iterator);
        RCString *key = $(master(object, RDictionary)->keys, m(elementAtIndex, RArray)), iterator);
        RCString *value = $(master(object, RDictionary)->values, m(elementAtIndex, RArray)), iterator);
        RPrintf(" %s : %s } \n", key->baseString, value->baseString);
    }
    RPrintf("} end of %s object %p \n\n", toString(RStringDictionary), object);
}

method(void, setObjectForKey, RStringDictionary), RCString *value, RCString *key) {
    $(master(object, RDictionary), m(setObjectForKey, RDictionary)), value, key);
}

method(RCString *, getObjectForKey, RStringDictionary), RCString *key) {
    RCString *result = $(master(object, RDictionary), m(getObjectForKey, RDictionary)), key);
    return result;
}