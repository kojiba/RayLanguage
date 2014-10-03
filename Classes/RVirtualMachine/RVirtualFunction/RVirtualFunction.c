#include "RVirtualFunction.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualFunction)) {
    object = allocator(RVirtualFunction);
    if(object != NULL) {
        // fixme size
        master(object, RByteArray) = c(RByteArray)(NULL, 255);
        if(master(object, RByteArray) != NULL) {
            object->classId = registerClassOnce(toString(RVirtualFunction));
        }
    }
    return object;
}

destructor(RVirtualFunction) {
    if(object != NULL) {
        $(master(object, RByteArray), d(RByteArray)) );
        deallocator(master(object, RByteArray));
    } else {
        RPrintf("Warning. RVF. Destructor of NULL.\n");
    }
}

printer(RVirtualFunction) {
    RPrintf("%s object - %p : \n", toString(RVirtualFunction), object);
    RPrintf("Name : \"%s\" \n", object->name->baseString);
    RPrintf("Byte Code : \" ");
    $(master(object, RByteArray), p(RByteArray)) );
    RPrintf("\" \n");
}
