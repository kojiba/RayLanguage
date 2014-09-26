#include "RCompareDelegate.h"

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck){
    if(objectToCheck == object->etaloneObject){
        return equals;
    } else {
        if(object->virtualCompareMethod == NULL){
            RPrintf("Warning. RFD - %p. Virtual method is not implemented.\n", object);
        } else {
            return object->virtualCompareMethod(object->etaloneObject, objectToCheck);
        }
    }
    return not_equals;
}