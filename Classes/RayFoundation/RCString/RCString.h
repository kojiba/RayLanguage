/**
 * RCString.h
 * Realization of wrapper on '\0' - terminated C-strings, in Ray additions.
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
#include "RayFoundation/RMemoryOperations/RBytes.h"

#define baseInputStringSize 40 // defines default buffer for getInputString

typedef RData RString;

RString * randomASCIIString(void); // uses rand(), string [10; 50] length, uses randomCharacter
char      randomCharacter(void); // uses rand() [34; 125] ascii

char *     cstringWithFormat             (const char *format, ...);
char *     vcstringWithFormat            (const char *format, va_list list);

#define makeRString(size) makeRData(RAlloc(size), size, RDataTypeASCII)

// Constructor - Destructor - Reallocation
constructor (RCString));
destructor  (RCString);
printer     (RCString);
RCString *  stringWithFormat(char *format, ...); // uses vsnprintf, string must be deleted with deleter(obj, RCString)
RCString *  RCStringInit(pointer data, size_t size);
void        stringDeleter(RCString *string);     // call destructor and deallocates ptr

method(void,                flush,                        RCString));                                         // deletes old string
// Setters
method(RCString *,          setString,                    RCString),    const char *string); // copy characters
method(RCString *,          setConstantString,            RCString),    char const *string); // copy pointer

// Replace
method(void,                replaceCharacters,            RCString),    char characterToReplace, char replacer);
method(void,                replaceSubstrings,            RCString),    RCString *toReplace, RCString *replacer);

// Info - universal encoding
constMethod(size_t,         numberOfCharacters,           RCString),    char character);
constMethod(size_t,         numberOfSubstrings,           RCString),    const RCString * const string);
constMethod(rbool,          isContains,                   RCString),    char character);
constMethod(rbool,          isContainsSubstring,          RCString),    RCString *string);
extern
constMethod(size_t,         numberOfLines,                RCString));
constMethod(size_t,         indexOfSubstring,             RCString),    RCString *string);

// Deletions
method(RCString *,          deleteAllCharacters,          RCString),    char character);            // returns reference (not copy!)
method(void,                removeRepetitionsOf,          RCString),    char character);

method(RCString *,          deleteAllSubstrings,          RCString),    const RCString *substring); // returns reference (not copy!)
method(void,                removeRepetitionsOfString,    RCString),    const RCString *substring);

method(RCString *,          deleteCharacterAt,            RCString),    size_t index);              // returns reference (not copy!)
method(void,                deleteInRange,                RCString),    RRange range);              // shifts string, main method
extern
method(void,                trimTail,                     RCString),    size_t size);
extern
method(void,                trimHead,                     RCString),    size_t size);
extern
method(void,                trimAfterString,              RCString),    RCString *string);
extern
method(void,                trimToString,                 RCString),    RCString *string);

// Subs and Copies
method(RCString *,          setSubstringInRange,          RCString),    RRange range, const char * const string); // returns reference (not copy!)
method(RCString *,          insertSubstringAt,            RCString),    RCString *substring, size_t place);       // returns reference (not copy!)

constMethod(RCString *,     substringToSymbol,            RCString),    char symbol);                             // or nil
constMethod(RCString *,     substringInRange,             RCString),    RRange range);                            // substring is a copy, basic method, that uses others
constMethod(RCString *,     substringByBounds,            RCString),    RBounds bounds);                          // substring is a copy, by nesting (search first and last)
constMethod(RArray *,       substringsSeparatedBySymbol,  RCString),    char symbol);                             // or nil, RArray is sizeToFit, subs are copies
constMethod(RArray *,       substringsSeparatedBySymbols, RCString),    const RCString * const separatorsString); // or nil, RArray is sizeToFit, subs are copies
constMethod(RArray *,       substringsSeparatedByString,  RCString),    const RCString * const separatorString);  // separatorString length > 1, not use for one symbol
constMethod(RArray *,       separatedByStringWithShield,  RCString),    const RCString * const separatorString, const RCString * const shield);

constMethod(RCString *,     copy,                         RCString));

// Comparator - universal encoding
constMethod(RCompareFlags,  compareWith,                  RCString),    const RCString *checkString);
extern
constMethod(rbool,          isEqualTo,                    RCString),    const RCString *checkString);
constMethod(rbool,          startsOn,                     RCString),    const RCString *const checkString);
constMethod(rbool,          endsOn,                       RCString),    const RCString *const checkString);

// Concatenate - universal encoding
method(void,                concatenate,                  RCString),    const RCString *string);
method(void,                append,                       RCString),    const char character);

// Conversions (only ASCII)
method(RCString *,          toUpperCase,                  RCString)); // returns reference (not copy!)
method(RCString *,          toLowerCase,                  RCString)); // returns reference (not copy!)

//----------------------------------------------------------------------------------

#define makeRCString()             $(nil, c(RCString)))
#define RCS(CString)               $(makeRCString(), m(setConstantString, RCString)), CString) // makes weak reference, need to be deallocated, but not destructed
#define RSC(CString)               $(makeRCString(), m(setString, RCString)), CString)         // makes copy from cstring

#define stringDelegates(array)     $(array, m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RCString)); \
                                   $(array, m(setDestructorDelegate, RArray)), (DestructorDelegate) stringDeleter)
#endif /*__R_C_STRING_H__*/