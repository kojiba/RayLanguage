#include "RVirtualMachine.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualMachine)) {
    object = allocator(RVirtualMachine);
    if(object != NULL) {
        object->memory = allocator(RByteArray);
        if(object->memory != NULL) {
            object->classId = registerClassOnce(toString(RVirtualMachine));
        }
    }
    return object;
}

destructor(RVirtualMachine) {
    if(object != NULL) {
        $(object->memory, d(RByteArray)));
        deallocator(object->memory);
    }
}

method(void, setUpDataBlock, RVirtualMachine)) {
    if(object->functionExecuting != NULL) {
        object->memory->array = makeFlushedBytes(memorySizeOfRVM, 0x00);
        object->memory->size = memorySizeOfRVM;
    } else {
        RPrintf("ERROR. RVM. Set-up function is NULL.");
    }
}

method(void, executeCode, RVirtualMachine)) {

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
            while(*object->dataRegister != r_string_end) {
                ++object->dataRegister;
                ++object->tickCount;
            }
            ++object->command;
        } break;

        case r_print_char : {
            if(*object->dataRegister < 32 || *object->dataRegister > 127) {
                RPrintf("%02x ", *object->dataRegister);
            } else {
                RPrintf("%c", *object->dataRegister);
            }
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
            object->command = object->functionStartAddress + *(++object->command);
        } break;

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

// work end
        case r_end : {
            RPrintf("End work of RVM.\n");
            return;
        }

// bad situation
        default: {
            RPrintf("RVM. Warning, default case, unhalted result\n");
            return;
        }
    }

    // increment ticks
    ++object->tickCount;

    // execute next code while not flag
    if(object->breakFlag == 0) {
        $(object, m(executeCode, RVirtualMachine)));
    }
}

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function) {
    // copy pointer
    object->functionExecuting = function;

    // function name label
    RPrintf("RVM. Start Executing Function : \"%s\"\n\n", function->name->baseString);

    // set tick count is 0
    object->tickCount = 0;

    // copy data block
    $(object, m(setUpDataBlock, RVirtualMachine)) );

    // set data register as pointer to first element of memory
    object->dataRegister = &object->memory->array[0];

    // set command to first byte of opcodes
    object->functionStartAddress = master(object->functionExecuting, RByteArray)->array;
    object->command = object->functionStartAddress;

    // execute first code, that starts processing
    $(object, m(executeCode, RVirtualMachine)));

    // at end of processing print analytics
    RPrintf("\nRVM. End Executing Function : \"%s\"\n", function->name->baseString);
    RPrintf("Ticks count for executing is - %qu\n", object->tickCount);
    RPrintf("Memory snapshot : {");
    $(object->memory, p(RByteArray)) );
    RPrintf("\n } end memory snapshot\n\n");
}

singleton(RVirtualMachine) {
    static RVirtualMachine *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualMachine)) );
    }
    return instance;
}