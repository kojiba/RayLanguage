/**
 * RString_Numbers.h
 * Number additions to RStrings.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RString.h"

// Basics
static inline rbool isDecimalDigit (char character);
static inline byte  decimalDigitFromChar(char character); // [0; 9] or 10 if error
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

constMethod(const RString *, toRString, RNumberSystemBase));

constMethod(size_t,  toSystemUInt, RString), rbool *isConvertedNormal);
constMethod(ssize_t, toSystemInt,  RString), rbool *isConvertedNormal);

//-----------------------------------------------------------

// RString Additions
constMethod(RNumberSystemBase, isNumber,       RString));

#define toUnsignedRString(object) RStringToUnsigned(object->baseString, nil, 0)
#define toIntRString(object)      RStringToInt(object->baseString)
#define toDoubleRString(object)   RStringToDouble(object->baseString)

#endif /*__R_C_STRING_NUMBERS_H__*/