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
size_t    indexOfFirstCharacterCString  (const char *string, size_t size, char character); // returns index, or size
size_t    indexOfLastCharacterCString   (const char *string, size_t size, char character); // returns index, or size
char*     copyOfCString                 (const char *string);
RCString* randomRCString                (void);
char      randomCharacter               (void);
char*     cstringWithFormat             (const char *format, ...);
char*     vcstringWithFormat            (const char *format, va_list list);

class(RCString) //------------------------------------

    char    *baseString; // 0-terminated c-string
    size_t   size;       // size without '\0' character

endOf(RCString) //------------------------------------

// Constructor - Destructor - Reallocation
constructor (RCString));
destructor  (RCString);
printer     (RCString);
RCString*   stringWithFormat(char *format, ...); // uses vsnprintf, string must be deleted with deleter(obj, RCString)

void        stringDeleter(RCString *string); // call destructor and deallocates ptr

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
static inline
method(rbool,               isContains,                   RCString),    char character);
static inline
method(rbool,               isContainsSubsting,           RCString),    RCString *string);
extern inline
method(size_t,              numberOfLines,                RCString));

// Deletions
method(RCString *,          deleteAllCharacters,          RCString),    char character);                      // returns reference (not copy!)
method(void,                removeRepetitionsOf,          RCString),    char character);

method(RCString *,          deleteAllSubstrings,          RCString),    const RCString *substring);           // returns reference (not copy!)
extern inline
method(RCString *,          deleteAllSubstringsCStr,      RCString),    const char     *substring);

method(void,                removeRepetitionsOfString,    RCString),    const RCString *substring);

method(RCString *,          deleteCharacterAt,            RCString),    size_t index);                        // returns reference (not copy!)
method(void,                deleteInRange,                RCString),    RRange range);                        // shifts string, main method
extern inline
method(void,                trimTail,                     RCString),    size_t size);                         // deletes start end
extern inline
method(void,                trimHead,                     RCString),    size_t size);                         // deletes start start

// Subs and Copies
method(RCString *,          setSubstringInRange,          RCString),    RRange range, const char *string);    // returns reference (not copy!)
method(RCString *,          insertSubstringAt,            RCString),    RCString *substring, size_t place);   // returns reference (not copy!)

method(RCString *,          substringToSymbol,            RCString),    char symbol);                         // or nil
method(RCString *,          substringInRange,             RCString),    RRange range);                        // substring is a copy, basic method, that uses others
method(RCString *,          substringByBounds,            RCString),    RBounds bounds);                      // substring is a copy, by nesting (search first and last)
method(RArray *,            substringsSeparatedBySymbol,  RCString),    char symbol);                         // or nil, RArray is sizeToFit, subs are copies
method(RArray *,            substringsSeparatedBySymbols, RCString),    RCString *separatorsString);          // or nil, RArray is sizeToFit, subs are copies
extern inline
method(RArray *,            substringsSeparatedBySymCStr, RCString),    char *separatorsString);              // or nil, RArray is sizeToFit, subs are copies
method(RArray *,            substringsSeparatedByString,  RCString),    RCString *separatorString);           // separatorString length > 1, not use for one symbol

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
#define RS(CString)                $(makeRCString(), m(setConstantString, RCString)), CString) // makes constant, ATTENTION need to be deallocated, but not destructed
#define RSC(CString)               $(makeRCString(), m(setString, RCString)), CString)         // makes copy from constant
#define stringDelegates(array)     array->destructorDelegate = (DestructorDelegate) stringDeleter; \
                                   array->printerDelegate = (PrinterDelegate) p(RCString)
#endif /*__R_C_STRING_H__*/