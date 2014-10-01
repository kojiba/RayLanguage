#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"
#include "../RArray/RCompareDelegate/RCompareDelegate.h"

typedef struct RRange {
    uint64_t from;
    uint64_t count;
} RRange;

RRange* makeRRange   (uint64_t from, uint64_t count);
RRange* makeRRangeTo (uint64_t from, uint64_t to);

class(RCString) //------------------------------------

    char    *baseString; // \0-terminated c-string
    uint64_t size;       // size without \0 character

endOf(RCString) //------------------------------------

// constructor - destructor - reallocation
constructor(RCString));
destructor(RCString);
method(void,             flush, RCString));

// setters
method(RCString *,       setString, RCString),           char *string);                 // copy characters
method(RCString *,       setConstantString, RCString),   char *string);                 // copy pointer

// substrings and copies
method(RCString *,       setSubstringInRange, RCString), RRange *range, char *string);
method(RCString *,       getSubstringInRange, RCString), RRange *range);
method(void,             deleteInRange, RCString),       RRange *range);                // with shift
method(RCString *,       copy, RCString));

// comparator
method(RCompareFlags,    compareWith, RCString),         RCString *checkString);

// reads from file
method(void,             fromFile, RCString),            RCString *filename);           // deletes old string

printer(RCString);

staticMethod(RCString *, randomString, RCString));
staticMethod(char ,      randomCharacter, RCString));


#define makeRCString()          $(NULL, c(RCString)))
#define printRString(_RCString) $(_RCString, p(RCString)) );
#define randomRCString()        sm(randomString, RCString)(NULL)
#define RS(CString)             $(makeRCString(), m(setConstantString, RCString)), CString)

#endif /*__R_C_STRING_H__*/