#ifndef __R_CLASS_NAME_PAIR_H__
#define __R_CLASS_NAME_PAIR_H__

#include "../../RSyntax.h"
#include "../../RCString/RCString.h"

class(RClassNamePair)
    discipleOf(RCString)

    uint64_t idForClassName;
endOf(RClassNamePair)


constructor(RClassNamePair));
destructor (RClassNamePair);
printer    (RClassNamePair);

method(RCompareFlags, compareWith, RClassNamePair), RClassNamePair *checkPair);


#endif /*__R_CLASS_NAME_PAIR_H__*/