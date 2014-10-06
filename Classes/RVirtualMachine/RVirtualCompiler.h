#ifndef __R_VIRTUAL_COMPILER_H__
#define __R_VIRTUAL_COMPILER_H__

#include "../RayFoundation/RayFoundation.h"
#include "../RayFoundation/RCString/RCString.h"
#include "RVirtualFunction/RVirtualFunction.h"

class(RVirtualCompiler)

    RCString *code;
    RArray   *dataBlock;
    uint64_t  lines;
    uint64_t  column;
    uint64_t iterator;

endOf(RVirtualCompiler)

constructor(RVirtualCompiler));
destructor(RVirtualCompiler);

method(RCString *,         getFunctionName,                       RVirtualCompiler));
method(byte *,             getFunctionBody,                       RVirtualCompiler));

// brainfuck lang
method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler),    RCString *sourceCode);
method(byte,               brainFuckSourceToByteCode,             RVirtualCompiler));


singleton(RVirtualCompiler);

#define RVC singletonCall(RVirtualCompiler)

#endif /*__R_VIRTUAL_COMPILER_H__*/
