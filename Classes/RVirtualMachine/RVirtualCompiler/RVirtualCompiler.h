#ifndef __R_VIRTUAL_COMPILER_H__
#define __R_VIRTUAL_COMPILER_H__

#include "../../RayFoundation/RayFoundation.h"
#include "../../RayFoundation/RCString/RCString.h"
#include "../RVirtualFunction/RVirtualFunction.h"

class(RVirtualCompiler)

    RCString   *code;          // copy of source-code

    // iterators for counting
    uint64_t    lines;
    uint64_t    numberOfLines;
    uint64_t    symbols;

    // for code processing
    uint64_t    iterator;
    uint64_t    iteratorShift;
    uint64_t    forwardRepetitions;
    uint64_t    backwardRepetitions;

    // workers
    uint64_t    deltaToNext;
    uint64_t    toPrev;

    RByteArray *body;          // pointer to byte-code, that will be returned

endOf(RVirtualCompiler)

constructor(RVirtualCompiler));
destructor(RVirtualCompiler);

method(RCString *,         getFunctionName,                       RVirtualCompiler));

// brainfuck lang
method(RByteArray*,        getBrainFuckFunctionBody,              RVirtualCompiler));
method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler),    const RCString *sourceCode); // copies source
method(byte,               brainFuckSourceToByteCode,             RVirtualCompiler));


singleton(RVirtualCompiler);

#define RVC singletonCall(RVirtualCompiler)

#endif /*__R_VIRTUAL_COMPILER_H__*/
