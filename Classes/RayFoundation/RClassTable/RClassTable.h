#include "../RayFoundation.h"
#include "../RDynamicArray/RDynamicArray.h"

#ifndef __CLASS_TABLE_H__
#define __CLASS_TABLE_H__


$class(RClassTable) //------------------------------------------------------------

    $discipleOf(RDynamicArray)

$endOfClass(RClassTable) //-------------------------------------------------------

$constructor(RClassTable));
$destructor(RClassTable);

$method(uint64_t, registerClassWithName, RClassTable), char *name);
$method(uint64_t, getNumberOfClasses, RClassTable));
$method(uint64_t, getIdentifierByClassName, RClassTable), char *name);

$printer(RClassTable);
$singleton(RClassTable);

// some substitutions macro for better syntax
#if defined(RCTSingleton) || defined(registerClassOnce) || defined(printRCTS) || defined(releaseRCTS) || defined(flushRCTS)
    #error "Ray Class Table Defines Error - some already defined"
#else
#define RCTSingleton singleton(RClassTable)
#define flushRCTS $(master(RCTSingleton,RDynamicArray), m(flush, RDynamicArray)));
#define releaseRCTS d(RClassTable)(RCTSingleton); free(singleton(RClassTable))
#define registerClassOnce(name) $(RCTSingleton, m(registerClassWithName, RClassTable)), name)
#define printRCTS $(RCTSingleton, p(RClassTable)) )
#endif

#endif /*__CLASS_TABLE_H__*/