/**
* RDynamicArray.h
* Realization of C dynamic array, in Ray additions.
* Author Kucheruavyu Ilya (kojiba@ro.ru)
*/

#ifndef __R_DYNAMIC_ARRAY_H__
#define __R_DYNAMIC_ARRAY_H__

#include "../RayFoundation.h"

#define RAY_SHORT_DEBUG 451

typedef enum RDynamicArrayErrors {

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


} RDynamicArrayErrors;

$class(RDynamicArray) //------------------------------------------------------------

    uint64_t startSize;             // start size of array in elements, default: 100
    uint64_t sizeMultiplier;        // size multiplier when auto-add-size, default: 2
    uint64_t count;                 // count of elements in array
    uint64_t freePlaces;            // count of free places for elements
    void (*destructor)(pointer);    // destructor of elements delegate
    void (*printer)(pointer);       // printer of elements delegate
    pointer *array;                 // array

$endOfClass(RDynamicArray) //-------------------------------------------------------

// constructor - destructor - reallocation
$constructor(RDynamicArray), RDynamicArrayErrors *error);
$destructor(RDynamicArray);

$method(RDynamicArrayErrors, addSize, RDynamicArray), uint64_t newSize);
$method(void, flush, RDynamicArray));
// constructor - destructor - reallocation

// add - set - delete
$method(RDynamicArrayErrors, addObject, RDynamicArray), pointer src);
$method(RDynamicArrayErrors, deleteObjectAtIndex, RDynamicArray), uint64_t index);
// add - set - delete

// finder
$method(pointer, findObjectWithDelegate, RDynamicArray), byte (*finder)(pointer));

// sorts
$method(void, bubbleSortWithDelegate, RDynamicArray), byte (*comparator)(pointer, pointer));
$method(void, quickSortWithDelegate, RDynamicArray), uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer));
// standart comparator
byte RDynamicArrayStandartComporator(pointer first, pointer second);
$method(void, sort, RDynamicArray));
// sorts

$method(void, shift, RDynamicArray), byte side, uint64_t number);
$method(byte, checkIfIndexIn, RDynamicArray), uint64_t index);

$printer(RDynamicArray);


#endif /*__R_DYNAMIC_ARRAY_H__*/