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
    }
    return nil;
}

#pragma mark Constructor - Destructor - Method

constructor(RayMethod), RayMethodType type, RCString *returnType) {
    object = allocator(RayMethod);
    if(object != nil) {
        master(object, RIObject) = allocator(RIObject);
        if(master(object, RIObject) != nil) {
            object->classId    = registerClassOnce(toString(RayMethod));
            object->type       = type;
            object->returnType = returnType;
            object->arguments  = $(nil, c(RStringDictionary)));
            // link some
            linkMethod(master(object, RIObject), printer, p(RayMethod));
        }
    }
    return object;
}

destructor(RayMethod) {
    deleteRIObject(master(object, RIObject));
}

printer(RayMethod) {
    RPrintf("%s object - %p {\n", toString(RayMethod), object);
    RPrintf("\t Method type : %s", $(object->type, toStringRayMethodType)));
    RPrintf("} - %p\n", object);
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
        RCString *inlineStr = RS("inline ");
        $(result, m(concatenate, RCString)), inlineStr);
        deallocator(inlineStr);
    }
    return result;
}

method(RCString*, CName, RayMethod)) {
    RCString *result = RSC("");

    // can be inner always
    if (object->type & MTInner) {
        RCString *inner = RS("inner_");
        $(result, m(concatenate, RCString)), inner);
        deallocator(inner);
    }

    // if static
    if(object->type & MTStatic) {
        RCString *staticStr = RS("static_");
        $(result, m(concatenate, RCString)), staticStr);
        deallocator(staticStr);

    // if not static
    } else {
        // constructor
        if (object->type & MTConstructor) {
             RCString *temp = RS("constructor_");
             $(result, m(concatenate, RCString)), temp);
             deallocator(temp);

        // destructor
        } else if (object->type & MTDestructor) {
             RCString *temp = RS("destructor_");
             $(result, m(concatenate, RCString)), temp);
             deallocator(temp);

        // operator
        } else if (object->type & MTOperator) {
             RCString *temp = RS("operator_");
             $(result, m(concatenate, RCString)), temp);
             deallocator(temp);

             // if prefix or postfix operator
             if(object->operatorType & OTPlusPlus
                     || object->operatorType & OTMinusMinus) {

                 if(object->operatorType & OTPostfix) {
                     RCString *temp2 = RS("postfix_");
                     $(result, m(concatenate, RCString)), temp2);
                     deallocator(temp2);
                 } else if (object->operatorType & OTPrefix) {
                     RCString *temp2 = RS("prefix_");
                     $(result, m(concatenate, RCString)), temp2);
                     deallocator(temp2);
                 }

                 if(object->operatorType & OTPlusPlus) {
                     RCString *temp2 = RS("plusPlus_");
                     $(result, m(concatenate, RCString)), temp2);
                     deallocator(temp2);
                 } else {
                     RCString *temp2 = RS("minusMinus_");
                     $(result, m(concatenate, RCString)), temp2);
                     deallocator(temp2);
                 }

             // if other binary operator
             } else {
                 if(object->operatorType & OTMinus) {
                     RCString *temp2 = RS("minus_");
                     $(result, m(concatenate, RCString)), temp2 );
                     deallocator(temp2);
                 } else if(object->operatorType & OTPlus) {
                     RCString *temp2 = RS("plus_");
                     $(result, m(concatenate, RCString)), temp2 );
                     deallocator(temp2);
                 } else if(object->operatorType & OTMultiplication) {
                     RCString *temp2 = RS("multiplication_");
                     $(result, m(concatenate, RCString)), temp2 );
                     deallocator(temp2);
                 } else if(object->operatorType & OTDivision) {
                     RCString *temp2 = RS("division_");
                     $(result, m(concatenate, RCString)), temp2 );
                     deallocator(temp2);
                 } else if(object->operatorType & OTModulo) {
                     RCString *temp2 = RS("modulo_");
                     $(result, m(concatenate, RCString)), temp2 );
                     deallocator(temp2);
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
    RCString *space = RS(" ");
    RCString *comma = RS(", ");
    forAll(iterator, object->arguments->masterRDictionaryObject->keys->count) {
        size_t type = (size_t) object->arguments->masterRDictionaryObject->values->array[iterator];
        RCString * temp = $(delegate, m(getClassNameByIdentifier, RClassTable)), type);

        // add type
        $(result, m(concatenate, RCString)), temp );

        // add space
        $(result, m(concatenate, RCString)), space );

        // add name
        $(result, m(concatenate, RCString)), ((RCString*)object->arguments->masterRDictionaryObject->keys->array[iterator]) );
        // if not last
        if(iterator != object->arguments->masterRDictionaryObject->keys->count - 1) {
            // add ','
            $(result, m(concatenate, RCString)), comma );
        }
    }
    deallocator(space);
    deallocator(comma);
    return result;
}

#pragma mark Main method

method(RCString*, serializetoCFunc, RayMethod), RClassTable *delegate) {
    RCString *result = RSC("");
    RCString *temp = RS(" ");
    RCString *leftQuote = RS(")");
    RCString *rightQuote = RS("(");

    $(result, m(concatenate, RCString)), $(object, m(CPrefix, RayMethod))) );
    $(result, m(concatenate, RCString)), object->returnType);
    $(result, m(concatenate, RCString)), temp);
    $(result, m(concatenate, RCString)), $(object, m(CName, RayMethod))) );
    $(result, m(concatenate, RCString)), rightQuote);
    $(result, m(concatenate, RCString)), $(object, m(CArgs, RayMethod)), delegate));
    $(result, m(concatenate, RCString)), leftQuote);


    // cleanup
    deallocator(temp);
    deallocator(leftQuote);
    deallocator(rightQuote);
    return result;
}