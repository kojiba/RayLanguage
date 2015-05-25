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

#include <RayFoundation.h>
#include <unistd.h>
#include "Tests.h"

#include "RVirtualMachine/RVirtualFunction/RVirtualFunction.h"
#include "RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h"
#include "RVirtualMachine/RVirtualMachine/RVirtualMachine.h"

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    ComplexTest();
//    // [ ] ,  [ [ [ ] ] ],  [ [ ] [] [ ] ]
    RCString *source = RS(" Hello : +>[+"
                                  "[ > + <<<<<<<<< + >> + ] "
                                  "[ >> + <<<<<<<< + >> + ] "
                                  "[ >>> + <<<<<<< + >> + ] "
                                  "[ >>>> + <<<<<< + >> + ] "
                                  "[ >>>>> + <<<<< + >> + ] "
                                  "]>]");

    // brainfuck hard(with [, ]) hello world on RVM
    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)), source );

    p(RVirtualFunction)(function);

    executeRay(function);

    deleter(function, RVirtualFunction);
    deleter(RVM, RVirtualMachine);
    deleter(RVC, RVirtualCompiler);

    endRay();
}


