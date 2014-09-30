#include "../Classes/RayFoundation/RayFoundation.h"
#include "RVirtualCompiler.h"

constructor(RVirtualCompiler)) {
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

method(void, flushSourceCode, RVirtualCompiler)){
    //fixme delete from substring some symbols
}

method(RVirtualFunction *, createFunctionFromSourceCode, RVirtualCompiler), RCString *sourceCode) {

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(NULL, c(RVirtualFunction)) );

        // copy source to object
        object->code = $(sourceCode, m(copy, RCString)) );

        // delete all unused symbols
        $(object, m(flushSourceCode, RVirtualCompiler)) );

        // set name
        function->name = $(object, m(getFunctionNameFrom, RVirtualCompiler)), sourceCode);

        // fixme
        return function;
    } else {
        RPrintf("Error. RVC. Bad virtual-code size\n");
    }
    return NULL;
}

method(RCString *, getFunctionNameFrom, RVirtualCompiler)) {
    if(object->code->size != 0){
        uint64_t counter = 0;
        // finding to startSymbol
        while($(object, m(characterToCode, RVirtualCompiler)), object->code->baseString[counter]) != r_function_begin && counter < object->code->size) {
            ++counter;
        }
        RCString *name = $(object->code, m(getSubstringInRange, RCString)), makeRRange(0, counter));
        return name;
    }
    return NULL;
}


method(char, characterToCode, RVirtualCompiler), char character) {
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

singleton(RVirtualCompiler) {
    static RVirtualCompiler *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualCompiler)));
    }
    return instance;
}