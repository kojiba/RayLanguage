#include "RIObject.h"

printer(RIObject) {
    if(object != nil) {
        if(object->printer == nil) {
            RPrintf("RI object - %p %s\n", object, object->description->baseString);
        } else {
            $(object->printer,));
        }
    } else {
        RPrintf("nil");
    }
}