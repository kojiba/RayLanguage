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

class(RCString)

    char    *baseString;
    uint64_t size;

endOf(RCString)

constructor(RCString));
destructor(RCString);

method(RCString *,    setString, RCString),           char *string);    // copy characters
method(RCString *,    setConstantString, RCString),   char *string);    // copy pointer

method(RCString *,    setSubstringInRange, RCString), RRange *range, char *string);
method(RCompareFlags, compareWith, RCString),         RCString *checkString);

printer(RCString);

#define makeRCString() $(NULL, c(RCString)))
#define printRString(_RCString) $(_RCString, p(RCString)) );
#define RS(CString) $(makeRCString(), m(setConstantString, RCString)), CString);

#endif /*__R_C_STRING_H__*/