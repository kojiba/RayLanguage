/**
 * RCString_Char.c
 * Inlines for (char *) strings of RCString API.
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

#include <RCString_Char.h>

inline method(void, replaceCSubstrings, RCString), char *toReplace, char *replacer) {
    RCString *toReplaceTemp = RCS(toReplace);
    RCString *replacerTemp = RCS(replacer);
    $(object, m(replaceSubstrings, RCString)), toReplaceTemp, replacerTemp);
    deallocator(toReplaceTemp);
    deallocator(replacerTemp);
}

inline constMethod(size_t, numberOfCSubstrings, RCString), const char * string) {
    RCString *temp   = RCS(string);
    size_t    result = $(object, m(numberOfSubstrings, RCString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(rbool, isContainsCSubstring, RCString), char *string) {
    RCString *temp = RCS(string);
    if(temp != nil) {
        rbool result = $(object, m(isContainsSubstring, RCString)), temp);
        deallocator(temp);
        return result;
    }
    return no;
}

inline method(RCString *, deleteAllCSubstrings, RCString), const char *substring) {
    RCString *temp = RCS(substring);
    RCString *result = nil;
    if(temp != nil) {
        result = $(object, m(deleteAllSubstrings, RCString)), temp);
        deallocator(temp);
    }
    return result;
}

inline constMethod(RArray *, substringsSeparatedByCSymbols, RCString), const char * const separatorsString) {
    RCString *temp = RCS(separatorsString);
    RArray *result = $(object, m(substringsSeparatedBySymbols, RCString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(RCompareFlags, compareWithC, RCString), const char *const checkString) {
    RCString *temp = RCS(checkString);
    RCompareFlags result = $(object, m(compareWith, RCString)), temp);
    deallocator(temp);
    return result;
}

inline constMethod(rbool, startsOnC, RCString), const char *const checkString) {
    size_t iterator = 0;
    while(checkString[iterator]) {
        if(checkString[iterator] == 0
           || object->baseString[iterator] != checkString[iterator]) {
            return no;
        }
        ++iterator;
    }
    return yes;
}
inline constMethod(rbool, endsOnStr, RCString), const char *const checkString) {
    RCString *temp = RCS(checkString);
    rbool result = $(object, m(endsOn, RCString)), temp);
    deallocator(temp);
    return result;
}

method(void, appendString, RCString), const char *string) {
    if(string != nil) {
        size_t stringSize = RStringLength(string);
        object->baseString = RReAlloc(object->baseString, arraySize(char, stringSize + object->size + 1));
        if(object->baseString != nil) {
            RMemMove(object->baseString + object->size, string, stringSize);
            object->baseString[stringSize + object->size] = 0;
            object->size += stringSize;

        } elseError(
                RError("RCS. appendString. realloc error.", object)
        );

    } elseWarning(
            RWarning("RCString. appendString. Bad concatenate string.", object)
    );
}