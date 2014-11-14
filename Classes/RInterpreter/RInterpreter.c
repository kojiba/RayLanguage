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
        object->rayTokens = nil;
        object->functions        = nil;
        object->globalVariables  = nil;
        object->typeDefs         = nil;
        object->stringConsts     = nil;
    }
    return object;
}

destructor(RInterpreter) {
    if(object->sourceFileString != nil) {
        deleteRCS(object->sourceFileString);
    }
    if(object->rayTokens != nil) {
        deleteRA(object->rayTokens);
    }
    if(object->codeTokens != nil) {
        deleteRA(object->codeTokens);
    }
    if(object->stringConsts != nil) {
        deleteRA(object->stringConsts);
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

    // create source rayTokens
    if(object->codeTokens != nil) {
        deleteRA(object->codeTokens);
    }
    object->codeTokens = makeRArray();
    makeStringArrayFrom(object->codeTokens);

    // create stringConsts
    if(object->stringConsts != nil) {
        deleteRA(object->stringConsts);
    }
    object->stringConsts = makeRArray();
    makeStringArrayFrom(object->stringConsts);
}

method(void, parseTokens, RInterpreter)) {
    if(object->rayTokens != nil) {
        size_t iterator;
        // for all rayTokens
        forAll(iterator, object->rayTokens->count) {
            RCString *token = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), iterator);
//            switch(token->baseString[0]) {
//                case '#': {
//                    if(RMemCmp(token->baseString, "include", token->size - 1) == 0) {
//                        // add include
//                        $(object->codeTokens, m(addObject, RArray)), $(token, m(copy, RCString))) );
//                    } else if (RMemCmp(token->baseString, "define", token->size - 1) == 0) {
//                        // add define
//
//                    } else if (RMemCmp(token->baseString, "pragma", token->size - 1) == 0) {
//                        // add pragma
//                    }
//                    return;
//                }
//                case '<' : {
//                    return;
//                }
//                case 'c' : { // start of class
//                    return;
//                }
//            }
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
        while(object->sourceFileString != nil) {

            size_t    endOfProcessingSubstring;
            RCString *processingSubstring = nil;

            // get Processing Substring
            endOfProcessingSubstring = $(object, m(endOfProcessingString,  RInterpreter)));
            processingSubstring = $(object->sourceFileString, m(substringInRange, RCString)),
                    makeRRange(0, endOfProcessingSubstring));

            if(processingSubstring != nil) {
                // check if constant
                if(!isBeginOfConst) {
                    isBeginOfConst = yes;
                    // tokenize processing substring
                    if(object->rayTokens != nil) {
                        deleteRA(object->rayTokens)
                    }
                    object->rayTokens = $(processingSubstring,
                            m(substringsSeparatedBySymbols, RCString)), basicSeparatorString);

                    deleteRCS(processingSubstring);

                    // go next, if have some rayTokens
                    $(object, m(parseTokens, RInterpreter)) );
                } else {
                    // save to string constants
                    $(object->stringConsts, m(addObject, RArray)), processingSubstring);
                    isBeginOfConst = no;
                }

                if(object->sourceFileString->size != endOfProcessingSubstring) {
                    // trim source string
                    $(object->sourceFileString, m(trimHead, RCString)),  endOfProcessingSubstring + 1);
                } else {
                    // delete at all
                    deleteRCS(object->sourceFileString);
                    object->sourceFileString = nil;
                }
            }
        }

        deallocator(basicSeparatorString);
    }
    deallocator(sourceName);
    return resultName;
}