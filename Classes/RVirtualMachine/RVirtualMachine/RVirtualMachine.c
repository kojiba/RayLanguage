#include <assert.h>
#include "RVirtualMachine.h"

constructor(RVirtualMachine)) {
    object = allocator(RVirtualMachine);
    if(object != NULL) {
        object->memory = makeRArray();
    }
    return object;
}

destructor(RVirtualMachine) {
    if(object != NULL) {
        deleteRA(object->memory);
    }
}
method(void, setUpMemory, RVirtualMachine)) {
    // delete old, create new
    deleteRA(object->memory);
    object->memory = makeRArray();
    if (object->memory != NULL) {
        assert(object->memory);
        RPrintf("ERROR. RVM. Memory set-up, can't allocate.");
    }
}

method(void, setUpDataBlock,  RVirtualMachine)) {
    uint64_t iterator = 0;
    if(object->functionExecuting != NULL) {
        if(object->functionExecuting->body[iterator] == r_data_start) {

            while(object->functionExecuting->body[iterator] != r_function_begin) {
                switch (object->functionExecuting->body[iterator]) {
                    case r_const_start : {

                    } break;
                }
                ++iterator;
            }

        } else {
            RPrintf("RVM. Function has no constant data!\n");
        }
    } else {
        assert(object->memory);
        RPrintf("ERROR. RVM. Set-up data block is NULL.");
    }
}

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function) {

    // copy pointer
    object->functionExecuting = function;

    // function name label
    RPrintf("RVM. Start Executing Function : \"");
    $(function->name, p(RCString)) );
    RPrintf("\"\n");

    uint64_t iterator = 0;

    // set-up memory
    $(object, m(setUpMemory, RVirtualMachine)) );

    // copy data block
    $(object, m(setUpDataBlock, RVirtualMachine)) );

    //

    while(function->body[iterator] != r_end) {
        switch (function->body[iterator]) {
            case r_print : {
//                char * charString =

            } break;
        }
        ++iterator;
    }
}