/**
 * @file RArray.h
 * @brief Realization of C dynamic array, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __R_ARRAY_H__
#define __R_ARRAY_H__

#include "../RayFoundation.h"
#include "RCompareDelegate/RCompareDelegate.h"
#include "../RCString/RCString.h"

typedef struct RArrayFindResult {
    pointer result;
    uint64_t index;
} RArrayFindResult;

typedef enum RArrayFlags {

    // basic errors
                 no_error,
    temp_allocation_error,
         allocation_error,

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

} RArrayFlags;

static const uint64_t startSizeOfRArrayDefault      = 100;
static const uint64_t sizeMultiplierOfRArrayDefault = 2;

class(RArray) //------------------------------------------------------------

members
    uint64_t  startSize;                     // start size of array in elements
    uint64_t  sizeMultiplier;                // size multiplier when auto-add-size
    uint64_t  count;                         // count of elements in array
    uint64_t  freePlaces;                    // count of free places for elements
    void    (*destructorDelegate)(pointer);  // destructor of elements delegate
    void    (*printerDelegate)(pointer);     // printer of elements delegate
    pointer  *array;                         // array

endOf(RArray) //------------------------------------------------------------

// constructor - destructor
constructor(RArray), RArrayFlags *error);
destructor(RArray);
// allocation - reallocation
method(RArrayFlags,        addSize, RArray),                    uint64_t newSize);
method(void,               flush, RArray));
method(byte,               sizeToFit, RArray));
// add - set - delete
method(RArrayFlags,        addObject, RArray),                  pointer src);                        // push_back analog
method(void,               deleteLast, RArray));                                                     // pop_back analog
method(void,               setObjectAtIndex, RArray),           pointer newObject, uint64_t index);  // be aware with this, addObject cause memory leak with that
method(RArrayFlags,        deleteObjectAtIndex, RArray),        uint64_t index);
method(RArrayFlags,        fastDeleteObjectAtIndexIn, RArray),  uint64_t index);
// get - find
method(RArrayFindResult *, findObjectWithDelegate, RArray),     RCompareDelegate *delegate);
method(RArray *,           getSubarray, RArray),                RRange *range);
method(pointer,            elementAtIndex, RArray),             uint64_t index);
// sort
method(void,               bubbleSortWithDelegate, RArray),     byte (*comparator)(pointer, pointer));
method(void,               quickSortWithDelegate, RArray),      uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer));
method(void,               sort, RArray));
// work
method(void,               shift, RArray),                      byte side, RRange *range);         // do not call destructor
method(static inline byte, checkIfIndexIn, RArray),             uint64_t index);

printer(RArray);
// standart comparator
byte RArrayStandartComporator(pointer first, pointer second);

//----------------------------------------------------------------------------------

#define makeRArray()                          $(NULL, c(RArray)), NULL)
#define deleteRA(dynamicArray)                $(dynamicArray, d(RArray))); \
                                              deallocator(dynamicArray)
#define printRA(dynamicArray)                 $(dynamicArray, p(RArray)))
#define addObjectToRA(dynamicArray, object)   $(dynamicArray, m(addObject, RArray)), object)
#define sortRA(dynamicArray)                  $(dynamicArray, m(sort, RArray)))
#define elementAtIndexRA(dynamicArray, index) $(dynamicArray, m(elementAtIndex, RArray)), index)
#define sizeToFitRA(dynamicArray)             $(dynamicArray, m(sizeToFit, RArray)) )

#endif /*__R_ARRAY_H__*/