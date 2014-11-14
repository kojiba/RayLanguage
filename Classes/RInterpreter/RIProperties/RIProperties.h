#ifndef __RAY_PROPERTY_H__
#define __RAY_PROPERTY_H__

#include "../../RayFoundation/RSyntax.h"

typedef enum RayPropertyType {
    PTReadOnly,
    PTWriteOnly,
    PTReadWrite,
    PRInner,
} RayPropertyType;

class(RIProperty)
    RayPropertyType type;
endOf(RIProperty)

#endif /*__RAY_PROPERTY_H__*/