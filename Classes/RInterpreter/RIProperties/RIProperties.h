#ifndef __RAY_PROPERTY_H__
#define __RAY_PROPERTY_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"
#include "../RInterpereterConsts.h"

const char *    PropertyTypeToString(RayPropertyType type);
RayPropertyType PropertyTypeFromString(const char *string);


// ----------------------------

class(RayProperty)
    size_t           memSizeType; // like void, int, double etc.
    RayPropertyType  type;        // from RInterpereterConsts.h
    RArray          *names;
endOf(RayProperty)

constructor (RayProperty));
destructor  (RayProperty);
printer     (RayProperty);

method(RCString *, serializeToCType, RayProperty), RClassTable *delegate);

RayProperty* ParsePropertyString(RCString *code, RClassTable *delegate); // must be already preprocessed

extern inline
RayProperty* ParsePropertyCString(char *code, RClassTable *delegate);

#endif /*__RAY_PROPERTY_H__*/