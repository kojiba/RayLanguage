/**
 * RString.h
 * Sized strings with encoding.
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

#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "RayFoundation/RBasics/RBasics.h"
#include "RayFoundation/RContainers/RArray.h"
#include "RayFoundation/RMemoryOperations/RData.h"

#define baseInputStringSize 40 // defines default buffer for getInputString

typedef RData RString;

RString * randomASCIIString(void); // uses rand(), string [10; 50] length, uses randomCharacter
char      randomCharacter(void); // uses rand() [34; 125] ascii

char *     cstringWithFormat             (const char *format, ...);
char *     vcstringWithFormat            (const char *format, va_list list);

#define makeRString(size) makeRData(RAlloc(size), size, RDataTypeASCII)

// Constructor - Destructor - Reallocation

#define constructorOfRString makeRData(nil, size, RDataTypeASCII)

extern destructor(RString);
extern printer(RString);

RString * stringWithFormat(char *format, ...); // ASCII
#define RStringDeleter RDataDeleter

method(void, setConstantString, RString), char *nullTerminatedString);

// Replace
method(void, replaceCharacters, RString), char characterToReplace, char replacer);
method(void, replaceSubstrings, RString),      RString *toReplace,      RString *replacer);

// Info - universal encoding
constMethod(size_t,         numberOfCharacters,           RString),    char character);
constMethod(size_t,         numberOfSubstrings,           RString),    const RString * const string);
constMethod(rbool,          isContains,                   RString),    char character);
constMethod(rbool,          isContainsSubstring,          RString),    RString *string);
extern
constMethod(size_t,         numberOfLines,                RString));
constMethod(size_t,         indexOfSubstring,             RString),    RString *string);

// Deletions
method(RString *,          deleteAllCharacters,      RString),    char character);            // returns reference (not copy!)
method(void,               removeRepetitionsOf,          RString),    char character);

method(RString *,          deleteAllSubstrings,          RString),    const RString *substring); // returns reference (not copy!)
method(void,               removeRepetitionsOfString,    RString),    const RString *substring);

method(RString *,          deleteCharacterAt,            RString),    size_t index);              // returns reference (not copy!)
method(void,               deleteInRange,                RString),    RRange range);              // shifts string, main method
extern
method(void,               trimTail,                     RString),    size_t size);
extern
method(void,               trimHead,                     RString),    size_t size);
extern
method(void,               trimAfterString,              RString),    RString *string);
extern
method(void,               trimToString,                 RString),    RString *string);

// Subs and Copies
method(RString *,          setSubstringInRange,          RString),    RRange range, RString *substring); // returns reference (not copy!)
method(RString *,          insertSubstringAt,            RString),    RString *substring, size_t place);       // returns reference (not copy!)

constMethod(RString *,     substringToSymbol,            RString),    char symbol);                             // or nil
constMethod(RString *,     substringInRange,             RString),    RRange range);                            // substring is a copy, basic method, that uses others
constMethod(RArray *,      substringsSeparatedBySymbols, RString),    const RString * const separatorsString); // or nil, RArray is sizeToFit, subs are copies
constMethod(RArray *,      substringsSeparatedByString,  RString),    const RString * const separatorString);  // separatorString length > 1, not use for one symbol
constMethod(RArray *,      separatedByStringWithShield,  RString),    const RString * const separatorString, const RString * const shield);

constMethod(RString *,     copy,                         RString));

// Comparator - universal encoding
constMethod(RCompareFlags,  compareWith,                  RString),    const RString *checkString);
extern
constMethod(rbool,          isEqualTo,                    RString),    const RString *checkString);
constMethod(rbool,          startsOn,                     RString),    const RString *const checkString);
constMethod(rbool,          endsOn,                       RString),    const RString *const checkString);

// Concatenate - universal encoding
method(void,                concatenate,                  RString),    const RString *string);
method(void,                append,                       RString),    const char character);

// Conversions (only ASCII)
method(RString *,          toUpperCase,                  RString)); // returns reference (not copy!)
method(RString *,          toLowerCase,                  RString)); // returns reference (not copy!)

//----------------------------------------------------------------------------------

#define RCS(CString)               makeRData((byte*)CString, sizeof(CString), RDataTypeASCII) // makes weak reference, need to be deallocated, but not destructed
#define RSC(CString)               makeRData(getByteArrayCopy((byte*)CString, sizeof(CString)), sizeof(CString), RDataTypeASCII) // makes copy from constant string

#define stringDelegates(array)     $(array, m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RString)); \
                                   $(array, m(setDestructorDelegate, RArray)), (DestructorDelegate) stringDeleter)
#endif /*__R_C_STRING_H__*/