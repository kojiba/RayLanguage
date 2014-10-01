#include "RVirtualFunction.h"
#include "../../Classes/RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualFunction)) {
    object = allocator(RVirtualFunction);
    if(object != NULL) {
//        object->memory = makeRArray();
        object->classId = registerClassOnce(toString(RVirtualFunction));
//        object->numberOfArguments = 0;
    }
    return object;
}

destructor(RVirtualFunction) {
//    deleteRA(object->memory);
}

method(void, executeWithArgs, RVirtualFunction), RArray *args) {
    RPrintf("Executing function - ");
    $(object->name, p(RCString)) );
    RPrintf("\n");

}

printer(RVirtualFunction) {
    RPrintf("%s object - %p : \n", toString(RVirtualFunction), object);
    RPrintf("Name : \"%s\" \n", object->name->baseString);
    RPrintf("Byte Code : \" ");
    uint64_t iterator = 1;
    while(object->body[iterator] != r_end) {
        if(iterator % 80 == 0) {
            RPrintf("\n");
        }
        RPrintf("%02x ", object->body[iterator]);
        ++iterator;
    }
    RPrintf("\" \n");
}
