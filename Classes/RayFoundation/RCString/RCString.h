/**
 * @file RCString.h
 * @brief Realization of wrapper on '\0' - terminated C-strings, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"
#include "../RBasics/RBasics.h"
#include "../RArray/RArray.h"

// basics
uint64_t indexOfFirstCharacterCString (const char *string, uint64_t size, char character); // returns index, or size
uint64_t indexOfLastCharacterCString  (const char *string, uint64_t size, char character); // returns index, or size

class(RCString) //------------------------------------

    char    *baseString; // 0-terminated c-string
    uint64_t size;       // size without '\0' character

endOf(RCString) //------------------------------------

// Constructor - Destructor - Reallocation
constructor (RCString));
destructor  (RCString);
printer     (RCString);

method(void,             flush,                        RCString));                                      // deletes old string

// Setters
method(RCString *,       setString,                    RCString),    const char *string);               // copy characters
method(RCString *,       setConstantString,            RCString),    char *string);                     // copy pointer

// Options
method(uint64_t,         numberOfRepetitions,          RCString),    char character);
method(RCString *,       deleteAllCharacters,          RCString),    char character);                   // returns not copy!
method(RCString *,       deleteAllSubstrings,          RCString),    const RCString *substring);        // returns not copy!
method(RCString *,       deleteCharacterAt,            RCString),    uint64_t index);                   // returns not copy!
method(void,             deleteInRange,                RCString),    RRange range);                     // shifts string

// Substrings and Copies
method(RCString *,       setSubstringInRange,          RCString),    RRange range, const char *string); // basic method, that uses others

method(RCString *,       substringToSymbol,            RCString),    char symbol);                      // or NULL
method(RArray *,         substringsSeparatedBySymbol,  RCString),    char symbol);                      // or NULL, RArray is sizeToFit
method(RCString *,       substringInRange,             RCString),    RRange range);                     // substring is a copy
method(RCString *,       substringByBounds,            RCString),    RBounds bounds);                   // substring is a copy, by nesting

method(RCString *,       copy,                         RCString));

// Comparator
method(RCompareFlags,    compareWith,                  RCString),    const RCString *checkString);

// With file
method(void,             fromFile,                     RCString),    const RCString *filename);          // deletes old string

RCString *randomRCString  (void);
char      randomCharacter (void);

//----------------------------------------------------------------------------------

#define makeRCString()          $(NULL, c(RCString)))
#define printRString(_RCString) $(_RCString, p(RCString)) );
#define RS(CString)             $(makeRCString(), m(setConstantString, RCString)), CString) // makes constant
#define RSC(CString)            $(makeRCString(), m(setString, RCString)), CString)         // makes copy from constant

#endif /*__R_C_STRING_H__*/