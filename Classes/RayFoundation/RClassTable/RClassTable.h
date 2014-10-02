#include "../RayFoundation.h"
#include "../RArray/RArray.h"
#include "RClassNamePair/RClassNamePair.h"

#ifndef __CLASS_TABLE_H__
#define __CLASS_TABLE_H__


class(RClassTable) //------------------------------------------------------------
    discipleOf(RArray)
    discipleOf(RCompareDelegate)

endOf(RClassTable) //-------------------------------------------------------

constructor (RClassTable));
destructor  (RClassTable);

printer    (RClassTable);
singleton  (RClassTable);

method (uint64_t,      registerClassWithName,    RClassTable),    char *name); // strings must be constant
method (uint64_t,      getIdentifierByClassName, RClassTable),    char *name); // strings must be constant
method (uint64_t,      getNumberOfClasses,       RClassTable));

//----------------------------------------------------------------------------------

#define makeRCTable() $(NULL, c(RClassTable)) );

// some substitutions macro for better syntax
#if defined(RCTSingleton) || defined(registerClassOnce) || defined(printRCTS) \
    || defined(releaseRCTS) || defined(flushRCTS) || defined(getIdentifierByName)
    #error "Ray Class Table Defines Error - some already defined"
#else
#define RCTSingleton              singletonCall(RClassTable)
#define flushRCTS                 $(master(RCTSingleton,RDynamicArray), m(flush, RDynamicArray)));
#define releaseRCTS               d(RClassTable)(RCTSingleton); free(singletonCall(RClassTable))
#define registerClassOnce(name)   $(RCTSingleton, m(registerClassWithName, RClassTable)), name)
#define printRCTS                 $(RCTSingleton, p(RClassTable)) )
#define getIdentifierByName(name) $(RCTSingleton, m(getIdentifierByClassName, RClassTable)), name)
#endif

#endif /*__CLASS_TABLE_H__*/