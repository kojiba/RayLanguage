/**
 * @file RStringDictionary.h
 * @brief Realization of C dictionary(map) for RCString, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 * @created 3.10.14 9:48:30
 */

#ifndef __R_STRING_DICTIONARY_H__
#define __R_STRING_DICTIONARY_H__

#include "../RayFoundation.h"
#include "../RDictionary/RDictionary.h"

class(RStringDictionary)
    discipleOf(RDictionary)

endOf(RStringDictionary)

constructor (RStringDictionary));
destructor  (RStringDictionary);
printer     (RStringDictionary);

method(void,     setObjectForKey, RStringDictionary),    RCString *value, RCString *key); // must be copies, use RSC, or RCString copy
method(pointer , getObjectForKey, RStringDictionary),    RCString *key);                  // return POINTER, NOT COPY, be aware!

#endif /*__R_STRING_DICTIONARY_H__*/