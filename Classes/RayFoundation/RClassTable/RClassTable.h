/**
 * RClassTable.h
 * ClassTable for manage(get info, etc.) instances in runtime.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __CLASS_TABLE_H__
#define __CLASS_TABLE_H__

#include <RSyntax.h>
#include <RClassNamePair.h>
#include <RArray.h>

class(RClassTable) //------------------------------------------------------------
    discipleOf(RArray)
    discipleOf(RCompareDelegate)

endOf(RClassTable) //-------------------------------------------------------

constructor (RClassTable));
destructor  (RClassTable);
printer     (RClassTable);
singleton   (RClassTable);

method(size_t,      registerClassWithName,    RClassTable),    char *name); // strings must be constant, or self-cleanup
method(size_t,      getIdentifierByClassName, RClassTable),    char *name); // strings must be constant, or self-cleanup
method(RCString*,   getClassNameByIdentifier, RClassTable),    size_t id);
method(size_t,      getNumberOfClasses,       RClassTable));

//----------------------------------------------------------------------------------

#define makeRCTable() $(nil, c(RClassTable)) );

// some substitutions macro for better syntax
#if defined(RCTSingleton) || defined(registerClassOnce) || defined(printRCTS) \
    || defined(releaseRCTS) || defined(flushRCTS) || defined(getIdentifierByName)
    #error "Ray Class Table Defines Error - some already defined"
#else
#define RCTSingleton              singletonCall(RClassTable)
#define flushRCTS()               $(master(RCTSingleton, RArray), m(flush, RArray)));
#define releaseRCTS               d(RClassTable)(RCTSingleton); free(singletonCall(RClassTable))
#define registerClassOnce(name)   $(RCTSingleton, m(registerClassWithName, RClassTable)), name) // return id, or 0 if fails. [0; 3] ids is reserved
#define printRCTS                 $(RCTSingleton, p(RClassTable)) )
#define getIdentifierByName(name) $(RCTSingleton, m(getIdentifierByClassName, RClassTable)), name)
#endif

#endif /*__CLASS_TABLE_H__*/