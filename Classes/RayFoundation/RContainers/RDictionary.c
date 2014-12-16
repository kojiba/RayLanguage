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

#include <RDictionary.h>
#include <RClassTable.h>

constructor(RDictionary)) {
    object = allocator(RDictionary);
    if(object == nil) {
        RError("RD. Constructor allocation error.", object);
    } else {
        // set up class ID
        object->classId = registerClassOnce(toString(RDictionary));
        object->keys    = makeRArray();
        object->values  = makeRArray();

        if(object->keys == nil
                || object->values == nil) {
            RPrintf("ERROR. RD. Allocation keys or values error.");
        } else {
            object->values->destructorDelegate = nil;
            object->keys->destructorDelegate   = nil;
            object->values->printerDelegate    = nil;
            object->keys->printerDelegate      = nil;

            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            master(object, RCompareDelegate)->virtualCompareMethod = nil;
        }
    }
    return object;
}

destructor(RDictionary) {
    if(object != nil) {
        deleter(object->keys, RArray);
        deleter(object->values, RArray);
        deallocator(master(object, RCompareDelegate));
    } else {
        RWarning("Warning. RD. Destructing a nil, do nothing, please delete function call, or fix it.", object);
    }
}

method(void, initDelegate, RDictionary), const RCompareDelegate *delegate) {
    master(object, RCompareDelegate)->etaloneObject        = delegate->etaloneObject;
    master(object, RCompareDelegate)->virtualCompareMethod = delegate->virtualCompareMethod;
}

method(void, setObjectForKey, RDictionary), pointer value, pointer key) {
    master(object, RCompareDelegate)->etaloneObject = key;
    RFindResult findResult = $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

    // if object for key not exist
    if(findResult.object == nil){
        $(object->keys,   m(addObject, RArray)), key);    // adding
        $(object->values, m(addObject, RArray)), value);  // adding

    // if key exist
    } else {
        $(object->values, m(setObjectAtIndex, RArray)), value, findResult.index);
    }
}

method(pointer, getObjectForKey, RDictionary), pointer key) {
    master(object, RCompareDelegate)->etaloneObject = key;
    RFindResult findResult= $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

    if(findResult.object != nil) {
        return $(object->values, m(elementAtIndex, RArray)), findResult.index);
    } else {
        return nil;
    }
}

printer(RDictionary){
    size_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RDictionary), object);
    RPrintf(" Count : %lu \n", object->keys->count);
    RPrintf(" Free  : %lu \n", object->keys->freePlaces);
    forAll(iterator, object->keys->count) {
        RPrintf("\t %lu - {", iterator);
        RPrintf(" %p : %p } \n", $(object->keys, m(elementAtIndex, RArray)), iterator),
                                 $(object->values, m(elementAtIndex, RArray)), iterator) );
    }
    RPrintf("} end of %s object %p \n\n", toString(RDictionary), object);
}