#include "RVirtualFunction.h"
#include "../../Classes/RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualFunction)) {
    object = allocator(RVirtualFunction);
    if(object != NULL) {
        object->memory = makeRArray();
        object->classId = registerClassOnce(toString(RVirtualFunction));
        object->numberOfArguments = 0;
    }
    return object;
}

destructor(RVirtualFunction){
    deleteRA(object->memory);
}
method(void, executeWithArgs, RVirtualFunction), RArray *args){
    RPrintf("Executing function - ");
    $(object->name, p(RCString)) );
    RPrintf("\n");

}
