/**
 * @file RDictionary.h
 * @brief Realization of C dynamic dictionary
 *        (not full std::map, NSDictionary analog), in Ray additions.
 *        Not use hashmaps pattern, or trees.
 *
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 9/25/2014
 * @par Ukraine Kharkiv
 *
 *//*
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 */

#ifndef __R_DICTIONARY_H__
#define __R_DICTIONARY_H__

#include "RArray.h"

/**
 * @par RDictionary main struct is:
 *
 *      indexes : [0,    1,    2,    3,    ...]
 *      keys    : [key0, key1, key2, key3, ...]
 *      values  : [val0, val1, val2, val3, ...]
 *
 * It means that index of key specifies index of value in values array.
 */
class(RDictionary) //--------------------------
    ComparatorDelegate comparator; ///< key comparator delegate

    RArray *keys;   ///< keys array
    RArray *values; ///< values array

endOf(RDictionary) //--------------------------


/**
 * Default constructor for RDictionary instances, uses defaults RArray.constructor.
 * Default key comparator is defaultComparator. (i.e. compares keys like a pointers)
 *
 * @return Created instance of RDictionary or nil if error occurs.
 */
constructor (RDictionary));

/**
 * Default destructor for RDictionary instances.
 * Deletes keys and values using RArray.destructor
 */
destructor  (RDictionary);

/**
 * Default printer for RDictionary instances.
 * Using keys and values printerDelegates.
 * Main format is key : value.
 */
printer     (RDictionary);

/**
 * Set new keys comparator.
 *
 * @param    comparator    New keys comparator
 */
    method(void,            initDelegate,        RDictionary),    ComparatorDelegate comparator); // comparator for keys

/**
 * Sets value  for specified key.
 * Keys and values auto-long-ups if needed.
 * Using RArray.findObjectWithDelegate for search keys,
 * also RArray.setObjectAtIndex and RArray.addObject.
 * Note: If find key in keys array - replace value at key index.
 *       If no - add key to key array, and add value to value array.
 *
 * @param    value  Some pointer or nil
 * @param    key    Key to object set
 */
    method(void,            setObjectForKey,     RDictionary),    pointer value, pointer key);

/**
 * Remove empty space in keys and values arrays,
 * using RArray.sizeToFit.
 */
    method(void,            sizeToFit,           RDictionary));

/**
 * Method to get object for key.
 * Using RArray.findObjectWithDelegate for search keys,
 * also RArray.objectAtIndex.
 * See 'RDictionary main struct' for understand how it searchs.
 *
 * @param    key    Key to it object get
 *
 * @return   Value from value array or nil, if key is not found
 */
constMethod(pointer,        getObjectForKey,     RDictionary),    pointer key);

#pragma mark Init from scratch

/**
 * Method to create RDictionary from variable argument list.
 * Format is dictionaryFromPairs(key1, value1, key2, value2, key3, value3, ..., nil)
 * RError if keysCount != valuesCount.
 * Using RArray.addObject, and RDictionary.sizeToFit.
 *
 * @param    firstKey    Argument list, which must be nil-terminated.
 *
 * @return   Created RDictionary instance
 *           or nil if allocation error occurs,
 *           or keysCount != valuesCount
 */
RDictionary * dictionaryFromPairs(pointer firstKey, ...); // dict from array of pointers of type key, value, last must be nil)

// some macros for shorter work
#define makeRDictionary()       $(nil, c(RDictionary)) )
#define RD                      dictionaryFromPairs

#endif /*__R_DICTIONARY_H__*/