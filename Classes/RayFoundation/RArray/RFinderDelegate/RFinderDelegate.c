#include "RFinderDelegate.h"

method(RFinderDelegateFlags, checkObject, RFinderDelegate), pointer objectToCheck){
    if(objectToCheck == object->etaloneObject){
        return equals;
    } else {
        if(object->virtualMethodcheckObjectOfRFinderDelegate == NULL){
            RPrintf("Warning. RFD - %p. Virtual method is not implemented.\n", object);
        } else {
            return object->virtualMethodcheckObjectOfRFinderDelegate(object, objectToCheck);
        }
    }
    return not_equals;
}