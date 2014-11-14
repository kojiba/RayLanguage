#include "../../RayFoundation/RSyntax.h"/**
 * RIMethod.h
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

#ifndef __RAY_METHOD_H__
#define __RAY_METHOD_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RCString/RCString.h"
#include "../RIObject/RIObject.h"

typedef enum RayMethodType {
    MTVirtual,
    MTInline,

    MTSetter,
    MTGetter,

    MTConstructor,
    MTDestructor,

    MTOperator,

    MTInner,
} RayMethodType;

char* toStringRayMethodType(RayMethodType object);

class(RayMethod)
    discipleOf(RIObject)

    RayMethodType  type;
    RCString      *returnType;
    RArray        *arguments;

    rbool          isImplemented;
endOf(RayMethod)

constructor (RayMethod),    RayMethodType type, RCString *returnType); // copies return type
destructor  (RayMethod);
printer     (RayMethod);

#endif /*__RAY_METHOD_H__*/