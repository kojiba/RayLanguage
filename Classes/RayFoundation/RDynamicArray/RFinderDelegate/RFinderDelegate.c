#include "RFinderDelegate.h"
#include "../RDynamicArray.h"

method(byte, checkObject, RFinderDelegate), pointer objectToCheck){
    if(objectToCheck == object->etaloneObject){
        return object_founded;
    } else {
        object->virtualMethodcheckObjectOfRFinderDelegate(object, objectToCheck);
    }
}