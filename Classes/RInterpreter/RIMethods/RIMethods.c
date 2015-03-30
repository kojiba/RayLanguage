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

//method(RCString*, CPrefix, RayMethod)) {
//    RCString *result = RSC("");
//    return result;
//}

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
    RCString *type = $(storedDelegate, m(getClassNameByIdentifier, RClassTable)), temp->classId);

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

//    RCString *prefix = $(object, m(CPrefix, RayMethod)));
    RCString *cname  = $(object, m(CName, RayMethod)));
    RCString *cargs  = $(object, m(CArgs, RayMethod)), delegate);

//    $(result, m(concatenate, RCString)), prefix );
    RCString *name = $(delegate, m(getClassNameByIdentifier, RClassTable)), object->returnType);

    if(name != nil) {
        $(result, m(concatenate, RCString)), name);
    }

    $(result, m(append, RCString)), ' ');

    if(isPointer) {
        $(result, m(appendString, RCString)), "(* ");
    }
    $(result, m(concatenate, RCString)), cname );

    if(isPointer) {
        $(result, m(appendString, RCString)), ")");
    }
    $(result, m(append, RCString)), '(');
    $(result, m(concatenate, RCString)), cargs );
    $(result, m(append, RCString)), ')');

    // cleanup
//    deleter(prefix, RCString);
    deleter(cname, RCString);
    deleter(cargs, RCString);
    return result;
}
