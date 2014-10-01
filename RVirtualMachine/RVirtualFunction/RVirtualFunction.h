#ifndef __R_VIRTUAL_FUNCTION_H__
#define __R_VIRTUAL_FUNCTION_H__

#include "../../Classes/RayFoundation/RayFoundation.h"
#include "../../Classes/RayFoundation/RArray/RArray.h"

typedef enum RVirtualCodes {
//
    r_modifies_input,
    r_end,
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

class(RVirtualFunction)
    RCString *name;
//    RArray   *memory;
//    RArray   *arguments;
//    uint64_t  numberOfArguments;
    byte   *body;
endOf(RVirtualFunction)

constructor(RVirtualFunction));
destructor(RVirtualFunction);

printer(RVirtualFunction);
method(void, executeWithArgs, RVirtualFunction), RArray *args);


#endif /*__R_VIRTUAL_FUNCTION_H__*/