#ifndef __RAY_CLASS_H__
#define __RAY_CLASS_H__

#include "../../RayFoundation/RSyntax.h"
#include "../../RayFoundation/RContainers/RArray.h"
#include "../RIObject/RIObject.h"

class(RayClass)
    discipleOf(RIObject)

    RArray *methods;
    RArray *properties;
    RArray *masterClasses;
    RArray *statics;
endOf(RayClass)

#endif /*__RAY_CLASS_H__*/