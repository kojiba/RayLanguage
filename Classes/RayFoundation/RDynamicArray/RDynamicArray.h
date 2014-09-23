/**
 * @file RDynamicArray.h
 * @brief Realization of C dynamic array, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_DYNAMIC_ARRAY_H__
#define __R_DYNAMIC_ARRAY_H__

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

class(RDynamicArray) //------------------------------------------------------------

members
    uint64_t startSize;                     // start size of array in elements
    uint64_t sizeMultiplier;                // size multiplier when auto-add-size
    uint64_t count;                         // count of elements in array
    uint64_t freePlaces;                    // count of free places for elements
    void (*destructorDelegate)(pointer);    // destructor of elements delegate
    void (*printerDelegate)(pointer);       // printer of elements delegate
    pointer *array;                         // array

endOfClass(RDynamicArray) //-------------------------------------------------------

// constructor - destructor - reallocation
constructor(RDynamicArray), RDynamicArrayFlags *error);
destructor(RDynamicArray);

method(RDynamicArrayFlags, addSize, RDynamicArray), uint64_t newSize);
method(void, flush, RDynamicArray));
method(byte, sizeToFit, RDynamicArray));
// constructor - destructor - reallocation

// add - set - delete
method(RDynamicArrayFlags, addObject, RDynamicArray), pointer src);
method(RDynamicArrayFlags, deleteObjectAtIndexIn, RDynamicArray), uint64_t index);
method(RDynamicArrayFlags, fastDeleteObjectAtIndexIn, RDynamicArray), uint64_t index);
// add - set - delete

// get - find
method(pointer, findObjectWithDelegate, RDynamicArray), RFinderDelegate *delegate);
method(RDynamicArray *, getSubarray, RDynamicArray), uint64_t from, uint64_t count);
method(pointer, elementAtIndex, RDynamicArray), uint64_t index);
// get - find

// sort
method(void, bubbleSortWithDelegate, RDynamicArray), byte (*comparator)(pointer, pointer));
method(void, quickSortWithDelegate, RDynamicArray), uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer));
// standart comparator
byte RDynamicArrayStandartComporator(pointer first, pointer second);
method(void, sort, RDynamicArray));
// sort

// work
method(void, shift, RDynamicArray), byte side, uint64_t number);
method(static inline byte, checkIfIndexIn, RDynamicArray), uint64_t index);
printer(RDynamicArray);
// work

//----------------------------------------------------------------------------------

#define makeRDArray() $(NULL, c(RDynamicArray)), NULL)
#define deleteArray(dynamicArray) $(dynamicArray, d(RDynamicArray))); \
                            deallocator(dynamicArray)
#define printArray(dynamicArray) $(dynamicArray, p(RDynamicArray)))
#define addObjectToArray(dynamicArray, object) $(dynamicArray, m(addObject, RDynamicArray)), object)
#define sortArray(dynamicArray) $(dynamicArray, m(sort, RDynamicArray)))
#define elementAtIndex(dynamicArray, index) $(dynamicArray, m(elementAtIndex, RDynamicArray)), index)
#define sizeToFit(dynamicArray) $(dynamicArray, m(sizeToFit, RDynamicArray)) )

#endif /*__R_DYNAMIC_ARRAY_H__*/