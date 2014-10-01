#include "../Classes/RayFoundation/RayFoundation.h"
#include "RVirtualCompiler.h"

constructor(RVirtualCompiler)) {
    object = allocator(RVirtualCompiler);
    if(object != NULL) {
        object->lines = 1;
        object->column = 1;
    }
    return object;
}

destructor(RVirtualCompiler) {
    if(object->code != NULL) {
        $(object->code, d(RCString)) );
    }
}

method(RVirtualFunction *, createFunctionFromSourceCode, RVirtualCompiler), RCString *sourceCode) {

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(NULL, c(RVirtualFunction)) );

        // copy source to object
        object->code = $(sourceCode, m(copy, RCString)) );

        // set name and body
        function->name = $(object, m(getFunctionName, RVirtualCompiler)) );
        function->body = $(object, m(getFunctionBody, RVirtualCompiler)) );

        // fixme
        RPrintf("Processed lines - %qu\n", object->lines);
        return function;
    } else {
        RPrintf("Error. RVC. Bad virtual-code size\n");
    }
    return NULL;
}

method(RCString *, getFunctionName, RVirtualCompiler)) {
    if(object->code->size != 0){
        uint64_t counter = 0;

        // finding to startSymbol

        while(object->code->baseString[counter] != ':' && counter < object->code->size) {
            ++counter;
        }

        RCString *name = $(object->code, m(getSubstringInRange, RCString)), makeRRange(0, counter));

        $(object->code, m(deleteInRange, RCString)), makeRRange(0, counter));

        return name;
    }
    return NULL;
}

method(byte *, getFunctionBody, RVirtualCompiler)){
    byte *body = RAlloc(sizeof(byte) * object->code->size);
    byte character;
    uint64_t iterator = 0;
    character = r_ignore;

    while(iterator < object->code->size) {
        character = $(object, m(sourceToByteCode, RVirtualCompiler)), iterator);
        ++iterator;
        body[iterator] = character;
    }
//    fixme
    body[++iterator] = r_end;

    return body;
}

method(byte, sourceToByteCode, RVirtualCompiler), uint64_t iterator) {
    static uint64_t counter = 0;
    if(counter == 1) {
        if(object->code->baseString[iterator] != '\"') {
            return object->code->baseString[iterator];
        }
    }

    switch (object->code->baseString[iterator]) {
        case '\"': {
            counter = (counter + 1) % 2;
            if(counter == 1){
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

        case ' ': {
            ++object->column;
            return r_ignore;
        } break;

        case '\n': {
            ++object->lines;
            object->column = 0;
            return r_ignore;
        } break;

        default: {
            RPrintf("Error parsing on line: %qu column: %qu !\n", object->lines, object->column);
            return r_error;
        }
    }
}

singleton(RVirtualCompiler) {
    static RVirtualCompiler *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualCompiler)));
    }
    return instance;
}