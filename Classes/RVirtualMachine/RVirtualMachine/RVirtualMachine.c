#include "RVirtualMachine.h"

constructor(RVirtualMachine)) {
    object = allocator(RVirtualMachine);
    if(object != NULL) {
        object->memory = NULL;
    }
    return object;
}

destructor(RVirtualMachine) {
    if(object != NULL) {
        if(object->memory != NULL) {
            $(object->memory, d(RByteArray)));
            deallocator(object->memory);
        }
    }
}

method(void, setUpDataBlock, RVirtualMachine)) {
    if(object->functionExecuting != NULL) {
        object->memory = $(master(object->functionExecuting, RByteArray), m(copy, RByteArray)));
    } else {
        RPrintf("ERROR. RVM. Set-up function is NULL.");
    }
}

method(void, executeCode, RVirtualMachine), byte code) {

    switch(code) {
        case r_increment : {
            // increment data at pointer
            ++(*object->dataRegister);
        } break;

        case r_decrement : {
            // decrement data at pointer
            --(*object->dataRegister);
        } break;

        case r_print : {
            RPrintf("%s\n", object->dataRegister);
            while(*object->dataRegister != r_string_end) {
                ++object->dataRegister;
                ++object->tickCount;
            }
        } break;

        case r_move_forward : {
            // increment pointer
            ++object->dataRegister;
        } break;

        case r_move_backward : {
            // decrement pointer
            --object->dataRegister;
        } break;

        case r_jump_address : {
            // set pointer to incremented pointers data, like JMP address
            object->dataRegister = *(++object->dataRegister);
        } break;

        case r_end : {
            RPrintf("End work of RVM.");
            return;
        }

        default: {
            RPrintf("RVM. Warning, default case, unhalted result\n");
            return;
        }
    }

    // increment ticks
    ++object->tickCount;

    // execute next code
    $(object, m(executeCode, RVirtualMachine)), *object->dataRegister);
}

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function) {
    // copy pointer
    object->functionExecuting = function;

    // function name label
    RPrintf("RVM. Start Executing Function : \"");
    $(function->name, p(RCString)) );
    RPrintf("\"\n");

    // set tick count is 0
    object->tickCount = 0;

    // copy data block
    $(object, m(setUpDataBlock, RVirtualMachine)) );

    // set data register as pointer to first element of memory
    object->dataRegister = object->memory->array;

    // execute first code, that starts processing
    $(object, m(executeCode, RVirtualMachine)), *object->dataRegister);

    // at end of processing print analytics
    RPrintf("RVM. End Executing Function : \"");
    $(function->name, p(RCString)) );
    RPrintf("\"\n");
    RPrintf("Ticks count for executing is - %qu\n", object->tickCount);
    RPrintf("Memory snapshot : {\n");
    $(object->memory, p(RByteArray)) );
    RPrintf(" } end memory snapshot\n\n");
}

singleton(RVirtualMachine) {
    static RVirtualMachine *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualMachine)) );
    }
    return instance;
}