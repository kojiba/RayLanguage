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

typedef struct RRange {
    uint64_t from;
    uint64_t count;
} RRange;

RRange makeRRange   (uint64_t from, uint64_t count);
RRange makeRRangeTo (uint64_t from, uint64_t to);

typedef struct RBounds {
    char startSymbol;
    char endSymbol;
} RBounds;

RBounds makeRBounds (char startSymbol, char endSymbol);

typedef enum RCompareFlags{
    equals,
    not_equals,
    longer,
    shorter,
    bigger,
    smaller
} RCompareFlags;

protocol(RCompareDelegate)
    RCompareFlags (*virtualCompareMethod)(pointer first, pointer second);
    pointer         etaloneObject;

endOf(RCompareDelegate)

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck);

#endif