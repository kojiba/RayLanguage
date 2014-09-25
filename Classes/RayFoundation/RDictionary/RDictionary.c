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
//            fixme
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

method(void, setObjectForKey, RDictionary), pointer value, pointer key){
//    fixme
    $(object->keys, m(addObject, RArray)), key);
    $(object->values, m(addObject, RArray)), value);
}

method(pointer, getObjectForKey, RDictionary), pointer key){
//    fixme
    return NULL;
}

printer(RDictionary){
    uint64_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RArray), object);
    RPrintf(" Count : %qu \n", object->keys->count);
    RPrintf(" Free  : %qu \n", object->keys->freePlaces);
    forAll(iterator, object->keys->count) {
        RPrintf("\t %qu - {", iterator);
        RPrintf(" %p : %p } \n", $(object->keys, m(elementAtIndex, RArray)), iterator),
                                $(object->values, m(elementAtIndex, RArray)), iterator) );
    }
    RPrintf("} end of %s object %p \n\n", toString(RArray), object);
}