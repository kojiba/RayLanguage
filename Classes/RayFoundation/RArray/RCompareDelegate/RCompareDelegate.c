#include "RCompareDelegate.h"

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck){
    if(object->virtualCompareMethod != NULL) {
        return object->virtualCompareMethod(object->etaloneObject, objectToCheck);
    } else if(object->etaloneObject == objectToCheck) {
            return equals;
    }
#if RAY_SHORT_DEBUG == 1
        else {
            static pointer lastObject = NULL;
            if(lastObject != object) {
                lastObject = object;
                RPrintf("Warning. RFD - %p virtual method is not implemented.\n", object);
            }
        }
#endif
    return not_equals;
}