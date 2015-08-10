/**
 * RCString_Char.h
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

#include "RCString.h"

extern      method(void,          replaceCSubstrings,            RCString),    char *toReplace, char *replacer);

extern constMethod(size_t,        numberOfCSubstrings,           RCString),    const char * string);
extern constMethod(rbool,         isContainsCSubstring,          RCString),    char *string);

extern      method(RCString *,    deleteAllCSubstrings,          RCString),    const char *substring);

extern constMethod(RArray *,      substringsSeparatedByCSymbols, RCString),    const char * const separatorsString); // or nil, RArray is sizeToFit, subs are copies

extern constMethod(RCompareFlags, compareWithC,                  RCString),    const char *const checkString);
extern constMethod(rbool,         startsOnC,                     RCString),    const char *const checkString);
extern constMethod(rbool,         endsOnC,                       RCString),    const char *const checkString);
extern      method(void,          appendString,                  RCString),    const char *string);
