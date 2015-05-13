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
    deleter(master(object, RByteArray), RByteArray);
    nilDeleter(object->name, RCString);
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
                RPrintLn("incr");
            } break;

            case r_decrement : {
                RPrintLn("decr");
            } break;

            case r_move_forward : {
                RPrintLn("fwd");
            } break;

            case r_move_backward : {
                RPrintLn("bkwd");
            } break;

            case r_print_char : {
                RPrintLn("p char");
            } break;

            case r_if : {
                RPrintLn("if");
                RPrintf("\t\tfalse : goto %u \n", master(object, RByteArray)->array[iterator += 2]);
                RPrintf("\t\ttrue : ");
            } break;

            case r_if_not : {
                RPrintLn("if NOT");
                RPrintf("\t\tfalse : goto %u \n", master(object, RByteArray)->array[iterator += 2]);
                RPrintf("\t\ttrue :");
            } break;

            case r_goto_address : {
                RPrintf("goto %u\n", master(object, RByteArray)->array[++iterator]);
            } break;

            case r_end : {
                RPrintLn("end");
            } break;

            default: {
                RPrintf("unknown - %u", master(object, RByteArray)->array[iterator]);
            } break;
        }
    }
    RPrintf("\" \n");
}
