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

#include "RCString.h"

// Basics
static inline RBool isDecimalDigit (char character);
static inline RBool isOctalDigit   (char character);
static inline RBool isHexDigit     (char character);

typedef enum RNumberSystemBase {
    RNotNumber,
    RBinary,
    ROctal,
    RDecimal,
    RHex,
} RNumberSystemBase;

method(RNumberSystemBase, isNumber,       RCString));

method(unsigned,          toUnsgined,     RCString));
method(long,              toLong,         RCString));
method(int,               toInt,          RCString));
method(float,             toFloat,        RCString));
method(double,            toDouble,       RCString));





#endif __R_C_STRING_NUMBERS_H__