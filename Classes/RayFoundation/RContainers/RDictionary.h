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

#include <RArray.h>

class(RDictionary) //--------------------------
    ComparatorDelegate comparator;

    RArray *keys;
    RArray *values;

endOf(RDictionary) //--------------------------

constructor (RDictionary));
destructor  (RDictionary);

     method(void,            initDelegate,        RDictionary),    ComparatorDelegate comparator); // comparator for keys
     method(void,            setObjectForKey,     RDictionary),    pointer value, pointer key);
     method(void,            sizeToFit,           RDictionary));
constMethod(pointer,         getObjectForKey,     RDictionary),    pointer key);

printer(RDictionary);

#pragma mark Init from scratch

RDictionary * dictionaryFromPairs(pointer firstKey, ...); // dict from array of pointers of type key, value, last must be nil)

// some macros for shorter work
#define makeRDictionary()       $(nil, c(RDictionary)) )
#define RD                      dictionaryFromPairs

#endif /*__R_DICTIONARY_H__*/