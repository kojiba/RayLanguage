/**
 * @file RDictionary.h
 * @brief Realization of C dictionary(map) pair, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_DICTIONARY_H__
#define __R_DICTIONARY_H__

#include "../RayFoundation.h"
#include "../RArray/RArray.h"

class(RDictionary) //---------------------------------------------------------------------
    discipleOf(RCompareDelegate)

members
    RArray *keys;
    RArray *values;

endOf(RDictionary) //----------------------------------------------------------------

constructor(RDictionary));
destructor(RDictionary);

method(void,       initDelegate,    RDictionary), RCompareDelegate *delegate);
method(void,       setObjectForKey, RDictionary), pointer value, pointer key);
method(pointer,    getObjectForKey, RDictionary), pointer key);

// for string dictionary
method(RCString *, getStringForKey, RDictionary), RCString *key);
method(void,       setStringForKey, RDictionary), RCString *key, RCString *string);

printer(RDictionary);

// some macros for shorter work
#define makeRDictionary() $(NULL, c(RDictionary)) );
#define printRD(rayDictionary) $(rayDictionary, p(RDictionary)) );

#define deleteRD(rayDictionary) $(rayDictionary, d(RDictionary)) );\
                                deallocator(rayDictionary)


#endif /*__R_DICTIONARY_H__*/