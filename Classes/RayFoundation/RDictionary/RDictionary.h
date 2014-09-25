/**
 * @file RDictionary.h
 * @brief Realization of C dictionary(map) pair, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_DICTIONARY_H__
#define __R_DICTIONARY_H__

#include "../RayFoundation.h"
#include "../RArray/RArray.h"

class(RDictionary)
    RArray *keys;
    RArray *values;
endOfClass(RDictionary)

constructor(RDictionary));
destructor(RDictionary);

method(void, setObjectForKey, RDictionary), pointer value, pointer key);
method(pointer, getObjectForKey, RDictionary), pointer key);

printer(RDictionary);

// some macros for shorter work
#define makeRDictionary() $(NULL, c(RDictionary)) );

#define setObject(obj) , obj
#define forKey(key) , key)
#define inDictionary(dict) m(setObjectForKey, RDictionary)(dict
#define printRD(rayDictionary) $(rayDictionary, p(RDictionary)) );

#define deleteRD(rayDictionary) $(rayDictionary, d(RDictionary)) );\
                                deallocator(rayDictionary)


#endif /*__R_DICTIONARY_H__*/