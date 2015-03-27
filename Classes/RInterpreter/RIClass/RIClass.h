#ifndef __RAY_CLASS_H__
#define __RAY_CLASS_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RContainers/RArray.h"
#include "../../RayFoundation/RCString/RCString.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

class(RayClass)

    RArray  *methods;
    RArray  *properties;
    RArray  *masterClasses;
    RArray  *statics;
    RCString *name;
endOf(RayClass)

constructor (RayClass));
destructor  (RayClass);
printer     (RayClass);

// Try to deserialize start source
method(RCString*, deserializeFromCode,         RayClass),    RCString *code);

#endif /*__RAY_CLASS_H__*/