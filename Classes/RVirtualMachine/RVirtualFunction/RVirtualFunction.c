#include "RVirtualFunction.h"
#include "../RVirtualMachine/RVirtualMachine.h"

constructor(RVirtualFunction)) {
    object = allocator(RVirtualFunction);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RVirtualFunction));
    }
    return object;
}

destructor(RVirtualFunction) {
    if(object != nil) {
        $(master(object, RByteArray), d(RByteArray)) );
        deallocator(master(object, RByteArray));
    } else {
        RPrintf("Warning. RVF. Destructor of nil.\n");
    }
}

printer(RVirtualFunction) {
    size_t iterator;
    RPrintf("%s object - %p : \n", toString(RVirtualFunction), object);
    RPrintf("Name : \"%s\" \n", object->name->baseString);
    RPrintf("Byte Code : \" \n");

    forAll(iterator, master(object, RByteArray)->size) {
        RPrintf("\t %lu - ",iterator);
        switch (master(object, RByteArray)->array[iterator]) {

            case r_increment : {
                RPrintf("incr");
                RPrintf("\n");
            } break;

            case r_decrement : {
                RPrintf("decr");
                RPrintf("\n");
            } break;

            case r_move_forward : {
                RPrintf("fwd");
                RPrintf("\n");
            } break;

            case r_move_backward : {
                RPrintf("bkwd");
                RPrintf("\n");
            } break;

            case r_print_char : {
                RPrintf("p char");
                RPrintf("\n");
            } break;

            case r_if : {
                RPrintf("if \n ");
                RPrintf("\t\tfalse : goto %lu \n", master(object, RByteArray)->array[iterator += 2]);
                RPrintf("\t\ttrue :");
            } break;

            case r_if_not : {
                RPrintf("if NOT \n ");
                RPrintf("\t\tfalse : goto %lu \n", master(object, RByteArray)->array[iterator += 2]);
                RPrintf("\t\ttrue :");
            } break;

            case r_goto_address : {
                RPrintf("goto %lu ", master(object, RByteArray)->array[++iterator]);
                RPrintf("\n");
            } break;

            case r_end : {
                RPrintf("end");
                RPrintf("\n");
            } break;

            default: {
                RPrintf("%lu", master(object, RByteArray)->array[iterator]);
                RPrintf("\n");
            } break;
        }
    }
    RPrintf("\" \n");
}
