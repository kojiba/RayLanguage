#include "RVirtualFunction.h"
#include "../RVirtualMachine/RVirtualMachine.h"

constructor(RVirtualFunction)) {
    object = allocator(RVirtualFunction);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RVirtualFunction));
        object->name = nil;
    }
    return object;
}

destructor(RVirtualFunction) {
    deleter(master(object, RData), RData);
    nilDeleter(object->name, RString);
}

printer(RVirtualFunction) {
    size_t iterator;
    RPrintf("%s object - %p : \n", toString(RVirtualFunction), object);
    RPrintf("Name : ");
    p(RString)(object->name);
    RPrintf("Byte Code : \n");

    forAll(iterator, master(object, RData)->size) {
        RPrintf("\t %lu - ",iterator);
        switch (master(object, RData)->data[iterator]) {

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

            case r_addition: {
                RPrintf("add ");
                byte addresses = master(object, RData)->data[iterator += 1];
                byte first = addresses >> 4 & (byte) 0x0F;
                byte second = addresses & (byte) 0x0F;
                RPrintf("%d, %d \n", first, second);
            } break;


            case r_if : {
                size_t whereGoTo;
                memcpy(&whereGoTo, &master(object, RData)->data[iterator += 2], sizeof(size_t));

                RPrintLn("if");
                RPrintf("\t\tfalse : goto %lu (%lu byte address)\n", whereGoTo, sizeof(size_t));
                RPrintf("\t\ttrue : ");
                iterator += sizeof(size_t) - 1;
            } break;

            case r_if_not : {
                size_t whereGoTo;
                memcpy(&whereGoTo, &master(object, RData)->data[iterator += 2], sizeof(size_t));

                RPrintLn("if NOT");
                RPrintf("\t\tfalse : goto %lu (%lu byte address)\n", whereGoTo, sizeof(size_t));
                RPrintf("\t\ttrue :");
                iterator += sizeof(size_t) - 1;
            } break;

            case r_goto_address : {
                RPrintf("goto %u\n", master(object, RData)->data[++iterator]);
            } break;

            case r_end : {
                RPrintLn("end");
            } break;

            default: {
                RPrintf("unknown - %u", master(object, RData)->data[iterator]);
            } break;
        }
    }
    RPrintf("\n");
}
