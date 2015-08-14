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
    RPrintf("Byte Code : \n");

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
                size_t whereGoTo;
                memcpy(&whereGoTo, &master(object, RByteArray)->array[iterator += 2], sizeof(size_t));

                RPrintLn("if");
                RPrintf("\t\tfalse : goto %lu (%lu byte address)\n", whereGoTo, sizeof(size_t));
                RPrintf("\t\ttrue : ");
                iterator += sizeof(size_t) - 1;
            } break;

            case r_if_not : {
                size_t whereGoTo;
                memcpy(&whereGoTo, &master(object, RByteArray)->array[iterator += 2], sizeof(size_t));

                RPrintLn("if NOT");
                RPrintf("\t\tfalse : goto %lu (%lu byte address)\n", whereGoTo, sizeof(size_t));
                RPrintf("\t\ttrue :");
                iterator += sizeof(size_t) - 1;
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
    RPrintf("\n");
}
