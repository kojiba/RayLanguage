#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"

class(RCString)
    char *string;
    uint64_t size;
endOfClass(RCString)


constructor(RCString));

destructor(RCString);

method(void, setString, RCString), char *string);


#endif /*__R_C_STRING_H__*/