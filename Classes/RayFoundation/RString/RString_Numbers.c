/**
 * RString_Numbers.c
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

#include "RString_Numbers.h"
#include "RString_Consts.h"

#pragma mark Basics

static inline rbool isDecimalDigit(char character) {
    if(character < '0' || character > '9') {
        return no;
    } else {
        return yes;
    }
}

static inline byte decimalDigitFromChar(char character) {
    if(isDecimalDigit(character)) {
        return (byte) character - (byte)'0';
    }
    return 10;
}

static inline rbool isOctalDigit(char character) {
    if(character < '0' || character > '7') {
        return no;
    } else {
        return yes;
    }
}

static inline rbool isHexDigit(char character) {
    if(isDecimalDigit(character)
            || (character > 64 && character < 71)
            || (character > 96 && character < 103)) {
        return yes;
    } else {
        return no;
    }
}

static inline rbool isBinaryDigit(char character) {
    if(character == '0' || character == '1') {
        return yes;
    } else {
        return no;
    }
}

#pragma mark RNumberSystemBase

constMethod(const RString *, toRString, RNumberSystemBase)) {
    switch (*object) {
        case RNotNumber : {
            return RS(toString(RNotNumber));
        }

        case RBinary : {
            return RS(toString(RBinary));
        }

        case ROctal : {
            return RS(toString(ROctal));
        }

        case RHex : {
            return RS(toString(RHex));
        }

        case RDecimal : {
            return RS(toString(RDecimal));
        }
    }
    return nil;
}

#pragma mark RString Additions

constMethod(RNumberSystemBase, isNumber, RString)) {
    size_t iterator;
    // not too large
    if(object->size < 200) {
        // it may be hex, or octal, or binary
        if(object->data[0] == '0') {

            // if hex
            if(object->data[1] == 'x'
                    || object->data[1] == 'X') {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isHexDigit(object->data[iterator]) == no) {
                        return RNotNumber;
                    }
                }
                return RHex;

            // binary
            } else if (object->data[0] == 'b'){
                // check symbols
                for(iterator = 1; iterator < object->size; ++iterator) {
                    if(isBinaryDigit(object->data[iterator]) == no) {
                        return RNotNumber;
                    }
                }

            // octal
            } else {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isOctalDigit(object->data[iterator]) == no) {
                        return RNotNumber;
                    }
                }
                return ROctal;
            }

        // decimal
        } else {
            forAll(iterator, object->size) {
                if (isDecimalDigit(object->data[iterator]) == no) {
                    return RNotNumber;
                }
            }
            return RDecimal;
        }
    } else {
        return RNotNumber;
    }
    return RNotNumber;
}

size_t powerUnsigned(size_t number, size_t power) {
    if(power == 0) {
        return 1;
    } else if (power == 1) {
        return number;
    } else {
        size_t result = number;
        size_t iterator;
        forAll(iterator, power - 1) {
            result *= number;
        }
        return result;
    }
}

constMethod(size_t, toSystemUInt, RString), rbool *isConvertedNormal) {
    if(object->type == RDataTypeASCII
            && object->size > 0) {

        size_t result = 0;
        size_t iterator = 0;
        byte currentDigit;


        forAll(iterator, object->size) { // from begin
            currentDigit = decimalDigitFromChar(object->data[iterator]);
            if (currentDigit != 10) {
                if (currentDigit != 0) {
                    result += (currentDigit * powerUnsigned(10, object->size - 1 - iterator));
                }
            } else {
                goto error;
            }
        }

        return result;
    } else {
        RError("Only ASCII compatible", object);
    }

error:
    if(isConvertedNormal != nil) {
        *isConvertedNormal = no;
    }
    return 0;
}