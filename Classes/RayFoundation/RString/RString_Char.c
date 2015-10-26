/**
 * RString_Char.c
 * Inlines for (char *) strings of RString API.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 05/11/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RString_Char.h"

inline method(void, replaceCSubstrings, RString), char *toReplace, char *replacer) {
    RString *toReplaceTemp = RCS(toReplace);
    RString *replacerTemp = RCS(replacer);
    $(object, m(replaceSubstrings, RString)), toReplaceTemp, replacerTemp);
    deallocator(toReplaceTemp);
    deallocator(replacerTemp);
}

inline constMethod(size_t, numberOfCSubstrings, RString), const char * string) {
    RString *temp   = RCS(string);
    size_t    result = $(object, m(numberOfSubstrings, RString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(rbool, isContainsCSubstring, RString), char *string) {
    RString *temp = RCS(string);
    if(temp != nil) {
        rbool result = $(object, m(isContainsSubstring, RString)), temp);
        deallocator(temp);
        return result;
    }
    return no;
}

inline method(RString *, deleteAllCSubstrings, RString), const char *substring) {
    RString *temp = RCS(substring);
    RString *result = nil;
    if(temp != nil) {
        result = $(object, m(deleteAllSubstrings, RString)), temp);
        deallocator(temp);
    }
    return result;
}

inline constMethod(RArray *, substringsSeparatedByCSymbols, RString), const char * const separatorsString) {
    RString *temp = RCS(separatorsString);
    RArray *result = $(object, m(substringsSeparatedBySymbols, RString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(RCompareFlags, compareWithC, RString), const char *const checkString) {
    RString *temp = RCS(checkString);
    RCompareFlags result = $(object, m(compareWith, RString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(rbool, startsOnC, RString), const char *const checkString) {
    size_t iterator = 0;
    while(checkString[iterator]) {
        if(checkString[iterator] == 0
           || object->data[iterator] != checkString[iterator]) {
            return no;
        }
        ++iterator;
    }
    return yes;
}
inline constMethod(rbool, endsOnStr, RString), const char *const checkString) {
    RString *temp = RCS(checkString);
    rbool result = $(object, m(endsOn, RString)), temp);
    deallocator(temp);
    return result;
}

method(void, appendString, RString), const char *string) {
    if(string != nil) {
        size_t stringSize = RStringLength(string);
        object->data = RReAlloc(object->data, arraySize(char, stringSize + object->size + 1));
        if(object->data != nil) {
            RMemMove(object->data + object->size, string, stringSize);
            object->data[stringSize + object->size] = 0;
            object->size += stringSize;

        } elseError(
                RError("RCS. appendString. realloc error.", object)
        );

    } elseWarning(
            RWarning("RString. appendString. Bad concatenate string.", object)
    );
}