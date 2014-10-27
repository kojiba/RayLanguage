/**
 * RFloatingEnum.h
 * Realization of enum, that can change values in runtime in Ray additions.
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

#ifndef __R_FLOATING_ENUM_H__
#define __R_FLOATING_ENUM_H__

#include "../RDictionary/RDictionary.h"

class(RFloatingEnum)
    discipleOf(RDictionary)

    byte      isChangesAfterCall;                 // if == 1 - rebase enum after each compare
    pointer (*nextElementForCode)(pointer code); // get - set delegate, to create random elements
endOf(RFloatingEnum)

constructor (RFloatingEnum), pointer (*)(pointer), RRange range); // range - is incrementing enum values from start value
destructor  (RFloatingEnum);
printer     (RFloatingEnum);

method(RCompareFlags, compareValueToKey, RFloatingEnum),    pointer value, pointer key);
method(void,          changesAfterCall,  RFloatingEnum),    byte flag);
method(void,          rebase,            RFloatingEnum));                                   // changes codes(values) depend on nextElementForCode delegate

#define RFloatingEnumName(enumName)           concatenate(RFloatingEnum, enumName)

#define createFloatingEnum(enumName, delegate, enumFirstValue, enumValuesCount) \
    const RFloatingEnum *RFloatingEnumName(enumName) = $(NULL, c(RFloatingEnum)), delegate, makeRRange(enumFirstValue, enumValuesCount))

#define checkValueToKey(enumName, value, key) $(RFloatingEnumName(enumName), m(compareValueToKey, RFloatingEnum)), value, key)
#define printEnum(enumName)                   $(RFloatingEnumName(enumName), p(RFloatingEnum)) )
#define rebaseEnum(enumName)                  $(RFloatingEnumName(enumName), m(rebase, RFloatingEnum)) )
#define deleteEnum(enumName)                  $(RFloatingEnumName(enumName), d(RFloatingEnum)) ); deallocator(RFloatingEnumName(enumName))
#define setFloatingEnum(enumName)             $(RFloatingEnumName(enumName), m(changesAfterCall, RFloatingEnum)), 1)

#endif /*__R_FLOATING_ENUM_H__*/