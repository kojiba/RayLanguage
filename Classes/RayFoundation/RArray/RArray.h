/**
 * @file RArray.h
 * @brief Realization of C dynamic array, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_ARRAY_H__
#define __R_ARRAY_H__

#include "../RayFoundation.h"
#include "RFinderDelegate/RFinderDelegate.h"

#define RAY_SHORT_DEBUG 451

typedef enum RDynamicArrayFlags {

    // basic errors
                 no_error,
    temp_allocation_error,
         allocation_error,

    // flags for findObjectWithDelegate
           object_founded, // or NULL

    // sort-flags for sortWithDelegate
             swap_objects,   // or 0

    // flag for checkIfIndexInArray
             index_exists,
     index_does_not_exist,

    // flag for shift
               shift_left,
              shift_right,

    // flag, determines bad newSize
    // (smaller that exist)
    // in addSize function
            bad_size,

} RDynamicArrayFlags;

static const uint64_t startSizeOfRdynamicArrayDefault = 100;
static const uint64_t sizeMultiplierOfRdynamicArrayDefault = 2;

class(RArray) //------------------------------------------------------------

members
    uint64_t startSize;                     // start size of array in elements
    uint64_t sizeMultiplier;                // size multiplier when auto-add-size
    uint64_t count;                         // count of elements in array
    uint64_t freePlaces;                    // count of free places for elements
    void (*destructorDelegate)(pointer);    // destructor of elements delegate
    void (*printerDelegate)(pointer);       // printer of elements delegate
    pointer *array;                         // array

endOfClass(RArray) //-------------------------------------------------------

// constructor - destructor - reallocation
constructor(RArray), RDynamicArrayFlags *error);
destructor(RArray);

method(RDynamicArrayFlags, addSize, RArray), uint64_t newSize);
method(void, flush, RArray));
method(byte, sizeToFit, RArray));
// constructor - destructor - reallocation

// add - set - delete
method(RDynamicArrayFlags, addObject, RArray), pointer src);
method(RDynamicArrayFlags, deleteObjectAtIndexIn, RArray), uint64_t index);
method(RDynamicArrayFlags, fastDeleteObjectAtIndexIn, RArray), uint64_t index);
// add - set - delete

// get - find
method(pointer, findObjectWithDelegate, RArray), RFinderDelegate *delegate);
method(RArray *, getSubarray, RArray), uint64_t from, uint64_t count);
method(pointer, elementAtIndex, RArray), uint64_t index);
// get - find

// sort
method(void, bubbleSortWithDelegate, RArray), byte (*comparator)(pointer, pointer));
method(void, quickSortWithDelegate, RArray), uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer));
// standart comparator
byte RDynamicArrayStandartComporator(pointer first, pointer second);
method(void, sort, RArray));
// sort

// work
method(void, shift, RArray), byte side, uint64_t number);
method(static inline byte, checkIfIndexIn, RArray), uint64_t index);
printer(RArray);
// work

//----------------------------------------------------------------------------------

#define makeRArray() $(NULL, c(RArray)), NULL)
#define deleteRA(dynamicArray) $(dynamicArray, d(RArray))); \
                            deallocator(dynamicArray)
#define printRA(dynamicArray) $(dynamicArray, p(RArray)))
#define addObjectToRA(dynamicArray, object) $(dynamicArray, m(addObject, RArray)), object)
#define sortRA(dynamicArray) $(dynamicArray, m(sort, RArray)))
#define elementAtIndexRA(dynamicArray, index) $(dynamicArray, m(elementAtIndex, RArray)), index)
#define sizeToFitRA(dynamicArray) $(dynamicArray, m(sizeToFit, RArray)) )

#endif /*__R_ARRAY_H__*/