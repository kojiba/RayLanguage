#ifndef __R_VIRTUAL_MACHINE_H__
#define __R_VIRTUAL_MACHINE_H__

#include "../../RayFoundation/RayFoundation.h"
#include "../RVirtualFunction/RVirtualFunction.h"

// rasm - ray assembler for RVM

typedef enum RVirtualCodes {

    r_end,                  // work-end-code
    r_function_begin,
    r_const_start,
    r_const_end,
    r_ignore,               // code for ignoring, not byte-code
    r_error,

// algebra operations (address 1, address 2)
    r_addition,
    r_subtraction,
    r_multiplication,
    r_division,

// binary operations (address 1, address 2)
    r_xor,
    r_and,
    r_or,


// one arg (dataRegister)
    r_increment,
    r_decrement,
    r_no,

// brain-fuck move ups
    r_move_forward,
    r_move_backward,

// jump to a difference
    r_goto_address,        // jump (address value) sets command register to address

// io
    r_print_0_string,      // prints all to 0, cause errors
    r_print_char,          // print simple ASCII char or byte
    r_print_pascal_string, // prints buffer like string, length first

    r_get_char,
    r_get,

// if (dataRegister)
    r_if,                  // if (false_instruction, reserved byte, true_instruction) value == 0,
                           // then false_instruction, else true_instruction

    r_if_not,               // if_not (false_instruction , true_instruction) value != 0,
                            // then false_instruction, else true_instruction

    r_virtual_codes_count   // last parameter, count of opcodes

} RInstructions;

static const size_t memorySizeOfRVM = 1024;

class(RVirtualMachine)
    RByteArray       *memory;               // memory 1 kB size
    RVirtualFunction *functionExecuting;    // pointer to function

    byte             *dataRegister;         // pointer to memory element
    byte             *command;              // pointer to rasm byte-code
    byte             *functionStartAddress; // pointer to place, where function starts

    size_t          tickCount;
    byte              breakFlag;            // for force quit

endOf(RVirtualMachine)

constructor (RVirtualMachine));
destructor  (RVirtualMachine);
singleton   (RVirtualMachine);

// main method, to execute byte-code function
method(void,     executeFunction, RVirtualMachine),    RVirtualFunction *function);

// workings methods
method(size_t, executeCode,     RVirtualMachine));
method(void,     setUpDataBlock,  RVirtualMachine));

#define RVM singletonCall(RVirtualMachine)
#define deleteRVM() $(RVM, d(RVirtualMachine)) ); \
                    deallocator(RVM)
#define executeRay(RVirtualFunction) $(RVM, m(executeFunction, RVirtualMachine)), RVirtualFunction)

#endif /*__R_VIRTUAL_MACHINE_H__*/