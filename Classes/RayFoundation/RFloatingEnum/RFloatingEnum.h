#include "../RayFoundation.h"

#ifndef __R_FLOATING_ENUM_H__
#define __R_FLOATING_ENUM_H__

#include "../RStringDictionary/RStringDictionary.h"

#if defined(REnum) || defined(endOfREnum) || defined(endREnumPoint)
    #error "RFloating enum working defines are redefined. Please, remove that defines"
 #else
    #define REnum(enumName) typedef enum enumName {
    #define endOfREnum(enumName) }; enumName ; concatenate(endOfEnum, enumName)();
    #define endREnumPoint(name) &concatenate(endOfEnum, name)
#endif

class(RFloatingEnum)
    discipleOf(RDictionary)

    pointer (*nextElementForCode)(pointer code);
endOf(RFloatingEnum)

constructor (RFloatingEnum), pointer (*)(pointer code), pointer enumStartPointer, pointer enumEndPointer);
destructor  (RFloatingEnum);
printer     (RFloatingEnum);
singleton   (RFloatingEnum);

method(RCompareFlags, compareValueToKey, RFloatingEnum) );

#define createFloatingEnum(enumName, delegate)

#endif /*__R_FLOATING_ENUM_H__*/