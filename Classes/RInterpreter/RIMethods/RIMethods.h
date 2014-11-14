/**
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

typedef enum RayMethodType {
    MTVirtual,
    MTInline,

    MTSetter,
    MTGetter,

    MTConstructor,
    MTDestructor,

    MTOperator
} RayMethodType;

class(RayMethod)
    RayMethodType  type;
    RCString      *returnType;
    RArray        *arguments;

    rbool          isImplemented;
endOf(RayMethod)

#endif /*__RAY_METHOD_H__*/