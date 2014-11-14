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

#include <Foundation/Foundation.h>
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
    }
    return nil;
}

constructor(RayMethod), RayMethodType type, RCString *returnType) {
    object = allocator(RayMethod);
    if(object != nil) {
        object->classId    = registerClassOnce(toString(RayMethod));
        object->type       = type;
        object->returnType = $(returnType, m(copy, RCString)));
        object->arguments = makeRArray();

        linkMethod(master(object, RIObject), printer, p(RayMethod));
    }
    return object;
}

destructor(RayMethod) {

}

printer(RayMethod) {
    RPrintf("%s object - %p {\n", toString(RayMethod), object);
    RPrintf("\t Method type : %s", $(object->type, toStringRayMethodType)));
    RPrintf("} - %p\n", object);
}