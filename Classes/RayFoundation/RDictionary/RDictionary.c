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
//        fixme
    }
    return object;
}

destructor(RDictionary){
    if(object != NULL){
//        fixme
    } else {
        RPrintf("Warning. RD. Destructing a NULL, do nothing, please delete function call, or fix it.\n");
    }
}

method(void, setObjectForKey, RDictionary), pointer key, pointer value){
//    fixme
    return;
}

method(pointer, getObjectForKey, RDictionary), pointer key){
//    fixme
    return NULL;
}

printer(RDictionary){
//    fixme
}