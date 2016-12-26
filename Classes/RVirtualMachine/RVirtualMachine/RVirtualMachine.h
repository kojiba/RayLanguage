#ifndef __R_VIRTUAL_MACHINE_H__
#define __R_VIRTUAL_MACHINE_H__

#include <RayFoundation/RayFoundation.h>
#include "../RVirtualFunction/RVirtualFunction.h"

// rasm - ray assembler for RVM

typedef enum RVirtualCodes {

    r_end,                  // work-end-code

// algebra operations (address 1, address 2 one of 16 regs, result store in first reg)
    r_addition,
    r_addition_b,
    r_addition_w,
    r_addition_d,

    r_subtraction,
    r_multiplication,
    r_division,

    r_copy_to_reg,
    r_copy,

// binary operations (address 1, address 2)
    r_xor,
    r_and,
    r_or,

    r_rotate_left,
    r_rotate_right,

// one arg (dataRegister)
    r_increment,
    r_decrement,
    r_no,

// brain-fuck move ups
    r_move_forward,
    r_move_backward,

// jump to a difference
    r_goto_address,        // jump (address value) sets command register to address

// debug
    r_break,

// io
    r_print_0_string,      // prints all to 0, cause errors
    r_print_char,          // print simple ASCII char or byte
    r_print_pascal_string, // prints buffer like string, length first

    r_get_char,
    r_get,

// if (dataRegister)
    r_if,                  // if (false_instruction, reserved size_t, true_instruction) value == 0,
                           // then false_instruction, else true_instruction

    r_if_not,               // if_not (false_instruction , true_instruction) value != 0,
                            // then false_instruction, else true_instruction

    r_ignore,               // code for ignoring, not byte-code

    r_virtual_codes_count   // last parameter, size of opcodes

} RInstructions;

static const size_t memorySizeOfRVM = 4096; // in bytes
#define registersCountOfRVM 16 // in bytes

class(RVirtualMachine)
    RData            *memory;               // memory memorySizeOfRVM size
    RVirtualFunction *functionExecuting;    // pointer to function

    byte             *dataRegister;         // pointer to memory element               (data    segment)

    byte            *command;              // pointer to rasm byte-code               (command ptr)
    byte            *functionStartAddress; // pointer to place, where function starts (program ptr)

    size_t          tickCount;
    rbool           breakFlag;              // for stop


    /*
     * +--------------------------------+
     * |R1B  8| R1W  16| R1D          32|
     * +--------------------------------+
     */

    size_t REGS[registersCountOfRVM]; // registers

    byte *REGS_B[registersCountOfRVM];     // pointers to byte registers
    u16 *REGS_W[registersCountOfRVM];      // pointers to u16 registers
    unsigned *REGS_D[registersCountOfRVM]; // pointers to u32 registers

endOf(RVirtualMachine)

constructor (RVirtualMachine));
destructor  (RVirtualMachine);
singleton   (RVirtualMachine);

// main method, to execute byte-code function
method(void,     executeFunction, RVirtualMachine),    RVirtualFunction *function);

#define RVM singletonCall(RVirtualMachine)
#define deleteRVM() $(RVM, d(RVirtualMachine)) ); \
                    deallocator(RVM)
#define executeRay(RVirtualFunction) $(RVM, m(executeFunction, RVirtualMachine)), RVirtualFunction)

#endif /*__R_VIRTUAL_MACHINE_H__*/