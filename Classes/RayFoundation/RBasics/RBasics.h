/**
 * RBasics.h
 * Some workers structures for Ray.
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

#ifndef __R_BASICS_H__
#define __R_BASICS_H__

#include "../RSyntax.h"

typedef enum RCompareFlags{
    not_equals = 0,
    equals     = 1,
    longer,
    shorter,
    bigger,
    smaller
} RCompareFlags;

typedef struct RRange {
    uint64_t from;
    uint64_t count;
} RRange;

inline RRange          makeRRange    (uint64_t from, uint64_t count);
inline RRange          makeRRangeTo  (uint64_t from, uint64_t to);
inline RCompareFlags   compareRRange (RRange first, RRange second);
inline byte            isInRange     (RRange range, uint64_t value);

typedef struct RBounds {
    char startSymbol;
    char endSymbol;
} RBounds;

inline RBounds makeRBounds     (char startSymbol, char endSymbol);
inline byte    isValueInBounds (RBounds bounds, char value);
inline byte    compareRBounds  (RBounds first, RBounds second);

protocol(RCompareDelegate) //--------------------------------------------
    RCompareFlags (*virtualCompareMethod)(pointer first, pointer second);
    pointer         etaloneObject;

endOf(RCompareDelegate) //-----------------------------------------------

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck);

#endif