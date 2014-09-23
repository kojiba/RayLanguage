#ifndef __RFINDER_DELEGATE_H__
#define __RFINDER_DELEGATE_H__

#include "../../RayFoundation.h"

class(RFinderDelegate)
    virtualMethod(byte, checkObject, RFinderDelegate), pointer objectToCheck);
    pointer etaloneObject;
endOfClass(RFinderDelegate)

method(byte, checkObject, RFinderDelegate), pointer objectToCheck);

#endif /*__RFINDER_DELEGATE_H__*/