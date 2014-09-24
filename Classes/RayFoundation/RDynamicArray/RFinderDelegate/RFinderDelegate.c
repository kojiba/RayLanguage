#include "RFinderDelegate.h"
#include "../RDynamicArray.h"

method(byte, checkObject, RFinderDelegate), pointer objectToCheck){
    if(objectToCheck == object->etaloneObject){
        return object_founded;
    } else {
        if(object->virtualMethodcheckObjectOfRFinderDelegate == NULL){
            RPrintf("Warning. RFD - %p. Virtual method is not implemented.\n", object);
        } else {
            return object->virtualMethodcheckObjectOfRFinderDelegate(object, objectToCheck);
        }
    }
    return 0;
}