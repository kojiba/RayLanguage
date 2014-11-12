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
#include "../RayFoundation/RCString/RCString.h"

class(RIterpreter)
// processing data store
    RArray   *classes;
    RArray   *functions;
    RArray   *typeDefs;
    RArray   *globalVariables;

// string and char consts
    RArray   *consts;

// raw tokens, that be separated only by ' ' and '\n'
    RArray *tokens;

// c code generated tokens
    RArray *codeTokens;

// string added when file is read
    RCString *sourceFileString;
endOf(RInterpreter)

constructor (RInterpreter));
destructor  (RInterpreter);
singleton   (RInterpreter);

method(RCString*, fileNameFromSourceName, RInterpreter),    const RCString *sourceFileName);
method(void,      initContainers,         RInterpreter));
method(size_t,    endOfProcessingString,  RInterpreter));
method(void,      parseTokens,            RInterpreter));

// Main method
method(RCString*, convertRayToC,          RInterpreter),    const char *sourceFileName);

#define RISingleton         singletonCall(RInterpreter)
#define RayToC(fileName)    $(singletonCall(RInterpreter), m(convertRayToC, RInterpreter)), fileName)

#endif /*__R_INTERPRETER_H__*/