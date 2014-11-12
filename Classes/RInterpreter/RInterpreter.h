#include "../RayFoundation/RSyntax.h"/**
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

class(RIterpreter)
    RArray *array;
endOf(RInterpreter)

constructor (RInterpreter));
destructor  (RInterpreter);
singleton   (RInterpreter);


#endif /*__R_INTERPRETER_H__*/