/**
 * RInterpreter.c
 * A ray of light in the realm of darkness.
 * Interpreter of Ray language to pure C.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12.11.2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RInterpreter.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

constructor(RInterpreter)) {
    object = allocator(RInterpreter);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RInterpreter));
        // all to nil
        object->codeTokens       = nil;
        object->sourceFileString = nil;
        object->tokens           = nil;
        object->functions        = nil;
        object->globalVariables  = nil;
        object->typeDefs         = nil;
        object->consts           = nil;
    }
    return object;
}

destructor(RInterpreter) {
    if(object->sourceFileString != nil) {
        deleteRCS(object->sourceFileString);
    }
    if(object->tokens != nil) {
        deleteRA(object->tokens);
    }
    if(object->codeTokens != nil) {
        deleteRA(object->codeTokens);
    }
    if(object->consts != nil) {
        deleteRA(object->consts);
    }
}

singleton(RInterpreter) {
    static RInterpreter *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RInterpreter)));
    }
    return instance;
}

method(RCString*, fileNameFromSourceName, RInterpreter), const RCString *sourceFileName) {
    RCString *oldExtension = RS(".ray");
    RCString *fileExtension = $(sourceFileName, m(substringInRange, RCString)), makeRRange(sourceFileName->size - oldExtension->size, oldExtension->size));
    RCString *newExtension = RS("c");
    if($(fileExtension, m(compareWith, RCString)), oldExtension) != equals) {
        RError("RInterpreter. Filetype is not a \'.ray\'", object);
        return nil;
    }
    RCString *result = $(sourceFileName, m(copy, RCString)) );
    $(result, m(deleteInRange, RCString)), makeRRange(sourceFileName->size - oldExtension->size + 1, oldExtension->size - 1));
    $(result, m(concatenate, RCString)), newExtension);
    deallocator(oldExtension);
    deallocator(newExtension);
    return result;
}

method(void, initContainers, RInterpreter)) {
    // create source file string
    if(object->sourceFileString != nil) {
        deleteRCS(object->sourceFileString);
    }

    // create source tokens
    if(object->codeTokens != nil) {
        deleteRA(object->codeTokens);
    }
    object->codeTokens = makeRArray();
    makeStringArrayFrom(object->codeTokens);

    // create consts
    if(object->consts != nil) {
        deleteRA(object->consts);
    }
    object->consts = makeRArray();
    makeStringArrayFrom(object->consts);
}

method(void, parseTokens, RInterpreter)) {
    if(object->tokens != nil) {
        size_t iterator;
        // for all tokens
        forAll(iterator, object->tokens->count) {
            RCString *token = (RCString*) $(object->tokens, m(elementAtIndex, RArray)), iterator);
            $(token, p(RCString)) );
        }
    }
}

method(size_t, endOfProcessingString, RInterpreter)) {
    size_t    indexOfQuotes;
    size_t    indexOfSingleQuote;
    // get string to first \' or \"
    indexOfQuotes = indexOfFirstCharacterCString(object->sourceFileString->baseString,
            object->sourceFileString->size, '\"');
    indexOfSingleQuote = indexOfFirstCharacterCString(object->sourceFileString->baseString,
            object->sourceFileString->size, '\'');

    // they can be only equal when == sourceFileString->size, that means they are not found
    if(indexOfQuotes == indexOfSingleQuote) {
        return object->sourceFileString->size;
    } else {
        if (indexOfQuotes < indexOfSingleQuote) {
            return indexOfQuotes;
        } else {
            return indexOfSingleQuote;
        }
    }
}

method(RCString*, convertRayToC, RInterpreter), const char *sourceFileName) {
    RCString *sourceName = RS(sourceFileName);
    RCString *resultName = $(object, m(fileNameFromSourceName, RInterpreter)), sourceName);

    if(resultName != nil) {
        $(object, m(initContainers, RInterpreter)) );

        object->sourceFileString = RCStringFromFile(sourceFileName);

        rbool isBeginOfConst = no;
        RCString *basicSeparatorString = RS(" \n");

        while(object->sourceFileString->size != 0) {
            size_t    endOfProcessingSubstring;
            RCString *processingSubstring = nil;

            endOfProcessingSubstring = $(object, m(endOfProcessingString,  RInterpreter)));
            // get Processing Substring
            processingSubstring = $(object->sourceFileString, m(substringInRange, RCString)),
                    makeRRange(0, endOfProcessingSubstring));

            if(processingSubstring != nil) {
                // check if constant
                if(!isBeginOfConst) {
                    isBeginOfConst = yes;
                    // tokenize processing substring
                    if(object->tokens != nil) {
                        deleteRA(object->tokens)
                    }
                    object->tokens = $(processingSubstring,
                            m(substringsSeparatedBySymbols, RCString)), basicSeparatorString);

                    deleteRCS(processingSubstring);

                    // go next, if have some tokens
                    $(object, m(parseTokens, RInterpreter)) );
                } else {
                    // save to string constants
                    $(object->consts, m(addObject, RArray)), processingSubstring);
                    isBeginOfConst = no;
                }


                // trim source string
                $(object->sourceFileString, m(deleteInRange, RCString)), makeRRange(0, endOfProcessingSubstring + 1));
            }
        }
        deallocator(basicSeparatorString);
    }
    deallocator(sourceName);
    return resultName;
}