/**
 * RString_Char.h
 * Inlines for (char *) strings of RString API.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RString.h"

extern      method(void,          replaceCSubstrings,            RString),    char *toReplace, char *replacer);

extern constMethod(size_t,        numberOfCSubstrings,           RString),    const char *string);
extern constMethod(rbool,         isContainsCSubstring,          RString),    char *string);

extern      method(RString *,     deleteAllCSubstrings,          RString),    const char *substring);

extern constMethod(RArray *,      substringsSeparatedByCSymbols, RString),    const char * const separatorsString); // or nil, RArray is sizeToFit, subs are copies

extern constMethod(RCompareFlags, compareWithC,                  RString),    const char * const checkString);
extern constMethod(rbool,         startsOnC,                     RString),    const char * const checkString);
extern constMethod(rbool,         endsOnC,                       RString),    const char * const checkString);
extern      method(void,          appendString,                  RString),    const char *string);
