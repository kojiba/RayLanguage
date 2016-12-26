#include "RVirtualMachine.h"

//#define VISUALIZE
#ifdef VISUALIZE
    #include "ncurses.h"

    #define COLORED
    #define msecDelay 0//50
    #if msecDelay != 0
        #include <unistd.h>
    #endif
#endif

constructor(RVirtualMachine)) {
    object = allocator(RVirtualMachine);
    if(object != nil) {
        object->memory = makeRDataAllocated(memorySizeOfRVM);
        if(object->memory != nil) {
            object->classId = registerClassOnce(toString(RVirtualMachine));
        }
    }
    return object;
}

destructor(RVirtualMachine) {
    deleter(object->memory, RData);
}

method(void, setUpDataBlock, RVirtualMachine)) {
    if(object->functionExecuting != nil) {
        // set-up break flag
        size_t iterator;
        object->breakFlag = no;
        $(object->memory, m(flushAllToByte, RData)), 0x00);

        flushAllToByte((byte *) &object->REGS, sizeof(size_t) * registersCountOfRVM, 0x00);

        forAll(iterator, registersCountOfRVM) {
            object->REGS_B[iterator] = (byte *) &object->REGS[iterator];
            object->REGS_W[iterator] = (u16 *) &object->REGS[iterator];
            object->REGS_D[iterator] = (unsigned int *) &object->REGS[iterator];
        }

    } elseWarning(
        RWarning("RVM. Set-up function is nil.", object)
    )
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
            while(*(size_t*)object->dataRegister != 0) {
                ++object->dataRegister;
                ++object->tickCount;
            }
            ++object->command;
        } break;

        case r_print_char : {
            if(*object->dataRegister == '\n'
                    || *object->dataRegister == '\r'
                    || *object->dataRegister == '\t') {
                RPrintf("%c", (byte) *object->dataRegister);
            } else if(*object->dataRegister < 32
                    || *object->dataRegister > 127) {
                RPrintf("%02x ", (byte) *object->dataRegister);
            } else {
                RPrintf("%c", (byte) *object->dataRegister);
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
            if(object->dataRegister == (object->memory->data + object->memory->size - 1)) {
                object->dataRegister = object->memory->data;
            }
            ++object->command;
        } break;

        case r_move_backward : {
            // decrement pointer
            --object->dataRegister;
            if(object->dataRegister < object->memory->data) {
                object->dataRegister = object->memory->data + object->memory->size - 1;
            }
            ++object->command;
        } break;

        case r_goto_address : {
            // set pointer to command incremented pointers data, like JMP address, fixme when instruction in data
            size_t absoluteAddress;
            memcpy(&absoluteAddress, ++object->command, sizeof(size_t));
            object->command = object->functionStartAddress + absoluteAddress;
        } break;

// logical
        case r_if : {
            if((*object->dataRegister) != 0) {
                // true instruction
                object->command += 2 + sizeof(size_t); // cause of goto arg byte
            } else {
                // false instruction
                ++object->command;
            }
        } break;

        case r_if_not : {
            if((*object->dataRegister) == 0) {
                // true instruction
                object->command += 2 + sizeof(size_t); // cause of goto arg byte
            } else {
                // false instruction
                ++object->command;
            }
        } break;

        case r_no : {
            *object->dataRegister = ~(*object->dataRegister);
            ++object->command;
        } break;

// debug
        case r_break : {
            object->breakFlag = yes;
            return 1;
        }

// work end
        case r_end : {
            RPrintf("\nEnd work of RVM.\n");
            return 1;
        }
// math
        case r_addition: {
            object->command += 1;
            byte addresses = *object->command;
            byte first = addresses >> 4 & (byte) 0x0F;
            byte second = addresses & (byte) 0x0F;
            object->REGS[first] += object->REGS[second];

            object->command += 1;
        } break;


// bad situation
        default: {
            RPrintf("EROOR. RVM. Warning, default case, unhalted result\n");
            return 1;
        }
    }
    return 0;
}

method(void, visualize, RVirtualMachine), rbool end) {
#ifdef VISUALIZE
    size_t iterator;
    static WINDOW *outputWindow = nil;
    static rbool initialized = no;
    if(!initialized) {
        initscr();
        curs_set(yes);

    #ifdef COLORED
        start_color();
        use_default_colors();

        forAll(iterator, COLORS) {
            init_pair((short) (iterator + 1), (short) iterator, -1);
        }
    #endif

        // set up initial windows
        outputWindow = newwin(64,128, 0, 0);
        initialized = yes;
    }
    wmove(outputWindow, 0, 0);

    forAll(iterator, object->memory->size) {
    #ifdef COLORED
        wattron(outputWindow, COLOR_PAIR(object->memory->data[iterator] + 1));
        wprintw(outputWindow, "# ");
    #else
        wprintw(outputWindow, "%02X", object->memory->data[iterator]);
    #endif
    }
#ifdef COLORED
    wattron(outputWindow, COLOR_PAIR(1));
#endif

    wrefresh(outputWindow);
    if(end) {
        // cleanup
        delwin(outputWindow);
        endwin();
    }
#endif /*VISUALIZE*/
}

method(void, setUpFunction, RVirtualMachine), RVirtualFunction *function) {
    // copy pointer
    object->functionExecuting = function;

    // set tick size is 0
    object->tickCount = 0;

    $(object, m(setUpDataBlock, RVirtualMachine)) );

    // set data register as pointer to first element of memory
    object->dataRegister = object->memory->data;

    // set command to first byte of opcodes
    object->functionStartAddress = master(object->functionExecuting, RData)->data;
    object->command              = object->functionStartAddress;
}

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function) {
    size_t result = 0;
    size_t iterator;

    $(object, m(setUpFunction, RVirtualMachine)), function);

    // function name label
    RPrintf("RVM. Start Executing Function - ");
    p(RString)(function->name);
    RPrintf("\n");

    // execute first code, that starts processing
    // execute next code while not flag
    while(result != 1) {
        result = $(object, m(executeCode, RVirtualMachine)));
        // increment ticks
        ++object->tickCount;
        // break
        if(object->breakFlag) {
            RPrintf("RVM break at %lu tick.", object->tickCount);
        } else {
            $(object, m(visualize, RVirtualMachine)), no);
        #if msecDelay != 0
            usleep(msecDelay * 1000);
        #endif
        }
    }

    $(object, m(visualize, RVirtualMachine)), yes);

    // at end of processing print analytics
    RPrintf("\nRVM. End Executing Function - ");
    p(RString)(function->name);
    RPrintf("\n");

    RPrintf("Ticks count for executing is - %lu\n", object->tickCount);

    RPrintf("Registers : {\n");
    forAll(iterator, registersCountOfRVM) {
        RPrintf("    REG %u : %lu - ", (unsigned int) iterator, object->REGS[iterator]);
        printByteArrayInHex((const byte *) &object->REGS[iterator], sizeof(size_t));
    }
    RPrintf("}\n");

    RPrintf("Memory snapshot : {\n");
    $(object->memory, p(RData)) );
    RPrintf("} end memory snapshot\n");
}

singleton(RVirtualMachine) {
    static RVirtualMachine *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RVirtualMachine)) );
    }
    return instance;
}