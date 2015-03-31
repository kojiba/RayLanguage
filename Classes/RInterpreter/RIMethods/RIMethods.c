/**
 * RIMethod.c
 * Method lexical struct of Ray.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 14.11.2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RIMethods.h"

RayMethodType RayMethodTypeFromString(char* string) {
    unsigned result = 0;
    const char *etalon = methodTypesConsts[result];
    while(result < MethodTypesCount) {
        if(strcmp(etalon, string) == 0) {
            break;
        }
        ++result;
        etalon = methodTypesConsts[result];
    }
    return (RayMethodType) (result);
}

const char* RayMethodTypeToString(RayMethodType type) {
    if(type < MethodTypesCount) {
        return methodTypesConsts[type];
    } else {
        return nil;
    }
}

#pragma mark Constructor - Destructor - Method

constructor(RayMethod), RayMethodType type, size_t returnType) {
    object = allocator(RayMethod);
    if(object != nil) {
        if((object->arguments = makeRArray()) != nil) {

            object->arguments->destructorDelegate = RClassNamePairDeleter;
            object->arguments->printerDelegate    = (PrinterDelegate) p(RClassNamePair);

            object->classId = registerClassOnce(toString(RayMethod));
            object->methodType = type;
            object->returnType = returnType;
            object->nativeName = nil;
            object->namespaceName = nil;
        }

    }
    return object;
}

destructor(RayMethod) {
    nilDeleter(object->namespaceName, RCString);
    nilDeleter(object->nativeName,    RCString);
       deleter(object->arguments,     RArray);
}

printer(RayMethod) {
    RPrintf("%s object - %p {\n", toString(RayMethod), object);
    RPrintf("\t Method type : %s\n", (RayMethodTypeToString(object->methodType)));
    RPrintf("} - %p\n\n", object);
}

#pragma mark Setters

method(void, setName, RayMethod), const char *name) {
    nilDeleter(object->nativeName, RCString);
    object->nativeName = RSC(name);
}

method(void, addArgument, RayMethod), size_t type, const char *name) {
    RClassNamePair *newOne = c(RClassNamePair)(nil);
    if(newOne != nil) {
        newOne->idForClassName = type;
        $(newOne->masterRCStringObject, m(setConstantString, RCString)), name);

        $(object->arguments, m(addObject, RArray)), newOne);
    }
}

method(void, setArguments, RayMethod), RArray *array) {
    nilDeleter(object->arguments, RArray);
    object->arguments = array;
}

method(void, setNamespaceName, RayMethod), const char *name) {
    nilDeleter(object->namespaceName, RCString);
    object->namespaceName = RSC(name);
}


#pragma mark Workers

static size_t lastIndex = 0;
static RCString *stored;
static RClassTable *storedDelegate = nil;

method(RCString*, CName, RayMethod)) {
    RCString *result = RSC("");
    const char * type = RayMethodTypeToString(object->methodType);
    if(type != nil) {
        // add type
        $(result, m(appendString, RCString)), type);
        // add postfix
        $(result, m(appendString, RCString)), method_type_prefix_postfix);

        // append native name, must be nil if operator or constructor
        if(!(object->methodType == MTConstructor
                || object->methodType == MTDestructor
                || object->methodType == MTOperator)) {
            if(object->nativeName == nil) {
                RError("RayMethod. No name for method", object);
            } else {
                $(result, m(concatenate, RCString)), object->nativeName);
            }
        }

        if(object->namespaceName == nil) {
            RError("RayMethod. No namespace name for method", object);
        } else {
            $(result, m(appendString, RCString)), namespace_name_prefix);
            $(result, m(concatenate, RCString)), object->namespaceName);
        }

        return result;
    } else {
        RError("RayMethod. CName, unknown method type.", object);
        return nil;
    }
}

rbool argumentsEnumerator(pointer argument, size_t iterator) {
    RClassNamePair *temp = argument;
    RCString *type = $(storedDelegate, m(getClassNameByIdentifier, RClassTable)), temp->idForClassName);

    if(type != nil) {
        $(stored, m(concatenate, RCString)), type);
        // add space
        $(stored, m(append, RCString)), ' ');
    } else {
        RError("RayMethod. Argument enumerator. Unknown argument type.", temp);
    }

    // add name
    $(stored, m(concatenate, RCString)), master(temp, RCString));

    if(iterator != lastIndex) {
        $(stored, m(appendString, RCString)), ", ");
    }
    return yes;
}

method(RCString*, CArgs, RayMethod), RClassTable *delegate) {
    RCString *result = RSC("");

    REnumerateDelegate enumerator;
    enumerator.virtualCheckObject = argumentsEnumerator;

    storedDelegate = delegate;
    stored         = result;
    lastIndex      = object->arguments->count - 1;

    $(object->arguments, m(enumerate, RArray)), &enumerator, yes);

    storedDelegate = nil;
    stored         = nil;
    lastIndex      = 0;

    return result;
}

#pragma mark Main methods

method(RCString *, serializetoCFunction, RayMethod), RClassTable *delegate, rbool isPointer) {
    RCString *result = RSC("");

    RCString *cname = $(object, m(CName, RayMethod)));
    RCString *cargs = $(object, m(CArgs, RayMethod)), delegate);

    RCString *returnTypeName = $(delegate, m(getClassNameByIdentifier, RClassTable)), object->returnType);

    if(returnTypeName != nil) {
        $(result, m(concatenate, RCString)), returnTypeName);
    }

    $(result, m(append, RCString)), ' ');

    if(isPointer) {
        $(result, m(appendString, RCString)), "(* ");
    }

    if(cname != nil) {
        $(result, m(concatenate, RCString)), cname );
        deleter(cname, RCString);
    }

    if(isPointer) {
        $(result, m(appendString, RCString)), ")");
    }
    $(result, m(append, RCString)), '(');

    if(cargs->size != 0) {
        $(result, m(concatenate, RCString)), cargs );
    }

    $(result, m(append, RCString)), ')');
    deleter(cargs, RCString);

    return result;
}

RayMethod* ParseMethodString(RCString *code, RClassTable *delegate) {
    RayMethod  *method = nil;
    RCString   *errorString = nil;

    // struct from scratch
    RayMethodType methodType = MTMethod;
    size_t        returnType = 1; // 1 - for void
    char         *nameString = nil;
    // workers
    rbool   isOperator = no,
            isHaveArgs = no;

    char   *codeIterator = code->baseString;
    size_t  codeSize     = code->size;
    size_t  tokenEnd     = indexOfFirstCharacterCString(codeIterator, codeSize,
                                                     method_type_separator);

    // found return type qualifier
    if (tokenEnd < code->size) {
        char *returnTypeString = substringInRange(codeIterator, makeRRange(0, tokenEnd));

        // one for separator property_type_separator symbol if its not there - :(
        codeSize     -= tokenEnd + 2;
        codeIterator += tokenEnd + 2;

        returnType = $(delegate, m(getIdentifierByClassName, RClassTable)), returnTypeString);
        if(returnType == 0) {
            errorString = stringWithFormat("Bad return type string \'%s\', not found in registered types.", returnTypeString);
            goto error;
        }
        deallocator(returnTypeString);
    }

    // next must be name or methodType
    while( !((isHaveArgs = (rbool) (*codeIterator == method_arguments_start_separator))
             || (isOperator = (rbool) (*codeIterator == method_operator_start_separator))
            )
            && codeSize <= code->size) {
        ++codeIterator;
        --codeSize;
    }

    if(isOperator) {
        char * operatorTokenString = substringInRange(code->baseString, makeRRange(tokenEnd + 1, code->size - codeSize - tokenEnd - 1));

        // next must be operator symbols like ++, --, etc
        if(strcmp(operatorTokenString, "operator") != 0) {
//            fixme
        } else {
            errorString = stringWithFormat("Bad operator token \'%s\'.", operatorTokenString);
            deallocator(operatorTokenString);
            goto error;
        }
        deallocator(operatorTokenString);

    // argument started
    } else if (isHaveArgs) {
        nameString = substringInRange(code->baseString, makeRRange(tokenEnd + 1, code->size - codeSize - tokenEnd - 1));

    // only name
    } else {
        nameString = substringInRange(code->baseString, makeRRange(tokenEnd + 1, code->size - tokenEnd - 1));
    }

    method = c(RayMethod)(nil, methodType, returnType);
    if(method != nil) {
        $(method, m(setName, RayMethod)), nameString);
    } else {
        errorString = RSC("Bad allocation of method struct.");
        goto error;
    }

    error:
    if(errorString != nil) {
        deallocator(method);
        RErrStr "Error. ParsePropertyString. %s\n", errorString->baseString);
        deleter(errorString, RCString);
        method = nil;
    }

    deallocator(nameString);

    return method;
}
