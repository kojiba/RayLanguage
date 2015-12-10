/**
 * RString.h
 * Sized strings with encoding.
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

#ifndef __R_STRING_H__
#define __R_STRING_H__

#include "RayFoundation/RBasics/RBasics.h"
#include "RayFoundation/RContainers/RArray.h"
#include "RayFoundation/RMemoryOperations/RData.h"

typedef RData RString;

RString * randomASCIIString(void); // uses rand(), string [10; 50] length, uses randomCharacter
char      randomCharacter(void); // uses rand() [34; 125] ascii

char *    cstringWithFormat (const char *format, ...);
char *    vcstringWithFormat(const char *format, va_list list);

// Constructor - Destructor - Reallocation

extern destructor(RString);
extern printer   (RString);

RString * stringWithFormat(char *format, ...); // ASCII
#define   RStringDeleter RDataDeleter

method(RString*, setConstantString, RString), const char *nullTerminatedString);
method(void,     validateToASCII,   RString)); // remove all non ascii-chars

// Replace
       method(void,               replaceCharacters,            RString), char characterToReplace, char replacer);
extern method(void,               replaceSubstrings,            RString), const RString *toReplace, const RString *replacer);

// Info - universal encoding
extern constMethod(size_t,        numberOfCharacters,           RString),    char character);
extern constMethod(size_t,        numberOfSubstrings,           RString),    const RString * const string);
extern constMethod(rbool,         isContains,                   RString),    char character);
extern constMethod(rbool,         isContainsSubstring,          RString),    const RString *string);
extern constMethod(size_t,        numberOfLines,                RString));
extern constMethod(size_t,        indexOfSubstring,             RString),    const RString *string);

// Deletions
extern method(RString *,          deleteAllCharacters,          RString),    char character);            // returns reference (not copy!)
extern method(void,               removeRepetitionsOf,          RString),    char character);

extern method(RString *,          deleteAllSubstrings,          RString),    const RString *substring); // returns reference (not copy!)
extern method(void,               removeRepetitionsOfString,    RString),    const RString *substring);

       method(RString *,          deleteCharacterAt,            RString),    size_t index);              // returns reference (not copy!)
extern method(void,               deleteInRange,                RString),    RRange range);              // shifts string, main method
extern method(void,               trimTail,                     RString),    size_t size);
extern method(void,               trimHead,                     RString),    size_t size);
extern method(void,               trimAfterString,              RString),    const RString *string);
extern method(void,               trimToString,                 RString),    const RString *string);

// Subs and Copies
extern      method(RString *,     setSubstringInRange,          RString),    RRange range, const RString *substring); // returns reference (not copy!)
extern      method(RString *,     insertSubstringAt,            RString),    const RString *substring, size_t place);       // returns reference (not copy!)
       constMethod(RString *,     substringToSymbol,            RString),    char symbol);                             // or nil
extern constMethod(RString *,     substringInRange,             RString),    RRange range);                            // substring is a copy, basic method, that uses others
extern constMethod(RArray *,      substringsSeparatedBySymbols, RString),    const RString * const separatorsString); // or nil, RArray is sizeToFit, subs are copies
extern constMethod(RArray *,      substringsSeparatedByString,  RString),    const RString * const separatorString);  // separatorString length > 1, not use for one symbol
extern constMethod(RArray *,      separatedByStringWithShield,  RString),    const RString * const separatorString, const RString * const shield);

constMethod(RString *,     copy,                         RString));

// Comparator - universal encoding
extern constMethod(RCompareFlags, compareWith,                  RString),    const RString *checkString);
extern constMethod(rbool,         isEqualTo,                    RString),    const RString *checkString);
extern constMethod(rbool,         startsOn,                     RString),    const RString *const checkString);
extern constMethod(rbool,         endsOn,                       RString),    const RString *const checkString);

// Concatenate - universal encoding
extern method(void,               concatenate,                  RString),    const RString *string);
extern method(void,               append,                       RString),    const char character);

// Conversions (only ASCII)
       method(RString *,          toUpperCase,                  RString)); // returns reference (not copy!)
       method(RString *,          toLowerCase,                  RString)); // returns reference (not copy!)

//----------------------------------------------------------------------------------

#define RCS(CString)               $(allocator(RData), m(setConstantString, RString)), CString) // makes weak reference, need to be deallocated, but not destructed
#define RSC(CString)               makeRData(getByteArrayCopy((byte*)CString, sizeof(CString) - 1), sizeof(CString) - 1, RDataTypeASCII) // makes copy from constant string

#define stringDelegates(array)     $(array, m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RString)); \
                                   $(array, m(setDestructorDelegate, RArray)), (DestructorDelegate) stringDeleter)
#endif /*__R_STRING_H__*/