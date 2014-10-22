/**
 * RDictionary.c
 * Realization of C dictionary(map) pair, in Ray additions.
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

#include "RDictionary.h"
#include "../RClassTable/RClassTable.h"

constructor(RDictionary)) {
    object = allocator(RDictionary);
    if(object == NULL) {
        RPrintf("ERROR. RD. Allocation error.");
    } else {
        // set up class ID
        object->classId = registerClassOnce(toString(RDictionary));
        object->keys    = makeRArray();
        object->values  = makeRArray();

        if(object->keys == NULL
                || object->values == NULL) {
            RPrintf("ERROR. RD. Allocation keys or values error.");
        } else {
            object->values->destructorDelegate = NULL;
            object->keys->destructorDelegate   = NULL;
            object->values->printerDelegate    = NULL;
            object->keys->printerDelegate      = NULL;

            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            master(object, RCompareDelegate)->virtualCompareMethod = NULL;
        }
    }
    return object;
}

destructor(RDictionary) {
    if(object != NULL) {
        deleteRA(object->keys);
        deleteRA(object->values);
        deallocator(master(object, RCompareDelegate));
    } else {
        RPrintf("Warning. RD. Destructing a NULL, do nothing, please delete function call, or fix it.\n");
    }
}

method(void, initDelegate, RDictionary), const RCompareDelegate *delegate) {
    master(object, RCompareDelegate)->etaloneObject        = delegate->etaloneObject;
    master(object, RCompareDelegate)->virtualCompareMethod = delegate->virtualCompareMethod;
}

method(void, setObjectForKey, RDictionary), pointer value, pointer key) {
    master(object, RCompareDelegate)->etaloneObject = key;
    RArrayFindResult *rArrayFindResult = $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

    // if object for key not exist
    if(rArrayFindResult == NULL){
        $(object->keys,   m(addObject, RArray)), key);    // adding
        $(object->values, m(addObject, RArray)), value);  // adding

    // if key exist
    } else {
        $(object->values, m(setObjectAtIndex, RArray)), value, rArrayFindResult->index);
    }
}

method(pointer, getObjectForKey, RDictionary), pointer key) {
    master(object, RCompareDelegate)->etaloneObject = key;
    RArrayFindResult *rArrayFindResult              = $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

    if(rArrayFindResult != NULL) {
        return $(object->values, m(elementAtIndex, RArray)), rArrayFindResult->index);
    } else {
        return NULL;
    }
}

printer(RDictionary){
    uint64_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RDictionary), object);
    RPrintf(" Count : %qu \n", object->keys->count);
    RPrintf(" Free  : %qu \n", object->keys->freePlaces);
    forAll(iterator, object->keys->count) {
        RPrintf("\t %qu - {", iterator);
        RPrintf(" %p : %p } \n", $(object->keys, m(elementAtIndex, RArray)), iterator),
                                 $(object->values, m(elementAtIndex, RArray)), iterator) );
    }
    RPrintf("} end of %s object %p \n\n", toString(RDictionary), object);
}