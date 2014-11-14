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
#include "../RIObject/RIObject.h"
#include "../../RayFoundation/RContainers/RStringDictionary.h"

typedef enum RayMethodType {
    MTVirtual = 0,
    MTInline  = 2,

    MTSetter  = 4,
    MTGetter  = 8,

    MTConstructor = 16,
    MTDestructor  = 32,

    MTOperator = 64,

    MTInner = 128,
    MTStatic = 256,
} RayMethodType;

char* toStringRayMethodType(RayMethodType object);

class(RayMethod)
    discipleOf(RIObject)

    RayMethodType  type;

    RCString      *returnType;
    RCString      *nativeName;

    RStringDictionary *arguments;

    rbool          isImplemented;
endOf(RayMethod)

constructor (RayMethod),    RayMethodType type, RCString *returnType); // copies return type
destructor  (RayMethod);
printer     (RayMethod);

// Setters
method(void, addArgument,  RayMethod), RCString *type, RCString *name);   // name - is key
method(void, setArguments, RayMethod), RStringDictionary *args);          // not copies arg dict

// Workers
method(RCString*, CPrefix, RayMethod)); // creates prefix like extern, inline
method(RCString*, CName,   RayMethod)); // serialize types to name
method(RCString*, CArgs,   RayMethod)); // serialize arguments

// Main method
method(RCString*, serializetoCFunc, RayMethod));

#endif /*__RAY_METHOD_H__*/