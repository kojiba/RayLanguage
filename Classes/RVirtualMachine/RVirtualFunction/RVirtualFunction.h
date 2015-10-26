#ifndef __R_VIRTUAL_FUNCTION_H__
#define __R_VIRTUAL_FUNCTION_H__

#include <RayFoundation/RayFoundation.h>

class(RVirtualFunction)
    discipleOf(RData)

    RString   *name;
endOf(RVirtualFunction)

constructor (RVirtualFunction));
destructor  (RVirtualFunction);

printer(RVirtualFunction);


#endif /*__R_VIRTUAL_FUNCTION_H__*/