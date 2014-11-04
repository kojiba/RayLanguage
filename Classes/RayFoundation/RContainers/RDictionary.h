/**
 * RDictionary.h
 * Realization of C dictionary(map) pair, in Ray additions.
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

#ifndef __R_DICTIONARY_H__
#define __R_DICTIONARY_H__

#include "RArray.h"

class(RDictionary) //---------------------------------------------------------------------
    discipleOf(RCompareDelegate)

members
    RArray *keys;
    RArray *values;

endOf(RDictionary) //----------------------------------------------------------------

constructor (RDictionary));
destructor  (RDictionary);

method(void,       initDelegate,    RDictionary),    const RCompareDelegate *delegate);
method(void,       setObjectForKey, RDictionary),    pointer value, pointer key);
method(pointer,    getObjectForKey, RDictionary),    pointer key);

printer(RDictionary);

// some macros for shorter work
#define makeRDictionary()       $(nullPtr, c(RDictionary)) );
#define printRD(rayDictionary)  $(rayDictionary, p(RDictionary)) );
#define deleteRD(rayDictionary) $(rayDictionary, d(RDictionary)) );\
                                deallocator(rayDictionary)


#endif /*__R_DICTIONARY_H__*/