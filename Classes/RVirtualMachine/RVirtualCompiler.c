#include <CoreFoundation/CoreFoundation.h>
#include "RVirtualCompiler.h"
#include "RVirtualMachine/RVirtualMachine.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

constructor(RVirtualCompiler)) {
    object = allocator(RVirtualCompiler);
    if(object != NULL) {

        object->classId = registerClassOnce(toString(RVirtualCompiler));
        object->lines = 1;
        object->symbols = 1;
        object->numberOfLines = 0;
    }
    return object;
}

destructor(RVirtualCompiler) {
    if(object != NULL) {
        $(object->code, d(RCString)) );
        deallocator(object->code);

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
    RByteArray *body;
    byte character;
    object->iterator = 0;
    object->iteratorShift = 0; // shift cause '[' and ']' 3x multiplience
    uint64_t sizeOfByteCode;

    // all [, ] instructions will be doubled in the byte-code,
    // because of r_if instruction build
    object->forwardRepetitions = $(object->code, m(numberOfRepetitions, RCString)), '[');
    object->backwardRepetitions = $(object->code, m(numberOfRepetitions, RCString)), ']');

    // removing all '\n' from byte-code, +1 to r_end
    sizeOfByteCode = object->code->size - object->numberOfLines + 1 + 2 * (object->forwardRepetitions + object->backwardRepetitions);

    body = makeRByteArray(sizeOfByteCode);

    // set pointer to body
    object->body = body;

    while(object->iterator < object->code->size) {
        character = $(object, m(brainFuckSourceToByteCode, RVirtualCompiler)));

        if(character != r_ignore) {
            body->array[object->iterator + object->iteratorShift] = character;
        }

        ++object->iterator;
    }

    body->array[object->iterator + object->iteratorShift] = r_end;

    return body;
}

method(byte, brainFuckSourceToByteCode, RVirtualCompiler)) {
    byte byteCode;
    static uint64_t deltaToNext = 0;
    static uint64_t toPrev = 0;

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

        case '[': {
            // complicated case
            uint64_t realPath;

            deltaToNext = indexOfLastCharacterCString(&object->code->baseString[object->iterator + object->iteratorShift], object->code->size - deltaToNext, ']');

            --object->forwardRepetitions;
            realPath = object->iterator + object->iteratorShift + deltaToNext + (object->forwardRepetitions + object->backwardRepetitions) * 2;

            if(realPath > 255) {
                RPrintf("RVC. Error. '[' Too long loop\n");
            }

            object->body->array[object->iterator + object->iteratorShift] = r_if;
            object->body->array[object->iterator + object->iteratorShift + 1] = r_goto_address;
            object->body->array[object->iterator + object->iteratorShift + 2] = realPath;

            object->iteratorShift += 2;
            byteCode = r_ignore;
        } break;

        case ']': {
            // complicated case
            uint64_t realPath;

            toPrev = indexOfLastCharacterCString(object->code->baseString, toPrev ? toPrev : object->code->size, '[');
            --object->backwardRepetitions;
            realPath = toPrev + (object->forwardRepetitions + object->backwardRepetitions) * 2;

            if(realPath > 255) {
                RPrintf("RVC. Error. ']' Too long loop\n");
            }

            object->body->array[object->iterator + object->iteratorShift] = r_if_not;
            object->body->array[object->iterator + object->iteratorShift + 1] = r_goto_address;
            object->body->array[object->iterator + object->iteratorShift + 2] = realPath;

            object->iteratorShift += 2;
            byteCode = r_ignore;
        } break;

        case '\n': {
            ++object->lines;
            object->symbols = 1;
            --object->iteratorShift;
            byteCode = r_ignore;
        } break;

        case ' ': {
            --object->iteratorShift;
            byteCode = r_ignore;
        } break;

        default: {
            byteCode = r_end;
            RPrintf("Error parsing Brainuck on line: %qu, symbol: %qu !\n", object->lines, object->symbols);
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

        RPrintf("RVC. Brainfuck. Processed lines - %qu of %qu, in %qu iterations \n", object->lines, object->numberOfLines + 1, object->iterator);

        // print result for debug
        $(function, p(RVirtualFunction)) );
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