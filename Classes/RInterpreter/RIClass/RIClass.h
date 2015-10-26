#ifndef __RAY_CLASS_H__
#define __RAY_CLASS_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RContainers/RArray.h"
#include "../../RayFoundation/RString/RString.h"
#include "../../RayFoundation/RClassTable/RClassTable.h"

class(RayClass)

    RArray  *methods;
    RArray  *properties;
    RArray  *masterClasses;
    RArray  *statics;
    RString *name;
endOf(RayClass)

constructor (RayClass));
destructor  (RayClass);
printer     (RayClass);

// Try to deserialize start source
method(RString*, deserializeFromCode,         RayClass),    RString *code);

#endif /*__RAY_CLASS_H__*/