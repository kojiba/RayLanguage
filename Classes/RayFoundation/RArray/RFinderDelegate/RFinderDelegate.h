#ifndef __RFINDER_DELEGATE_H__
#define __RFINDER_DELEGATE_H__

#include "../../RayFoundation.h"

typedef enum RFinderDelegateFlags{
    equals,
    not_equals
} RFinderDelegateFlags;

protocol(RFinderDelegate)
    virtualMethod(RFinderDelegateFlags, checkObject, RFinderDelegate), pointer objectToCheck);
    pointer etaloneObject;
endOf(RFinderDelegate)

method(RFinderDelegateFlags, checkObject, RFinderDelegate), pointer objectToCheck);

#endif /*__RFINDER_DELEGATE_H__*/