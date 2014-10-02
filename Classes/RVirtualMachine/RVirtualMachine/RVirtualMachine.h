#ifndef __R_VIRTUAL_MACHINE_H__
#define __R_VIRTUAL_MACHINE_H__

#include "../../Classes/RayFoundation/RayFoundation.h"
#include "../../Classes/RayFoundation/RayFoundation.h"
#include "../../Classes/RayFoundation/RArray/RArray.h"
#include "../RVirtualFunction/RVirtualFunction.h"

typedef enum RVirtualCodes {
//
    r_modifies_input,
    r_end,
    r_data_start,
    r_function_begin,
    r_const_start,
    r_const_end,
    r_ignore,
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

// jump to a difference
    r_jump_address,

// io
     r_print,
    r_get,

// if
    r_if,
    r_while,

} RInstructions;


class(RVirtualMachine)
    RArray           *memory;
    RVirtualFunction *functionExecuting;

endOf(RVirtualMachine)

constructor(RVirtualMachine));
destructor(RVirtualMachine);

method(void, executeFunction, RVirtualMachine), RVirtualFunction *function);

method(void, setUpMemory,     RVirtualMachine));
method(void, setUpDataBlock,  RVirtualMachine));

#endif /*__R_VIRTUAL_MACHINE_H__*/