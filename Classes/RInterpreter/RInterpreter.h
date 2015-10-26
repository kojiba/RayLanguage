/**
 * RInterpreter.h
 * A ray of light in the realm of darkness.
 * Interpreter of Ray language to pure C.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12.11.2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/


#ifndef __R_INTERPRETER_H__
#define __R_INTERPRETER_H__

#include "../RayFoundation/RSyntax.h"
#include "../RayFoundation/RContainers/RArray.h"
#include "../RayFoundation/RString/RString.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

class(RIterpreter)
// table of types
    RClassTable *typesTable;

// processing data store
    RArray   *classes;    // array of RIClass objects
    RArray   *functions;
    RArray   *typeDefs;
    RArray   *globalVariables;
    RArray   *includes;

// for checking places
    rbool isInClass;
    rbool isInProperties;
    rbool isInMethods;
    rbool isInStatics;

// string and char consts
    RArray *stringConsts;

// raw rayTokens, that be separated only by ' ' and '\n'
    RArray *rayTokens;

// c code generated rayTokens
    RArray *codeTokens;

// string added when file is read
    RString *sourceFileString;

endOf(RInterpreter)

constructor (RInterpreter));
destructor  (RInterpreter);
singleton   (RInterpreter);

// Main method
method(RString*, convertRayToC,          RInterpreter),    const char *sourceFileName);

#define RISingleton         singletonCall(RInterpreter)
#define RayToC(fileName)    $(singletonCall(RInterpreter), m(convertRayToC, RInterpreter)), fileName)

#endif /*__R_INTERPRETER_H__*/