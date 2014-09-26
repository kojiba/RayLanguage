/**
 * @file RDictionary.c
 * @brief Realization of C dictionary(map) pair, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RDictionary.h"
#include "../RClassTable/RClassTable.h"

constructor(RDictionary)){
    object = allocator(RDictionary);
    if(object == NULL) {
        RPrintf("ERROR. RD. Allocation error.");
    } else {
        // set up class ID
        object->classId = registerClassOnce(toString(RDictionary));
        object->keys = makeRArray();
        object->values = makeRArray();

        if(object->keys == NULL || object->values == NULL){
            RPrintf("ERROR. RD. Allocation keys or values error.");
            return object;
        } else {
            master(object, RCompareDelegate) = allocator(RCompareDelegate);
        }
    }
    return object;
}

destructor(RDictionary){
    if(object != NULL){
//        fixme
        deleteRA(object->keys);
        deleteRA(object->values);
    } else {
        RPrintf("Warning. RD. Destructing a NULL, do nothing, please delete function call, or fix it.\n");
    }
}

method(void, initDelegate, RDictionary), RCompareDelegate *delegate){
    master(object, RCompareDelegate) = delegate;
}

method(void, setObjectForKey, RDictionary), pointer value, pointer key){
    master(object, RCompareDelegate)->etaloneObject = key;
    RArrayFindResult *rArrayFindResult = $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

    // if object for key not exist
    if(rArrayFindResult == NULL){
        $(object->keys, m(addObject, RArray)), key);      // adding
        $(object->values, m(addObject, RArray)), value);  // adding

    // if key exist
    } else {
        $(object->values,  m(setObjectAtIndex, RArray)), value, rArrayFindResult->index);
    }

}

method(pointer, getObjectForKey, RDictionary), pointer key){
    master(object, RCompareDelegate)->etaloneObject = key;
    RArrayFindResult *rArrayFindResult = $(object->keys, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));
    return rArrayFindResult->result;
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
    RPrintf("} end of %s object %p \n\n", toString(RArray), object);
}