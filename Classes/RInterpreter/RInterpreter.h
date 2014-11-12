/**
 * RayBase.h
 * A ray of light in the realm of darkness.
 * Defines basic c-functions, to may re-format it if needed.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
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
    RArray   *types;
    RArray   *globalVariables;

// string added when file is read
    RCString *sourceFileString;
endOf(RInterpreter)

constructor (RInterpreter));
destructor  (RInterpreter);
singleton   (RInterpreter);

method(RCString*, fileNameFromSourceName, RInterpreter), const RCString *sourceFileName);

method(RCString*, convertRayToC, RInterpreter), const char *sourceFileName);

#define RISingleton singletonCall(RInterpreter)

#endif /*__R_INTERPRETER_H__*/