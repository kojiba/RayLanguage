/**
 * RStringDictionary.c
 * Realization of C dictionary(map) for RCString, in Ray additions.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 3.10.14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RStringDictionary.h"
#include "../RClassTable/RClassTable.h"

constructor(RStringDictionary)) {
    object = allocator(RStringDictionary);
    if(object != nil) {
        master(object, RDictionary) = makeRDictionary();
        if(master(object, RDictionary) != nil) {

            // register self
            object->classId = registerClassOnce(toString(RStringDictionary));

            // set-up comparator
            master(master(object, RDictionary), RCompareDelegate)->virtualCompareMethod = m(compareWith, RCString);

            // set-up delegates destructors
            master(object, RDictionary)->keys->destructorDelegate = d(RCString);
        }
    }
    return object;
}

destructor(RStringDictionary) {
    if(object != nil) {
        $(master(object, RDictionary), d(RDictionary)) );
        deallocator(master(object, RDictionary));
    } else {
        RPrintf("Warning. RSD. Destructing a nil, do nothing, please delete function call, or fix it.\n");
    }
}

printer(RStringDictionary) {
    size_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RStringDictionary), object);
    RPrintf(" Count : %qu \n", master(object, RDictionary)->keys->count);
    RPrintf(" Free  : %qu \n", master(object, RDictionary)->keys->freePlaces);
    forAll(iterator, master(object, RDictionary)->keys->count) {
        RPrintf("\t %qu - {", iterator);
        RCString *key   = $(master(object, RDictionary)->keys, m(elementAtIndex, RArray)), iterator);
        RCString *value = $(master(object, RDictionary)->values, m(elementAtIndex, RArray)), iterator);
        RPrintf(" %s : %s } \n", key->baseString, value->baseString);
    }
    RPrintf("} end of %s object %p \n\n", toString(RStringDictionary), object);
}

method(void, setObjectForKey, RStringDictionary), pointer value, RCString *key) {
    $(master(object, RDictionary), m(setObjectForKey, RDictionary)), value, key);
}

method(pointer, getObjectForKey, RStringDictionary), RCString *key) {
    return $(master(object, RDictionary), m(getObjectForKey, RDictionary)), key);
}