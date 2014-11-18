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
#include "../../RayFoundation/RContainers/RStringDictionary.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

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

    MTExtern = 512,
} RayMethodType;

typedef enum RayOperatorType {
    OTPrefix = 0,
    OTPostfix = 1,

// one operand
    OTPlusPlus,
    OTMinusMinus,

// two operands
    OTMinus,
    OTPlus,
    OTMultiplication,
    OTDivision,
    OTModulo,


} RayOperatorType;

char* toStringRayMethodType(RayMethodType object);

class(RayMethod)

    RayMethodType    type;
    RayOperatorType  operatorType;

    RCString        *returnType;
    RCString        *nativeName;

    RStringDictionary *arguments;

    rbool          isImplemented;
endOf(RayMethod)

constructor (RayMethod),    RayMethodType type, RCString *returnType); // strings mustn't be copies
destructor  (RayMethod);
printer     (RayMethod);

// Setters
method(void, addArgument,  RayMethod), size_t type, RCString *name);      // name - is key, type is value, type - from typeTable of interpreter
method(void, setArguments, RayMethod), RStringDictionary *args);          // not copies arg dict

// Workers
method(RCString*, CPrefix, RayMethod)); // creates prefix like extern, inline
method(RCString*, CName,   RayMethod)); // serialize types to name
method(RCString*, CArgs,   RayMethod),    RClassTable *delegate); // serialize arguments from size_t to strings, need classTable delegate with registered types

// Main methods
method(RCString*, serializetoCFunction, RayMethod),    RClassTable *delegate);
method(RCString*, serializetoCPointer,  RayMethod),    RClassTable *delegate);

#endif /*__RAY_METHOD_H__*/