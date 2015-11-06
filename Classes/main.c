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

#include <RVirtualMachine/RVirtualFunction/RVirtualFunction.h>
#include <RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h>
#include <RVirtualMachine/RVirtualMachine/RVirtualMachine.h>

#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();

    const RString *source = RS("Hard Hello world : ++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++\n"
                               " .>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.\n"
                               " ------.--------.>+.>.");


    // brainfuck hard(with [, ]) hello world on RVM
    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)), source );

    executeRay(function);

    deleter(function, RVirtualFunction);
    deleter(RVC, RVirtualCompiler);
    deleter(RVM, RVirtualMachine);

    endRay();
}