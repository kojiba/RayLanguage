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

#include "../RBasics/RBasics.h"
#include "../RContainers/RArray.h"

typedef struct RCString RCString;

// basics
size_t    indexOfFirstCharacterCString (const char *string, size_t size, char character); // returns index, or size
size_t    indexOfLastCharacterCString  (const char *string, size_t size, char character); // returns index, or size
RCString* randomRCString               (void);
char      randomCharacter              (void);

class(RCString) //------------------------------------

    char    *baseString; // 0-terminated c-string
    size_t   size;       // size without '\0' character

endOf(RCString) //------------------------------------

// Constructor - Destructor - Reallocation
constructor (RCString));
destructor  (RCString);
printer     (RCString);

method(void,                flush,                        RCString));                                         // deletes old string

// Setters
method(RCString *,          setString,                    RCString),    const char *string);                  // copy characters
method(RCString *,          setConstantString,            RCString),    char const *string);                  // copy pointer

// Replace
method(void,                replaceCharacters,            RCString),    char characterToReplace, char replacer);
method(void,                replaceSubstrings,            RCString),    RCString *toReplace, RCString *replacer);

// Info
method(size_t,              numberOfCharacters,           RCString),    char character);
method(size_t,              numberOfSubstrings,           RCString),    RCString *string);
method(static inline rbool, isContains,                   RCString),    char character);
method(static inline rbool, isContainsSubsting,           RCString),    RCString *string);

// Deletions
method(RCString *,          deleteAllCharacters,          RCString),    char character);                      // returns reference (not copy!)
method(void,                removeRepetitionsOf,          RCString),    char character);

method(RCString *,          deleteAllSubstrings,          RCString),    const RCString *substring);           // returns reference (not copy!)
method(void,                removeRepetitionsOfString,    RCString),    const RCString *substring);

method(RCString *,          deleteCharacterAt,            RCString),    size_t index);                        // returns reference (not copy!)
method(void,                deleteInRange,                RCString),    RRange range);                        // shifts string

// Substrings and Copies
method(RCString *,          setSubstringInRange,          RCString),    RRange range, const char *string);    // returns reference (not copy!)
method(RCString *,          insertSubstringAt,            RCString),    RCString *substring, size_t place);   // returns reference (not copy!)

method(RCString *,          substringToSymbol,            RCString),    char symbol);                         // or nil
method(RCString *,          substringInRange,             RCString),    RRange range);                        // substring is a copy, basic method, that uses others
method(RCString *,          substringByBounds,            RCString),    RBounds bounds);                      // substring is a copy, by nesting
method(RArray *,            substringsSeparatedBySymbol,  RCString),    char symbol);                         // or nil, RArray is sizeToFit, subs are copies
method(RArray *,            substringsSeparatedBySymbols, RCString),    RCString *separatorsString);          // or nil, RArray is sizeToFit, subs are copies

method(RCString *,          copy,                         RCString));

// Comparator
method(RCompareFlags,       compareWith,                  RCString),    const RCString *checkString);

// Concatenate
method(void,                concatenate,                  RCString),    const RCString *string);

// Conversions
method(RCString*,           toUpperCase,                  RCString));                                          // returns reference (not copy!)
method(RCString*,           toLowerCase,                  RCString));                                          // returns reference (not copy!)

// With file
method(void,                appendToFile,                 RCString),    const char *filename);
RCString* RCStringFromFile(const char *filename);

//----------------------------------------------------------------------------------

#define makeRCString()          $(nil, c(RCString)))
#define printRString(string)    $(string, p(RCString)) );
#define RS(CString)             $(makeRCString(), m(setConstantString, RCString)), CString) // makes constant
#define RSC(CString)            $(makeRCString(), m(setString, RCString)), CString)         // makes copy from constant
#define deleteRCS(string)       $(string , d(RCString)) ); deallocator(string)

#endif /*__R_C_STRING_H__*/