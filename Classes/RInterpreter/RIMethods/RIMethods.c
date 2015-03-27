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

char* toStringRayMethodType(RayMethodType object) {
    switch (object) {
        case MTVirtual: {
            return toString(MTVirtual);
        }
        case MTInline: {
            return toString(MTInline);
        }
        case MTGetter: {
            return toString(MTGetter);
        }
        case MTDestructor: {
            return toString(MTDestructor);
        }
        case MTInner: {
            return toString(MTInner);
        }
        case MTConstructor: {
            return toString(MTConstructor);
        }
        case MTSetter: {
            return toString(MTSetter);
        }
        case MTOperator: {
            return toString(MTOperator);
        }
        case MTStatic: {
            return toString(MTStatic);
        }
        case MTExtern: {
            return toString(MTExtern);
        }
    }
    return nil;
}

#pragma mark Constructor - Destructor - Method

constructor(RayMethod), RayMethodType type, RCString *returnType) {
    object = allocator(RayMethod);
    if(object != nil) {
        object->classId    = registerClassOnce(toString(RayMethod));
        object->type       = type;
        object->returnType = returnType;
        object->arguments  = $(nil, c(RStringDictionary)));
    }
    return object;
}

destructor(RayMethod) {
    deleter(object->nativeName, RCString);
    deleter(object->returnType, RCString);
    deleter(object->arguments,  RStringDictionary);
}

printer(RayMethod) {
    RPrintf("%s object - %p {\n", toString(RayMethod), object);
    RPrintf("\t Method type : %s\n", $(object->type, toStringRayMethodType)));
    RPrintf("} - %p\n\n", object);
}

#pragma mark Setters

method(void, addArgument,  RayMethod), size_t type, RCString *name) {
    $(object->arguments, m(setObjectForKey, RStringDictionary)), (pointer) type, name);
}

method(void, setArguments, RayMethod), RStringDictionary *args) {
    if(object->arguments != nil) {
        $(object->arguments, d(RStringDictionary)) );
        deallocator(object->arguments);
    }
    object->arguments = args;
}

#pragma mark Workers

method(RCString*, CPrefix, RayMethod)) {
    RCString *result = RSC("");
    // can be inline always
    if(object->type & MTInline) {
        $(result, m(appendString, RCString)), "inline ");
    }
    return result;
}

method(RCString*, CName, RayMethod)) {
    RCString *result = RSC("");

    // can be inner always
    if (object->type & MTInner) {
        $(result, m(appendString, RCString)), "inner_");
    }

    // if static
    if(object->type & MTStatic) {
        $(result, m(appendString, RCString)), "static_");

    // if not static
    } else {
        // constructor
        if (object->type & MTConstructor) {
             $(result, m(appendString, RCString)), "constructor_");

        // destructor
        } else if (object->type & MTDestructor) {
             $(result, m(appendString, RCString)), "destructor_");

        // operator
        } else if (object->type & MTOperator) {
             $(result, m(appendString, RCString)), "operator_");

             // if prefix or postfix operator
             if(object->operatorType & OTPlusPlus
                     || object->operatorType & OTMinusMinus) {

                 if(object->operatorType & OTPostfix) {
                     $(result, m(appendString, RCString)), "postfix_");
                 } else if (object->operatorType & OTPrefix) {
                     $(result, m(appendString, RCString)), "prefix_");
                 }

                 if(object->operatorType & OTPlusPlus) {
                     $(result, m(appendString, RCString)), "plusPlus_");
                 } else {
                     $(result, m(appendString, RCString)), "minusMinus_");
                 }

             // if other binary operator
             } else {
                 if(object->operatorType & OTMinus) {
                     $(result, m(appendString, RCString)), "minus_");
                 } else if(object->operatorType & OTPlus) {
                     $(result, m(appendString, RCString)), "plus_");
                 } else if(object->operatorType & OTMultiplication) {
                     $(result, m(appendString, RCString)), "multiplication_");
                 } else if(object->operatorType & OTDivision) {
                     $(result, m(appendString, RCString)), "division_");
                 } else if(object->operatorType & OTModulo) {
                     $(result, m(appendString, RCString)), "modulo_");
                 }
             }
        }
    }

    // append native name, must be nil if operator
    $(result, m(concatenate, RCString)), object->nativeName);
    return result;
}

method(RCString*, CArgs, RayMethod), RClassTable *delegate) {
    size_t iterator;
    RCString *result = RSC("");
    forAll(iterator, object->arguments->masterRDictionaryObject->keys->count) {
        size_t type = (size_t) $(master(object->arguments, RDictionary)->values, m(elementAtIndex, RArray)), iterator);
        RCString *temp = $(delegate, m(getClassNameByIdentifier, RClassTable)), type);

        // add type
        $(result, m(concatenate, RCString)), temp );

        // add space
        $(result, m(append, RCString)), ' ');

        // add name
        $(result, m(concatenate, RCString)), ((RCString*)$(master(object->arguments, RDictionary)->keys, m(elementAtIndex, RArray)), iterator)));
        // if not last
        if(iterator != master(object->arguments, RDictionary)->keys->count - 1) {
            // add ','
            $(result, m(append, RCString)), ',' );
            // add space
            $(result, m(append, RCString)), ' ');
        }
    }
    return result;
}

#pragma mark Main methods

method(RCString*, serializetoCFunction, RayMethod), RClassTable *delegate) {
    RCString *result = RSC("");

    RCString *prefix = $(object, m(CPrefix, RayMethod)));
    RCString *cname = $(object, m(CName, RayMethod)));
    RCString *cargs = $(object, m(CArgs, RayMethod)), delegate);

    $(result, m(concatenate, RCString)), prefix );
    $(result, m(concatenate, RCString)), object->returnType);
    $(result, m(append, RCString)), ' ');
    $(result, m(concatenate, RCString)), cname );
    $(result, m(append, RCString)), '(');
    $(result, m(concatenate, RCString)), cargs );
    $(result, m(append, RCString)), ')');

    // cleanup
    deleter(prefix, RCString);
    deleter(cname, RCString);
    deleter(cargs, RCString);
    return result;
}

method(RCString*, serializetoCPointer,  RayMethod), RClassTable *delegate) {
    RCString *result = RSC("");

    RCString *cname = $(object, m(CName, RayMethod)));
    RCString *cargs = $(object, m(CArgs, RayMethod)), delegate);

    $(result, m(concatenate, RCString)), object->returnType);
    $(result, m(append, RCString)), ' ');
    $(result, m(append, RCString)), '(');
    $(result, m(append, RCString)), '*');
    $(result, m(concatenate, RCString)), cname );
    $(result, m(append, RCString)), ')');
    $(result, m(append, RCString)), '(');
    $(result, m(concatenate, RCString)), cargs);
    $(result, m(append, RCString)), ')');

    // cleanup
    deleter(cname, RCString);
    deleter(cargs, RCString);
    return result;
}