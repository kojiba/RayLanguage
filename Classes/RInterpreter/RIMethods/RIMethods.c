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
#include "../../RayFoundation/RClassTable/RClassTable.h"

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
    $(object->arguments, m(setObjectForKey, RStringDictionary)), type, name);
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

    // can be inline always
//    if(object->type & MTInline) {
//        RCString *inlineStr = RS("inline_");
//        $(result, m(concatenate, RCString)), inlineStr);
//        deallocator(inlineStr);
//    }

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
         if (object->type & MTConstructor) {
            result = RSC("constructor_");
        } else if (object->type & MTDestructor) {
            result = RSC("destructor_");
        } else if (object->type & MTOperator) {
            result = RSC("operator_");
        }
    }

    // append native name
    $(result, m(concatenate, RCString)), object->nativeName);
    return result;
}

method(RCString*, CArgs, RayMethod)) {
    RCString *result = nil;
    return result;
}

#pragma mark Main method

method(RCString*, serializetoCFunc, RayMethod)) {
    RCString  *result = nil;

    return result;
}