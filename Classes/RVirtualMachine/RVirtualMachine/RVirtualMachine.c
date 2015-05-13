#include "RVirtualMachine.h"

constructor(RVirtualMachine)) {
    object = allocator(RVirtualMachine);
    if(object != nil) {
        object->memory = allocator(RByteArray);
        if(object->memory != nil) {
            object->classId = registerClassOnce(toString(RVirtualMachine));
        }
    }
    return object;
}

destructor(RVirtualMachine) {
    if(object != nil) {
        $(object->memory, d(RByteArray)));
        deallocator(object->memory);
    }
}

method(void, setUpDataBlock, RVirtualMachine)) {
    if(object->functionExecuting != nil) {
        // set-up break flag
        object->breakFlag = no;

        object->memory->array = makeFlushedBytes(memorySizeOfRVM, 0x00);
        object->memory->size  = memorySizeOfRVM;
    } else {
        RPrintf("ERROR. RVM. Set-up function is nil.");
    }
}

method(size_t, executeCode, RVirtualMachine)) {

    switch(*object->command) {

        case r_increment : {
            // increment data at pointer
            ++(*object->dataRegister);
            ++object->command;
        } break;

        case r_decrement : {
            // decrement data at pointer
            --(*object->dataRegister);
            ++object->command;
        } break;

// prints
        case r_print_0_string : {
            RPrintf("%s\n", object->dataRegister);
            while(*object->dataRegister != 0) {
                ++object->dataRegister;
                ++object->tickCount;
            }
            ++object->command;
        } break;

        case r_print_char : {
            if(*object->dataRegister == '\n'
                    || *object->dataRegister == '\r'
                    || *object->dataRegister == '\t') {
                RPrintf("%c", *object->dataRegister);
            } else if(*object->dataRegister < 32
                    || *object->dataRegister > 127) {
                RPrintf("%02x ", *object->dataRegister);
            } else {
                RPrintf("%c", *object->dataRegister);
            }
            ++object->command;
        } break;
// gets
        case r_get_char : {
            *object->dataRegister = (byte) RGetChar();
            ++object->command;
        } break;

// moves forward-backward
        case r_move_forward : {
            // increment pointer
            ++object->dataRegister;
            ++object->command;
        } break;

        case r_move_backward : {
            // decrement pointer
            --object->dataRegister;
            ++object->command;
        } break;

        case r_goto_address : {
            // set pointer to command incremented pointers data, like JMP address, fixme when instruction in data
            object->command = object->functionStartAddress + (*(++object->command) % object->memory->size); // testme mod size
        } break;

// logical
        case r_if : {
            if((*object->dataRegister) != 0) {
                // true instruction
                object->command += 3; // 3 - cause of goto arg byte
            } else {
                // false instruction
                ++object->command;
            }
        } break;

        case r_if_not : {
            if((*object->dataRegister) == 0) {
                // true instruction
                object->command += 3; // 3 - cause of goto arg byte
            } else {
                // false instruction
                ++object->command;
            }
        } break;

// debug
        case r_break : {
            object->breakFlag = yes;
            RPrintf("\nEnd work of RVM.\n");
            return 1;
        }

// work end
        case r_end : {
            RPrintf("\nEnd work of RVM.\n");
            return 1;
        }

// bad situation
        default: {
            RPrintf("RVM. Warning, default case, unhalted result\n");
            return 1;
        }
    }
    return 0;
}

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function) {
    size_t result = 0;
    // copy pointer
    object->functionExecuting = function;

    // function name label
    RPrintf("RVM. Start Executing Function \"%s\"\n\n", function->name->baseString);

    // set tick size is 0
    object->tickCount = 0;

    // set-up data block
    $(object, m(setUpDataBlock, RVirtualMachine)) );

    // set data register as pointer to first element of memory
    object->dataRegister = &object->memory->array[0];

    // set command to first byte of opcodes
    object->functionStartAddress = master(object->functionExecuting, RByteArray)->array;
    object->command              = object->functionStartAddress;

    // execute first code, that starts processing
    // execute next code while not flag
    while(result != 1) {
        result = $(object, m(executeCode, RVirtualMachine)));
        // increment ticks
        ++object->tickCount;
        // break
        if(object->breakFlag) {
            RPrintf("RVM break at %lu tick.", object->tickCount);
        }
    }

    // at end of processing print analytics
    RPrintf("\nRVM. End Executing Function \"%s\"\n", function->name->baseString);
    RPrintf("Ticks count for executing is - %lu\n", object->tickCount);
    RPrintf("Memory snapshot : {\n");
    $(object->memory, p(RByteArray)) );
    RPrintf("} end memory snapshot\n");
}

singleton(RVirtualMachine) {
    static RVirtualMachine *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RVirtualMachine)) );
    }
    return instance;
}