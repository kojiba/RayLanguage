/**
 * RCString_Numbers.c
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

#include <RCString_Numbers.h>

#pragma mark Basics

static inline rbool isDecimalDigit(char character) {
    if(character < '0' || character > '9') {
        return no;
    } else {
        return yes;
    }
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
            || character > 64 && character < 71
            || character > 96 && character < 103) {
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

constMethod(RCString *, toRCString, RNumberSystemBase)) {
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

#pragma mark RCString Additions

constMethod(RNumberSystemBase, isNumber, RCString)) {
    size_t iterator;
    // not too large
    if(object->size < 200) {
        // it may be hex, or octal, or binary
        if(object->baseString[0] == '0') {

            // if hex
            if(object->baseString[1] == 'x'
                    || object->baseString[1] == 'X') {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isHexDigit(object->baseString[iterator]) == no) {
                        return RNotNumber;
                    }
                }
                return RHex;

            // binary
            } else if (object->baseString[0] == 'b'){
                // check symbols
                for(iterator = 1; iterator < object->size; ++iterator) {
                    if(isBinaryDigit(object->baseString[iterator]) == no) {
                        return RNotNumber;
                    }
                }

            // octal
            } else {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isOctalDigit(object->baseString[iterator]) == no) {
                        return RNotNumber;
                    }
                }
                return ROctal;
            }

        // decimal
        } else {
            forAll(iterator, object->size) {
                if (isDecimalDigit(object->baseString[iterator]) == no) {
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