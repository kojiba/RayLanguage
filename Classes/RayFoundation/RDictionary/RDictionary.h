/**
 * @file RDictionary.h
 * @brief Realization of C dictionary(map) pair, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_DICTIONARY_H__
#define __R_DICTIONARY_H__

#include "../RayFoundation.h"

class(RDictionary)

endOfClass(RDictionary)

constructor(RDictionary));
destructor(RDictionary);

method(void, setObjectForKey, RDictionary), pointer key, pointer value);
method(pointer, getObjectForKey, RDictionary), pointer key);

printer(RDictionary);

#endif /*__R_DICTIONARY_H__*/