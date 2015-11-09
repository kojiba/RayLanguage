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

#include "RayFoundation/RSyntax.h"

typedef enum RCompareFlags{
    not_equals = 0,
    equals     = 1,
    longer,
    shorter,
    bigger,
    smaller
} RCompareFlags;

static const size_t RNotFound = SIZE_MAX;
static const size_t REquals = SIZE_MAX;

// -----------------------------------------------------------------------

typedef struct RRange {
    size_t start;
    size_t size;
} RRange;

extern RRange          makeRRange    (size_t start, size_t size);
extern RRange          makeRRangeTo  (size_t from,  size_t to);
extern RCompareFlags   compareRRange (RRange first, RRange second);
extern rbool           isInRange     (RRange range, size_t value);
extern rbool           isOverlappingRRange(RRange first, RRange second);
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

typedef RCompareFlags (* ComparatorDelegate)(pointer first, pointer second);
typedef rbool         (* EnumeratorDelegate)(pointer context, pointer object, size_t iterator);  // if return yes - continue enumerating, else stops it

// -----------------------------------------------------------------------

protocol(RCompareDelegate)
    ComparatorDelegate virtualCompareMethod;
    pointer            etaloneObject;
endOf(RCompareDelegate)

extern RCompareFlags defaultComparator(pointer first, pointer second);

// -----------------------------------------------------------------------

protocol(REnumerateDelegate)
    EnumeratorDelegate virtualEnumerator;
endOf(REnumerateDelegate)

typedef struct SerializerData { // worker struct for serializers
                   size_t  size;
                  pointer  serializePtrStart;
    struct SerializerData *next; // for multiple-serialize
} SerializerData;



#ifdef RAY_BLOCKS_ON
    typedef rbool (^REnumerateBlock)(pointer objectIterator, size_t iterator);
#endif

#endif