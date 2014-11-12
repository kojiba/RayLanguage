#include "RInterpreter.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

constructor(RInterpreter)) {
    object = allocator(RInterpreter);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RInterpreter));
//        fixme
    }
    return object;
}

destructor(RInterpreter) {
    //fixme
}

singleton(RInterpreter) {

}