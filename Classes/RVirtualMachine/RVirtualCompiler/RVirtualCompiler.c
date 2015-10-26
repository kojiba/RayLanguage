#include "RVirtualCompiler.h"
#include "../RVirtualMachine/RVirtualMachine.h"

constructor(RVirtualCompiler)) {
    object = allocator(RVirtualCompiler);
    if(object != nil) {
        object->brakets = makeRArray();
        if(object->brakets != nil) {
            object->classId = registerClassOnce(toString(RVirtualCompiler));
        } else {
            deallocator(object);
            return nil;
        }
    }
    return object;
}

destructor(RVirtualCompiler) {
    deleter(object->brakets, RArray);
}

singleton(RVirtualCompiler) {
    static RVirtualCompiler *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RVirtualCompiler)));
    }
    return instance;
}

method(RString *, getFunctionName, RVirtualCompiler)) {
    if(object->code->size != 0) {
        size_t place;

        // finding to startSymbol
        place = indexOfFirstCharacterCString(object->code->baseString, object->code->size, ':');

        // if there is ':'
        if(place != object->code->size) {

            // get copy of substring
            RString *name = $(object->code, m(substringInRange, RString)), makeRRange(0, place));

            // delete nameString start sourceCode and ':' symbol
            $(object->code, m(deleteInRange, RString)), makeRRange(0, place + 1));
            return name;
        }
    }
    return nil;
}

#pragma mark Brainfuck lang to rasm

method(byte, brainFuckSourceToByteCode, RVirtualCompiler)) {
    static rbool isOpenedBracket = no;
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
            isOpenedBracket = yes;
            // complicated case
            size_t realPath, deltaToFirstBack,
                    currSubSize  = object->code->size - object->deltaToNext,
                   currSubStart = object->iterator + object->iteratorShift + 1;

            object->deltaToNext = indexOfLastCharacterCString(&object->code->baseString[currSubStart], currSubSize, ']');
            deltaToFirstBack    = indexOfFirstCharacterCString(&object->code->baseString[currSubStart], currSubSize, ']');

            if(deltaToFirstBack != currSubSize
                    && deltaToFirstBack != object->deltaToNext) {
                size_t deltaToFirstFront   = indexOfFirstCharacterCString(&object->code->baseString[currSubStart], currSubSize, '[');

                // while we see code like - [ []  []  [] ]
                if(deltaToFirstFront != currSubSize &&
                        deltaToFirstFront > deltaToFirstBack) {
                    object->deltaToNext = deltaToFirstBack;
                }
            }
            $(object->brakets, m(addObject, RArray)), (pointer) object->iterator);

            realPath = object->iterator + object->iteratorShift + object->deltaToNext + (object->forwardRepetitions + object->backwardRepetitions) * (1 + sizeof(size_t)) + 2;

            object->body->data[object->iterator + object->iteratorShift]     = r_if;
            object->body->data[object->iterator + object->iteratorShift + 1] = r_goto_address;
            memcpy(object->body->data + object->iterator + object->iteratorShift + 2, &realPath, sizeof(size_t));

            --object->forwardRepetitions;


            object->iteratorShift += 1 + sizeof(size_t);
            byteCode = r_ignore;
        } break;

        case ']': {
            size_t realPath;
            if(!isOpenedBracket) {
                RPrintf("Warning. RVirtualCompiler (BrainFuck). ']' not opened\n");
                return r_ignore;
            }

            realPath = (size_t) $(object->brakets, m(lastObject, RArray)));
            $(object->brakets, m(deleteLast, RArray)));

            --object->backwardRepetitions;

            if(realPath > 255) {
                RPrintf("Warning. RVirtualCompiler (BrainFuck). ']' Too long loop %lu\n", realPath);
            }

            object->body->data[object->iterator + object->iteratorShift]     = r_if_not;
            object->body->data[object->iterator + object->iteratorShift + 1] = r_goto_address;
            memcpy(object->body->data + object->iterator + object->iteratorShift + 2, &realPath, sizeof(size_t));

            object->iteratorShift += 1 + sizeof(size_t);
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
            RPrintf("Warning. RVirtualCompiler (BrainFuck). Undefined symbol on line: %lu, place: %lu !\n",
                                                                            object->lines, object->symbols);
        }
    }

    ++object->symbols;
    return byteCode;
}

method(RData *, getBrainFuckFunctionBody, RVirtualCompiler)) {
    RData *body;
    byte        character;
    size_t      sizeOfByteCode;

    // delete spaces
    $(object->code, m(deleteAllCharacters, RString)), ' ');
    object->numberOfLines = $(object->code, m(numberOfCharacters, RString)), '\n');

    // all [, ] instructions will be doubled in the byte-code,
    // because of r_if instruction build
    object->forwardRepetitions  = $(object->code, m(numberOfCharacters, RString)), '[');
    object->backwardRepetitions = $(object->code, m(numberOfCharacters, RString)), ']');

    if(object->forwardRepetitions != object->backwardRepetitions) {
        RPrintf("Warning. RVirtualCompiler (BrainFuck). Count of \'[\' and \']\' isn't equal!");
    }

    // removing all '\n' start byte-code, +1 to r_end
    sizeOfByteCode = object->code->size - object->numberOfLines + 1 + (1 + sizeof(size_t)) * (object->forwardRepetitions + object->backwardRepetitions);

    body = allocator(RData);
    body->data = nil;
    body->size = sizeOfByteCode;

    // set pointer to body
    object->body = body;

    while(object->iterator < object->code->size) {
        character = $(object, m(brainFuckSourceToByteCode, RVirtualCompiler)));

        if(character != r_ignore) {
            body->data[object->iterator + object->iteratorShift] = character;
        }

        ++object->iterator;
    }

    body->data[object->iterator + object->iteratorShift] = r_end;

    return body;
}

method(RVirtualFunction *, createFunctionFromBrainFuckSourceCode, RVirtualCompiler), const RString *sourceCode) {

    if(sourceCode->size != 0) {
        RVirtualFunction *function = $(nil, c(RVirtualFunction)) );

        // copy source to object
        object->code = $(sourceCode, m(copy, RString)) );

        // init workings
        object->deltaToNext   = 0;
        object->iterator      = 0;
        object->lines         = 0;
        object->iteratorShift = 0; // shift cause '[' and ']' 3x multiplience
        $(object->brakets, m(flush, RArray)));

        // set name and body
        function->name               = $(object, m(getFunctionName,          RVirtualCompiler)) );
        master(function, RData) = $(object, m(getBrainFuckFunctionBody, RVirtualCompiler)) );

        if(function->name == nil) {
            function->name = RSC("Unnamed");
        }
        deleter(object->code, RString);

        RPrintf("RVirtualCompiler. Brainfuck. Processed lines - %lu of %lu, in %lu iterations \n", object->lines, object->numberOfLines + 1, object->iterator);
        // print result for debug
        //  $(function, p(RVirtualFunction)) );
        return function;
    } else {
        RPrintf("Error. RVirtualCompiler. Bad virtual-code size\n");
    }
    return nil;
}