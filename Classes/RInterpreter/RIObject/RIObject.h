#ifndef __RAY_INTERPRETATOR_OBJECT_H__
#define __RAY_INTERPRETATOR_OBJECT_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RCString/RCString.h"

protocol(RIObject)
    virtualMethod(void, printer)(void);
//    RCString *description;
endOf(RIObject)

#endif /*__RAY_INTERPRETATOR_OBJECT_H__*/