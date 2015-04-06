/**
 * RCString_Numbers.h
 * Number additions to RCStrings.
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

#ifndef __R_C_STRING_NUMBERS_H__
#define __R_C_STRING_NUMBERS_H__

#include <RCString.h>

// Basics
static inline rbool isDecimalDigit (char character);
static inline rbool isOctalDigit   (char character);
static inline rbool isHexDigit     (char character);
static inline rbool isBinaryDigit  (char character);

typedef enum RNumberSystemBase { //--------------------------
    RNotNumber = 0,
    RBinary,
    ROctal,
    RDecimal,
    RHex,
} RNumberSystemBase; //--------------------------------------

constMethod(RCString *,        toRCString, RNumberSystemBase));

//-----------------------------------------------------------

// RCString Additions
constMethod(RNumberSystemBase, isNumber,       RCString));

#define toUnsignedRCString(object) RStringToUnsigned(object->baseString, nil, 0)
#define toIntRCString(object)      RStringToInt(object->baseString)
#define toDoubleRCString(object)   RStringToDouble(object->baseString)

#endif /*__R_C_STRING_NUMBERS_H__*/