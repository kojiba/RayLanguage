#ifndef __R_COMPARE_DELEGATE_H__
#define __R_COMPARE_DELEGATE_H__

#include "../../RayFoundation.h"

typedef enum RCompareFlags{
    equals,
    not_equals
} RCompareFlags;

protocol(RCompareDelegate)
    RCompareFlags (*virtualCompareMethod)(pointer first, pointer second);
    pointer         etaloneObject;
endOf(RCompareDelegate)

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck);

#endif /*__R_COMPARE_DELEGATE_H__*/