#include "RIObject.h"

printer(RIObject) {
    if(object != nil) {
        if(object->printer == nil) {
            RPrintf("%s object - %p\n", toString(RIOjbect), object);
        } else {
            $(object->printer,));
        }
    } else {
        RPrintf("nil");
    }
}

void deleteRIObject(RIObject *object) {
    deallocator(object);
}