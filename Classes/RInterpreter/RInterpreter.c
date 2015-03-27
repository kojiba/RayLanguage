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
#include "RIProperties/RIProperties.h"
#include "RIClass/RIClass.h"

constructor(RInterpreter)) {
    object = allocator(RInterpreter);
    if(object != nil) {
        object->typesTable = makeRCTable();

        if(object->typesTable != nil) {
            object->classId = registerClassOnce(toString(RInterpreter));
            // all to nil
            object->codeTokens       = nil;
            object->sourceFileString = nil;
            object->rayTokens        = nil;
            object->functions        = nil;
            object->globalVariables  = nil;
            object->typeDefs         = nil;
            object->stringConsts     = nil;
            object->classes          = nil;

            // register some basic c types
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(void)); // must be 0
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(int));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(char));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(float));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(double));

            // type of types
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(short));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(long));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(signed));
            $(object->typesTable, m(registerClassWithName, RClassTable)), toString(unsigned));
        }
    }

    return object;
}

destructor(RInterpreter) {
    nilDeleter(object->sourceFileString, RCString)
    nilDeleter(object->rayTokens, RArray)
    nilDeleter(object->codeTokens, RArray)
    nilDeleter(object->stringConsts, RArray)
    nilDeleter(object->classes, RArray)
    deleter(object->typesTable, RClassTable);
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
    if($(fileExtension, m(compareWith, RCString)), oldExtension) != equals) {
        RError("RInterpreter. Filetype is not a \'.ray\'", object);
        deallocator(oldExtension);
        deleter(fileExtension, RCString);
        return nil;
    }
    RCString *result = $(sourceFileName, m(copy, RCString)) );
    $(result, m(deleteInRange, RCString)), makeRRange(sourceFileName->size - oldExtension->size + 1, oldExtension->size - 1));

    // append .c extension
    $(result, m(append, RCString)), 'c');

    // cleanup
    deallocator(oldExtension);
    deleter(fileExtension, RCString);
    return result;
}

method(void, initContainers, RInterpreter)) {
    // create source file string
    nilDeleter(object->sourceFileString, RCString)

    // create source rayTokens
    nilDeleter(object->codeTokens, RArray)
    object->codeTokens = makeRArray();
    stringDelegates(object->codeTokens);

    // create stringConsts
    nilDeleter(object->stringConsts, RArray)
    object->stringConsts = makeRArray();
    stringDelegates(object->stringConsts);
}

method(void, parseClass, RInterpreter), size_t iterator) {
    size_t inner;
    RayClass *rayClass = c(RayClass)(nil);

    // add name
    rayClass->name = copyRCString($(object->rayTokens, m(elementAtIndex, RArray)), iterator - 1));

    for(inner = iterator; inner <  object->rayTokens->count; ++inner) {
        RCString *token = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), ++iterator);
        switch(token->baseString[0]) {
            case 'p' : {
                // properties begin
                if(RMemCmp(token->baseString + 1, "roperties:", token->size - 1) == 0) {
                    RayProperty *property = $(nil, c(RayProperty)));

                    addObjectToRA(rayClass->properties, property);
                }
                break;
            }

            case 'r' : {

            }

            // start of classBody
            case '{' : {
                break;
            }

            // end of classBody
            case '}' : {
                break;
            }
        }
    }
}

method(void, parseTokens, RInterpreter)) {
    if(object->rayTokens != nil) {
        size_t iterator;
        // for all rayTokens
        forAll(iterator, object->rayTokens->count) {
            RCString *token = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), iterator);
            switch(token->baseString[0]) {
                case '#': {
                    if(RMemCmp(token->baseString + 1, "include", token->size - 1) == 0) {
                        // add include
                        $(object->codeTokens, m(addObject, RArray)), $(token, m(copy, RCString))) );

                        // add include name
                        RCString *name = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), ++iterator);
                        $(object->codeTokens, m(addObject, RArray)), $(name, m(copy, RCString))) );

                    } else if (RMemCmp(token->baseString + 1, "define", token->size - 1) == 0) {
                        // add define

                    } else if (RMemCmp(token->baseString + 1, "pragma", token->size - 1) == 0) {
                        // add pragma
                    }
                    break;
                }

                // start of class
                case 'c' : {
                    if(RMemCmp(token->baseString + 1, "lass", token->size - 1) == 0) {
                        // register new class
                        RCString *name = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), ++iterator);
                        size_t identifier = $(object->typesTable, m(getIdentifierByClassName, RClassTable)), name->baseString);

                        if(identifier == 0) {
                            $(object->typesTable, m(registerClassWithName, RClassTable)), copyOfCString(name->baseString));
                            $(object, m(parseClass, RInterpreter)), iterator);
                        } else {
                            RFPrintf(stderr, "%p ERROR. RInterpreter. Class with name \"%s\" already exists.\n", object, name->baseString);
                            return;
                        }
                        break;
                    }
                }

                // start of typedef
                case 't' : {
                    if(RMemCmp(token->baseString + 1, "ypedef", token->size - 1) == 0) {
                        // register new type
                        RCString *name = (RCString*) $(object->rayTokens, m(elementAtIndex, RArray)), ++iterator);
                        size_t identifier = $(object->typesTable, m(getIdentifierByClassName, RClassTable)), name->baseString);

                        if(identifier == 0) {
                            $(object->typesTable, m(registerClassWithName, RClassTable)), copyOfCString(name->baseString));
                        } else {
                            RFPrintf(stderr, "%p ERROR. RInterpreter. Type with name \"%s\" already exists.\n", object, name->baseString);
                        }
                        break;
                    }
                }

            }
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

#pragma mark Main method

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
                        deleter(object->rayTokens, RArray);
                    }
                    object->rayTokens = $(processingSubstring,
                            m(substringsSeparatedBySymbols, RCString)), basicSeparatorString);

                    deleter(processingSubstring, RCString);

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
                    deleter(object->sourceFileString, RCString);
                    object->sourceFileString = nil;
                }
            }
        }

        deallocator(basicSeparatorString);
    }
    deallocator(sourceName);
    return resultName;
}