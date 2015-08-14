#ifndef __R_VIRTUAL_COMPILER_H__
#define __R_VIRTUAL_COMPILER_H__

#include <RayFoundation/RayFoundation.h>
#include "../RVirtualFunction/RVirtualFunction.h"

class(RVirtualCompiler)

    RCString   *code;          // copy of source-code

    // iterators for counting
    size_t      lines;
    size_t      numberOfLines;
    size_t      symbols;

    // for code processing
    size_t      iterator;
    size_t      iteratorShift;
    size_t      forwardRepetitions;
    size_t      backwardRepetitions;

    // workers
    size_t      deltaToNext;

    // for store open brackets places
    RArray     *brakets;

    RByteArray *body;          // pointer to byte-code, that will be returned

endOf(RVirtualCompiler)

constructor (RVirtualCompiler));
destructor  (RVirtualCompiler);
singleton   (RVirtualCompiler);

// brainfuck lang
method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler),    const RCString *sourceCode); // copies source

#define RVC singletonCall(RVirtualCompiler)

#endif /*__R_VIRTUAL_COMPILER_H__*/
