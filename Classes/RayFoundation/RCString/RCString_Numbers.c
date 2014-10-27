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

#include "RCString_Numbers.h"

static inline RBool isDecimalDigit(char character) {
    if(character < '0' || character > '9') {
        return NO;
    } else {
        return YES;
    }
}

static inline RBool isOctalDigit(char character) {
    if(character < '0' || character > '8') {
        return NO;
    } else {
        return YES;
    }
}

static inline RBool isHexDigit(char character) {
    if(isDecimalDigit(character)
            || character > 60 && character < 67
            || character > 40 && character < 47) {
        return YES;
    } else {
        return NO;
    }
}

method(RNumberSystemBase, isNumber, RCString)) {
    size_t iterator;
    // not too large
    if(object->size < 200) {
        // it may be hex, or octal
        if(object->baseString[0] == '0') {
            // if hex
            if(object->baseString[1] == 'x'
                    || object->baseString[1] == 'X') {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isHexDigit(object->baseString[iterator]) == NO) {
                        return RNotNumber;
                    }
                }
                return RHex;

            // octal
            } else {
                // check symbols
                for(iterator = 2; iterator < object->size; ++iterator) {
                    if(isOctalDigit(object->baseString[iterator]) == NO) {
                        return RNotNumber;
                    }
                }
                return ROctal;
            }

        // decimal
        } else {
            forAll(iterator, object->size) {
                if (isDecimalDigit(object->baseString[iterator]) == NO) {
                    return RNotNumber;
                }
            }
            return RDecimal;
        }
    } else {
        return RNotNumber;
    }
}