#include "RVirtualCompiler.h"
#include "RVirtualMachine/RVirtualMachine.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualCompiler)) {
    object = allocator(RVirtualCompiler);
    if(object != NULL) {

        object->dataBlock = makeRArray();
        if(object->dataBlock != NULL) {
            object->classId = registerClassOnce(toString(RVirtualCompiler));
            object->lines = 1;
            object->symbols = 1;
            object->numberOfLines = 0;
        }
    }
    return object;
}

destructor(RVirtualCompiler) {
    if(object != NULL) {
        $(object->code, d(RCString)) );
        deallocator(object->code);

        deleteRA(object->dataBlock);
    }
}

method(RCString *, getFunctionName, RVirtualCompiler)) {
    if(object->code->size != 0){
        uint64_t counter = 0;

        // finding to startSymbol
        while(object->code->baseString[counter] != ':' && counter < object->code->size) {
            ++counter;
        }

        // get copy of substring
        RCString *name = $(object->code, m(getSubstringInRange, RCString)), makeRRange(0, counter));

        // delete nameString from sourceCode and ':' symbol
        $(object->code, m(deleteInRange, RCString)), makeRRange(0, counter + 1));

        // delete spaces
        $(object->code, m(deleteAllCharacters, RCString)), ' ');
        object->numberOfLines = $(object->code, m(numberOfRepetitions, RCString)), '\n');
        return name;
    }
    return NULL;
}

#pragma mark Brainfuck lang to rasm

method(RByteArray *, getBrainFuckFunctionBody, RVirtualCompiler)) {
    RByteArray *body = makeRByteArray(object->code->size + 1 - object->numberOfLines);

    byte character;

    object->iterator = 0;

    while(object->iterator < object->code->size) {
        character = $(object, m(brainFuckSourceToByteCode, RVirtualCompiler)));
        if(character != r_ignore) {
            body->array[object->iterator] = character;
        }
        ++object->iterator;
    }

    body->array[++object->iterator] = r_end;

    return body;
}

method(byte, brainFuckSourceToByteCode, RVirtualCompiler)) {
    byte byteCode;
    switch (object->code->baseString[object->iterator]) {
        case '+': {
            byteCode = r_increment;
        } break;

        case '-': {
            byteCode = r_decrement;
        } break;

        case '.': {
            byteCode = r_print_char;
        } break;

        case '>': {
            byteCode = r_move_forward;
        } break;

        case '<': {
            byteCode = r_move_backward;
        } break;

        case ',': {
            byteCode = r_get_char;
        } break;

// fixme not working
        case '[': {
            byteCode = r_while;
        } break;

        case ']': {
            byteCode = r_end_while;
        } break;
// fixme end not working

        case '\n': {
            ++object->lines;
            object->symbols = 1;
            byteCode = r_ignore;
        } break;

        case ' ': {
            object->symbols = 1;
            byteCode = r_ignore;
        } break;

        default: {
            byteCode = r_end;
            RPrintf("Error parsing on line: %qu, symbol: %qu !\n", object->lines, object->symbols);
        }
    }

    ++object->symbols;
    return byteCode;
}

method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler), const RCString *sourceCode) {

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(NULL, c(RVirtualFunction)) );

        // copy source to object
        if(object->code != NULL) {
            $(object->code, d(RCString)) );
        }
        object->code = $(sourceCode, m(copy, RCString)) );

        // set name and body
        function->name = $(object, m(getFunctionName, RVirtualCompiler)) );
        master(function, RByteArray) = $(object, m(getBrainFuckFunctionBody, RVirtualCompiler)) );

        RPrintf("Processed lines - %qu of %qu, in %qu iterations \n", object->lines, object->numberOfLines + 1, object->iterator);
        return function;
    } else {
        RPrintf("Error. RVC. Bad virtual-code size\n");
    }
    return NULL;
}

singleton(RVirtualCompiler) {
    static RVirtualCompiler *instance = NULL;
    if(instance == NULL) {
        instance = $(NULL, c(RVirtualCompiler)));
    }
    return instance;
}