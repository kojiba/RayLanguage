#include "../../RayFoundation/RSyntax.h"

#ifndef __RAY_PROPERTY_H__
#define __RAY_PROPERTY_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

typedef enum RayPropertyType {
    PTReadOnly,
    PTReadWrite,
    PTInner,
} RayPropertyType;

class(RayProperty)
    size_t memSizeType;     // like void, int, double etc.
    RayPropertyType type;
    RCString *name;
endOf(RayProperty)

constructor (RayProperty));
destructor  (RayProperty);
printer     (RayProperty);

method(RCString *, serializeToCType, RayProperty), RClassTable *delegate);

#endif /*__RAY_PROPERTY_H__*/