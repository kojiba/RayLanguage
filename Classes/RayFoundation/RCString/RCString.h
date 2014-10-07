/**
 * @file RCString.h
 * @brief Realization of wrapper on '\0' - terminated C-strings, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"
#include "../RArray/RCompareDelegate/RCompareDelegate.h"

typedef struct RRange {
    uint64_t from;
    uint64_t count;
} RRange;

RRange makeRRange   (uint64_t from, uint64_t count);
RRange makeRRangeTo (uint64_t from, uint64_t to);

typedef struct RBounds {
    char startSymbol;
    char endSymbol;
} RBounds;

RBounds makeRBounds (char startSymbol, char endSymbol);

// basics
uint64_t indexOfFirstCharacterCString (const char *string, uint64_t size, char character);
uint64_t indexOfLastCharacterCString  (const char *string, uint64_t size, char character);

class(RCString) //------------------------------------

    char    *baseString; // 0-terminated c-string
    uint64_t size;       // size without '\0' character

endOf(RCString) //------------------------------------

// Constructor - Destructor - Reallocation
constructor(RCString));
destructor(RCString);
method(void,             flush,              RCString));                                       // deletes old string

// Setters
method(RCString *,       setString,           RCString),    const char *string);               // copy characters
method(RCString *,       setConstantString,   RCString),    char *string);                     // copy pointer

// Options
method(uint64_t,         numberOfRepetitions, RCString),    char character);
method(RCString *,       deleteAllCharacters, RCString),    char character);
method(RCString *,       deleteAllSubstrings, RCString),    const RCString *substring);

// Substrings and Copies
method(RCString *,       setSubstringInRange, RCString),    RRange range, const char *string);
method(RCString *,       getSubstringInRange, RCString),    RRange range);                     // substring is a copy
method(void,             deleteInRange,       RCString),    RRange range);                     // with shift
method(RCString *,       copy,                RCString));

// Comparator
method(RCompareFlags,    compareWith,         RCString),    const RCString *checkString);

// With file
method(void,             fromFile,            RCString),    const RCString *filename);          // deletes old string

printer(RCString);

staticMethod(RCString *, randomString,        RCString));
staticMethod(char ,      randomCharacter,     RCString));

//----------------------------------------------------------------------------------

#define makeRCString()          $(NULL, c(RCString)))
#define printRString(_RCString) $(_RCString, p(RCString)) );
#define randomRCString()        sm(randomString, RCString)(NULL)
#define RS(CString)             $(makeRCString(), m(setConstantString, RCString)), CString) // makes constant
#define RSC(CString)            $(makeRCString(), m(setString, RCString)), CString)         // makes copy from constant

#endif /*__R_C_STRING_H__*/