#ifndef __R_CLASS_NAME_PAIR_H__
#define __R_CLASS_NAME_PAIR_H__

#include "../../RayFoundation.h"

class(RClassNamePair)
    char *className;
    uint64_t idForClassName;
endOfClass(RClassNamePair)


constructor(RClassNamePair));

destructor(RClassNamePair);

printer(RClassNamePair);

#endif /*__R_CLASS_NAME_PAIR_H__*/