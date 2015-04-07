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

#include <RSyntax.h>

typedef enum RCompareFlags{
    not_equals = 0,
    equals     = 1,
    longer,
    shorter,
    bigger,
    smaller
} RCompareFlags;

// -----------------------------------------------------------------------

typedef struct RRange {
    size_t start;
    size_t size;
} RRange;

extern RRange          makeRRange    (size_t start, size_t size);
extern RRange          makeRRangeTo  (size_t from,  size_t to);
extern RCompareFlags   compareRRange (RRange first, RRange second);
extern rbool           isInRange     (RRange range, size_t value);
extern rbool           isOverlapping (RRange first, RRange second);
printer(RRange);

// -----------------------------------------------------------------------

typedef struct RBounds {
    char startSymbol;
    char endSymbol;
} RBounds;

extern RBounds makeRBounds     (char startSymbol, char endSymbol);
extern rbool   isValueInBounds (RBounds bounds, char value);
extern rbool   compareRBounds  (RBounds first, RBounds second);

// -----------------------------------------------------------------------

typedef struct RFindResult {
    pointer object;
    size_t  index;
} RFindResult;

// -----------------------------------------------------------------------

typedef void (* DestructorDelegate)(pointer);
typedef void (* PrinterDelegate)(pointer);

typedef RCompareFlags (* ComparatorDelegate)(pointer, pointer);
typedef rbool         (* EnumeretorDelegate)(pointer, size_t);  // if return yes - continue enumerating, else stops it

// -----------------------------------------------------------------------

protocol(RCompareDelegate) //--------------------------------------------
    ComparatorDelegate virtualCompareMethod;
    pointer            etaloneObject;
endOf(RCompareDelegate) //-----------------------------------------------

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck);

// -----------------------------------------------------------------------

protocol(REnumerateDelegate) //--------------------------------------------
    EnumeretorDelegate virtualCheckObject;
endOf(REnumerateDelegate)

method(rbool, checkObject, REnumerateDelegate), pointer data, size_t index); // call virtualCheck for all while not no

#endif