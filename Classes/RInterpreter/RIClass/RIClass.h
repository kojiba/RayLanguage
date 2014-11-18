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

method(RCString*, methodSingletonName,         RayClass));

// fully typedefs method singleton struct with all methods
method(RCString*, methodSingletonFor,          RayClass),    RClassTable *table);
method(RCString*, classStructWithMasterClass,  RayClass));

#endif /*__RAY_CLASS_H__*/