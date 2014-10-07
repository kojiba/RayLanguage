#ifndef __R_VIRTUAL_MACHINE_H__
#define __R_VIRTUAL_MACHINE_H__

#include "../../RayFoundation/RayFoundation.h"
#include "../../RayFoundation/RayFoundation.h"
#include "../../RayFoundation/RArray/RArray.h"
#include "../RVirtualFunction/RVirtualFunction.h"
#include "../../RayFoundation/RByteOperations/RByteOperations.h"

// rasm - ray assembler for RVM

typedef enum RVirtualCodes {

    r_string_end = 0,       // reserved for 0-terminated strings

    r_modifies_input,
    r_data_start,
    r_end,
    r_function_begin,
    r_const_start,
    r_const_end,
    r_ignore,               // code for ignoring, not byte-code
    r_error,

// algebra operations
    r_addition,
    r_subtraction,
    r_multiplication,
    r_division,

// binary operations
    r_xor,
    r_and,
    r_or,
    r_no,

// one arg
    r_increment,
    r_decrement,

// brain-fuck move ups
    r_move_forward,
    r_move_backward,

// jump to a difference
    r_goto_address,        // jump (address) sets command register to address

// io
    r_print_0_string,      // prints all to 0, cause errors
    r_print_char,          // print simple ASCII char
    r_print_pascal_string, // prints buffer like string, length first

    r_get_char,
    r_get,

// if
    r_if,                  // if (false_instruction , true_instruction) value in dataRegister == 0,
                           // then false_instruction, else true_instruction

} RInstructions;


class(RVirtualMachine)
    RByteArray       *memory;
    RVirtualFunction *functionExecuting;

    byte             *dataRegister;      // pointer to memory element
    byte             *command;           // pointer to rasm byte-code

    uint64_t          tickCount;

endOf(RVirtualMachine)

constructor (RVirtualMachine));
destructor  (RVirtualMachine);
singleton   (RVirtualMachine);

method(void, executeFunction, RVirtualMachine),    RVirtualFunction *function);
method(void, executeCode, RVirtualMachine));

method(void, setUpDataBlock,  RVirtualMachine));

#define RVM singletonCall(RVirtualMachine)
#define deleteRVM() $(RVM, d(RVirtualMachine)) ); \
                    deallocator(RVM)
#define executeRay(RVirtualFunction) $(RVM, m(executeFunction, RVirtualMachine)), RVirtualFunction)

#endif /*__R_VIRTUAL_MACHINE_H__*/