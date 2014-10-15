#include "../RayFoundation.h"

#ifndef __R_FLOATING_ENUM_H__
#define __R_FLOATING_ENUM_H__

#include "../RStringDictionary/RStringDictionary.h"

class(RFloatingEnum)
    discipleOf(RDictionary)

    byte     isChangesAfterCall;               // if == 1 - rebase enum after each compare
    pointer (*nextElementForCode)(pointer code); // get - set delegate, to create random elements
endOf(RFloatingEnum)

constructor (RFloatingEnum), pointer (*)(pointer), RRange range); // range - is incrementing enum values from start value
destructor  (RFloatingEnum);
printer     (RFloatingEnum);

method(RCompareFlags, compareValueToKey, RFloatingEnum),    pointer value, pointer key);
method(void,          rebase,            RFloatingEnum));                                   // changes codes(values) depend on nextElementForCode delegate

#define RFloatingEnumName(enumName) concatenate(RFloatingEnum, enumName)
#define createFloatingEnum(enumName, delegate, enumFirstValue, enumValuesCount) \
const RFloatingEnum *RFloatingEnumName(enumName) = $(NULL, c(RFloatingEnum)), delegate, makeRRange(enumFirstValue, enumValuesCount))

#define checkValueToKey(enumName, value, key) $(RFloatingEnumName(enumName), m(compareValueToKey, RFloatingEnum)), value, key)
#define printEnum(enumName) $(RFloatingEnumName(enumName), p(RFloatingEnum)) )
#define rebaseEnum(enumName) $(RFloatingEnumName(enumName), m(rebase, RFloatingEnum)) )

#endif /*__R_FLOATING_ENUM_H__*/