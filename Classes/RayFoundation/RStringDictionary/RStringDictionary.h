/**
 * @file RStringDictionary.h
 * @brief Realization of C dictionary(map) for RCString, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_STRING_DICTIONARY_H__
#define __R_STRING_DICTIONARY_H__

#include "../RayFoundation.h"
#include "../RDictionary/RDictionary.h"

class(RStringDictionary)
    discipleOf(RDictionary)
    discipleOf(RCompareDelegate)

endOf(RStringDictionary)

constructor(RStringDictionary));
destructor(RStringDictionary);

#endif /*__R_STRING_DICTIONARY_H__*/