/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation/RayFoundation.h>
#include "Tests.h"

#include "RVirtualMachine/RVirtualFunction/RVirtualFunction.h"
#include "RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h"
#include "RVirtualMachine/RVirtualMachine/RVirtualMachine.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();


    // [ ] ,  [ [ [ ] ] ],  [ [ ] [] [ ] ]
    const RString *source = RS(" ++++++++++ [ >+++++++>++++++++++>+++>+<<<<- ] >++\n"
                                  " .>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.\n"
                                  " ------.--------.>+.>.");

    RVirtualFunction *function = c(RVirtualFunction)(nil);
    function->name = nil;
    function->masterRDataObject = makeRDataAllocated(5);
    function->masterRDataObject->data[0] = r_addition;
    function->masterRDataObject->data[1] = 0x01;

    function->masterRDataObject->data[2] = r_addition;
    function->masterRDataObject->data[3] = 0x23;

    function->masterRDataObject->data[4] = r_end;

    // brainfuck hard(with [, ]) hello world on RVM
//    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)), source );

    p(RVirtualFunction)(function);

    executeRay(function);

    deleter(function, RVirtualFunction);
    deleter(RVM, RVirtualMachine);
    deleter(RVC, RVirtualCompiler);

    endRay();
}