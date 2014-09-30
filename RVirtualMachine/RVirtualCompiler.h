#include "../Classes/RayFoundation/RayFoundation.h"

#ifndef __R_VIRTUAL_COMPILER_H__
#define __R_VIRTUAL_COMPILER_H__

#include "../Classes/RayFoundation/RayFoundation.h"
#include "../Classes/RayFoundation/RCString/RCString.h"
#include "RVirtualFunction/RVirtualFunction.h"

class(RVirtualCompiler)
    RCString *code;
endOf(RVirtualCompiler)

constructor(RVirtualCompiler));
destructor(RVirtualCompiler);

method(RVirtualFunction *, createFunctionFromSourceCode, RVirtualCompiler), RCString *sourceCode);
method(char, characterToCode,RVirtualCompiler),                             char character);

method(RCString *, getFunctionNameFrom, RVirtualCompiler));
method(void, flushSourceCode, RVirtualCompiler));

singleton(RVirtualCompiler);

#define RVC singletonCall(RVirtualCompiler)

#endif /*__R_VIRTUAL_COMPILER_H__*/
