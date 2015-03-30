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
#include "../RInterpereterConsts.h"

RayMethodType RayMethodTypeFromString(char* string);
const char*   RayMethodTypeToString(RayMethodType type);

class(RayMethod)

    RayMethodType     methodType;
    RayOperatorType   operatorType;

    size_t         returnType;
    RCString      *nativeName;
    RCString      *namespaceName;

    RArray        *arguments;
    rbool          isImplemented;
endOf(RayMethod)

constructor (RayMethod),    RayMethodType type, size_t returnType);
destructor  (RayMethod);
printer     (RayMethod);

// Setters
method(void, setName,          RayMethod), const char *name);
method(void, setNamespaceName, RayMethod), const char *name);
method(void, addArgument,      RayMethod), size_t type, const char *name); // name - is key, type is value, type - start typeTable of interpreter
method(void, setArguments,     RayMethod), RArray *array);                // not copies arg dict

// Main methods
method(RCString *, serializetoCFunction, RayMethod),    RClassTable *delegate, rbool isPointer);

#endif /*__RAY_METHOD_H__*/