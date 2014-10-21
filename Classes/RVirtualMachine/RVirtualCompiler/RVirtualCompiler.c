#include "RVirtualCompiler.h"
#include "../RVirtualMachine/RVirtualMachine.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

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
    if(object->code->size != 0) {
        uint64_t place;

        // finding to startSymbol
        place = indexOfFirstCharacterCString(object->code->baseString, object->code->size, ':');

        // if there is ':'
        if(place != object->code->size) {

            // get copy of substring
            RCString *name = $(object->code, m(substringInRange, RCString)), makeRRange(0, place));

            // delete nameString from sourceCode and ':' symbol
            $(object->code, m(deleteInRange, RCString)), makeRRange(0, place + 1));
            return name;
        }
    }
    return NULL;
}

#pragma mark Brainfuck lang to rasm

method(RByteArray *, getBrainFuckFunctionBody, RVirtualCompiler)) {
    RByteArray *body;
    byte        character;
    uint64_t    sizeOfByteCode;
    object->iterator      = 0;
    object->iteratorShift = 0; // shift cause '[' and ']' 3x multiplience

    // delete spaces
    $(object->code, m(deleteAllCharacters, RCString)), ' ');
    object->numberOfLines = $(object->code, m(numberOfRepetitions, RCString)), '\n');

    // all [, ] instructions will be doubled in the byte-code,
    // because of r_if instruction build
    object->forwardRepetitions  = $(object->code, m(numberOfRepetitions, RCString)), '[');
    object->backwardRepetitions = $(object->code, m(numberOfRepetitions, RCString)), ']');

    if(object->forwardRepetitions != object->backwardRepetitions) {
        RPrintf("Warning. RVC (BrainFuck). Count of \'[\' and \']\' isn't equal!");
    }

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

            object->deltaToNext = indexOfLastCharacterCString(&object->code->baseString[object->iterator + object->iteratorShift], object->code->size - object->deltaToNext, ']');
            --object->forwardRepetitions;
            realPath = object->iterator + object->iteratorShift + object->deltaToNext + (object->forwardRepetitions + object->backwardRepetitions) * 2;

            if(realPath > 255) {
                RPrintf("Warning. RVC (BrainFuck). '[' Too long loop\n");
            }

            object->body->array[object->iterator + object->iteratorShift]     = r_if;
            object->body->array[object->iterator + object->iteratorShift + 1] = r_goto_address;
            object->body->array[object->iterator + object->iteratorShift + 2] = realPath;

            object->iteratorShift += 2;
            byteCode = r_ignore;
        } break;

        case ']': {
            // complicated case
            uint64_t realPath;

            object->toPrev = indexOfLastCharacterCString(object->code->baseString, object->toPrev ? object->toPrev : object->code->size, '[');
            --object->backwardRepetitions;
            realPath = object->toPrev + (object->forwardRepetitions + object->backwardRepetitions) * 2;

            if(realPath > 255) {
                RPrintf("Warning. RVC (BrainFuck). ']' Too long loop\n");
            }

            object->body->array[object->iterator + object->iteratorShift]     = r_if_not;
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
            RPrintf("ERROR. RVC (BrainFuck). Undefined symbol on line: %qu, place: %qu !\n", object->lines, object->symbols);
        }
    }

    ++object->symbols;
    return byteCode;
}

method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler), const RCString *sourceCode) {

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(NULL, c(RVirtualFunction)) );

        // copy source to object
        object->code = $(sourceCode, m(copy, RCString)) );

        // init workings
        object->deltaToNext = 0;
        object->toPrev      = 0;

        // set name and body
        function->name               = $(object, m(getFunctionName,          RVirtualCompiler)) );
        master(function, RByteArray) = $(object, m(getBrainFuckFunctionBody, RVirtualCompiler)) );

        if(function->name == NULL) {
            function->name = RSC("Unnamed");
        }
        $(object->code, d(RCString)) );

        RPrintf("RVC. Brainfuck. Processed lines - %qu of %qu, in %qu iterations \n", object->lines, object->numberOfLines + 1, object->iterator);
        // print result for debug
        //  $(function, p(RVirtualFunction)) );
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