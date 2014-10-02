#ifndef __R_VIRTUAL_FUNCTION_H__
#define __R_VIRTUAL_FUNCTION_H__

#include "../../Classes/RayFoundation/RayFoundation.h"
#include "../../Classes/RayFoundation/RArray/RArray.h"

class(RVirtualFunction)
    RCString *name;
    byte   *body;
endOf(RVirtualFunction)

constructor(RVirtualFunction));
destructor(RVirtualFunction);

printer(RVirtualFunction);


#endif /*__R_VIRTUAL_FUNCTION_H__*/