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

#include <RBasics.h>
#include <RArray.h>

typedef struct RCString RCString;

// basics
size_t    indexOfFirstCharacterCString (const char *string, size_t size, char character); // returns index, or size
size_t    indexOfLastCharacterCString  (const char *string, size_t size, char character); // returns index, or size
char*     copyOfString                 (const char *string);
RCString* randomRCString               (void);
char      randomCharacter              (void);

class(RCString) //------------------------------------

    char    *baseString; // 0-terminated c-string
    size_t   size;       // size without '\0' character

endOf(RCString) //------------------------------------

// Make RCS constant start constant ""-string
RCString makeRCSConstant(char *string);

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
method(inline size_t,       numberOfLines,                RCString));

// Deletions
method(RCString *,          deleteAllCharacters,          RCString),    char character);                      // returns reference (not copy!)
method(void,                removeRepetitionsOf,          RCString),    char character);

method(RCString *,          deleteAllSubstrings,          RCString),    const RCString *substring);           // returns reference (not copy!)
method(void,                removeRepetitionsOfString,    RCString),    const RCString *substring);

method(RCString *,          deleteCharacterAt,            RCString),    size_t index);                        // returns reference (not copy!)
method(void,                deleteInRange,                RCString),    RRange range);                        // shifts string, main method
method(inline void,         trimTail,                     RCString),    size_t size);                         // deletes start end
method(inline void,         trimHead,                     RCString),    size_t size);                         // deletes start start

// Substrings and Copies
method(RCString *,          setSubstringInRange,          RCString),    RRange range, const char *string);    // returns reference (not copy!)
method(RCString *,          insertSubstringAt,            RCString),    RCString *substring, size_t place);   // returns reference (not copy!)

method(RCString *,          substringToSymbol,            RCString),    char symbol);                         // or nil
method(RCString *,          substringInRange,             RCString),    RRange range);                        // substring is a copy, basic method, that uses others
method(RCString *,          substringByBounds,            RCString),    RBounds bounds);                      // substring is a copy, by nesting (search first and last)
method(RArray *,            substringsSeparatedBySymbol,  RCString),    char symbol);                         // or nil, RArray is sizeToFit, subs are copies
method(RArray *,            substringsSeparatedBySymbols, RCString),    RCString *separatorsString);          // or nil, RArray is sizeToFit, subs are copies
method(inline RArray *,     substringsSeparatedBySymCStr, RCString),    char *separatorsString);              // or nil, RArray is sizeToFit, subs are copies

method(RCString *,          copy,                         RCString));

// Comparator
method(RCompareFlags,       compareWith,                  RCString),    const RCString *checkString);
method(RCompareFlags,       compareWithStr,               RCString),    const char *const checkString);
method(rbool,               startsOnStr,                  RCString),    const char *const checkString);
method(rbool,               startsOn,                     RCString),    const RCString *const checkString);
method(rbool,               endsOnStr,                    RCString),    const char *const checkString);
method(rbool,               endsOn,                       RCString),    const RCString *const checkString);

// Concatenate
method(void,                concatenate,                  RCString),    const RCString *string);
method(void,                appendString,                 RCString),    const char *string);
method(void,                append,                       RCString),    const char character);

// Conversions
method(RCString *,           toUpperCase,                 RCString));                                          // returns reference (not copy!)
method(RCString *,           toLowerCase,                 RCString));                                          // returns reference (not copy!)

// With file
method(void,                appendToFile,                 RCString),    const char *filename);
RCString* RCStringFromFile(const char *filename); // may not be 0-terminated, use it like a buffer with file content

//----------------------------------------------------------------------------------

#define makeRCString()             $(nil, c(RCString)))
#define printRString(string)       $(string, p(RCString)) );
#define RS(CString)                $(makeRCString(), m(setConstantString, RCString)), CString) // makes constant, ATTENTION need to be deallocated, but not destructed
#define RSC(CString)               $(makeRCString(), m(setString, RCString)), CString)         // makes copy from constant
#define makeStringArrayFrom(array) array->destructorDelegate = d(RCString); array->printerDelegate = p(RCString);

#endif /*__R_C_STRING_H__*/