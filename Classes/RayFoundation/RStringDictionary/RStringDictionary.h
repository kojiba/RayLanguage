/**
 * RStringDictionary.h
 * Realization of C dictionary(map) for RCString, in Ray additions.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 3.10.14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_STRING_DICTIONARY_H__
#define __R_STRING_DICTIONARY_H__

#include "../RDictionary/RDictionary.h"
#include "../RCString/RCString.h"

class(RStringDictionary)
    discipleOf(RDictionary)

endOf(RStringDictionary)

constructor (RStringDictionary));
destructor  (RStringDictionary);
printer     (RStringDictionary);

method(void,     setObjectForKey, RStringDictionary),    RCString *value, RCString *key); // must be copies, use RSC, or RCString copy
method(pointer , getObjectForKey, RStringDictionary),    RCString *key);                  // return POINTER, NOT COPY, be aware!

#endif /*__R_STRING_DICTIONARY_H__*/