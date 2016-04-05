/**
 * @file RBasics.h
 * @brief Some workers structures for Ray.
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 10/21/2014
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

#ifndef __R_BASICS_H__
#define __R_BASICS_H__

#include "RayFoundation/RSyntax.h"

typedef enum RCompareFlags { ///< flags for compareWith structs function
    not_equals = 0,
    equals     = 1,
    longer,
    shorter,
    bigger,
    smaller
} RCompareFlags;

static const size_t RNotFound = SIZE_MAX; ///< not found flag for searching functions
static const size_t REquals = SIZE_MAX;   ///< flag for compareMemory function

// -----------------------------------------------------------------------

typedef struct RRange { ///< struct to handle range
    size_t start;
    size_t size;
} RRange;

/**
 * Main constructor for RRange instances.
 *
 * @param      start     Begin index of range
 * @param      size      Range long
 *
 * @return instance of RRange
 */
extern RRange          makeRRange    (size_t start, size_t size);

/**
 * Second constructor for RRange instances, where defines from and to index.
 *
 * @param      from     Begin index of range
 * @param      to       End index of range
 *
 * @return instance of RRange with range.size = to - from;
 */
extern RRange          makeRRangeTo  (size_t from,  size_t to);

/**
 * Compares two ranges function.
 *
 * @param      first     Range to compare
 * @param      second    Range to compare
 *
 * @return RCompareFlags : longer, shorter, - size compare; smaller, bigger, - start index compare; equals for equal ranges
 */
extern RCompareFlags   compareRRange (RRange first, RRange second);

/**
 * Checking is value in specified range.
 *
 * @param      range     Range to check
 * @param      value     Value to check
 *
 * @return yes if (rangeBegin, value, rangeEnd)
 */
extern rbool           isInRange     (RRange range, size_t value);

/**
 * Checking is ranges overlapping.
 *
 * @param      first     Range to check
 * @param      second    Range to check
 *
 * @return yes if, (range1 () range2), no if (range1) (range2)
 */
extern rbool           isOverlappingRRange(RRange first, RRange second);

/**
 * Default printer.
 * Note: output format [rangeBegin; rangeEnd]
 *
 * @param      object    Range to print
 */
printer(RRange);

// -----------------------------------------------------------------------

typedef struct RFindResult { ///< struct for handling search results
    pointer object; ///< pointer to object found or nil
    size_t  index; ///< index of object found or RNotFound
} RFindResult;

// -----------------------------------------------------------------------

typedef void (* DestructorDelegate)(pointer); ///< default destructor function signature
typedef void (* PrinterDelegate)(pointer);    ///< default printer function signature

/**
 * Default comparator function signature.
 * Note: output format [rangeBegin; rangeEnd]
 *
 * @param      first    First object to compare
 * @param      second   Second object to compare
 *
 * @returns one of RCompareFlags
 */
typedef RCompareFlags (* ComparatorDelegate)(pointer first, pointer second);

/**
 * Default enumerator function signature.
 * Note: output format [rangeBegin; rangeEnd]
 *
 * @param      context   REnumerateDelegate.context, used for data communication, or nil
 * @param      object    Enumerating object
 * @param      iterator  Current iterator/index of enumerating object
 *
 * @returns yes for continue enumerating, no for stop enumerating
 */
typedef rbool         (* EnumeratorDelegate)(pointer context, pointer object, size_t iterator);

// -----------------------------------------------------------------------

protocol(RCompareDelegate)
    ComparatorDelegate virtualCompareMethod; ///< overriding/delegating compare method
    pointer            etaloneObject;        ///< object to compare with
endOf(RCompareDelegate)

/**
 * Default comparator function.
 *
 * @param      first     Object to compare
 * @param      second    Object to compare
 *
 * @returns RCompareFlags.equals if pointers are equal
 */
extern RCompareFlags defaultComparator(pointer first, pointer second);

// -----------------------------------------------------------------------

protocol(REnumerateDelegate)
    EnumeratorDelegate virtualEnumerator; ///< overriding/delegating enumerating method
endOf(REnumerateDelegate)

// -----------------------------------------------------------------------

typedef struct SerializerData { ///< worker struct for serializers
                   size_t  size;
                  pointer  serializePtrStart;
    struct SerializerData *next; ///< for multiple-serialize
} SerializerData;



#ifdef RAY_BLOCKS_ON
    typedef rbool (^REnumerateBlock)(pointer objectIterator, size_t iterator); ///< default enumerator block signature
#endif

#endif