#include "../Classes/RayFoundation/RayFoundation.h"
#include "RVirtualCompiler.h"

constructor(RVirtualCompiler)){
    object = allocator(RVirtualCompiler);
    if(object != NULL) {

    }
    return object;
}

destructor(RVirtualCompiler) {
    if(object->code != NULL) {
        $(object->code, d(RCString)) );
    }
}

method(RVirtualFunction *, createFunctionFromSourceCode, RVirtualCompiler), RCString *sourceCode){

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(NULL, c(RVirtualFunction)) );
        function->name = $(object, m(getFunctionNameFrom, RVirtualCompiler)), sourceCode);
        // fixme
        return function;
    } else {
        RPrintf("Error. Bad virtual-code size");
    }
    return NULL;
}

method(RCString *, getFunctionNameFrom, RVirtualCompiler), RCString *sourceCode){
    uint64_t counter = 0;
    // finding to startSymbol
    while(characterToCodeRVirtualCompiler(sourceCode->baseString[counter]) != r_function_begin && counter < sourceCode->size) {
        ++counter;
    }
    RCString *name = $(sourceCode, m(getSubstringInRange, RCString)), makeRRange(0, counter));
    return name;
}


char characterToCodeRVirtualCompiler(char character){
    switch (character) {
        case '\"': {
            static uint64_t counter = 0;
            counter = (counter + 1) % 2;
            if(counter == 0){
                return r_const_start;
            } else {
                return r_const_end;
            }
        } break;

        case 'p': {
            return r_print;
        } break;

        case ':': {
            return r_function_begin;
        } break;
    }
}

singleton(RVirtualCompiler){
    static RVirtualCompiler *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualCompiler)));
    }
    return instance;
}