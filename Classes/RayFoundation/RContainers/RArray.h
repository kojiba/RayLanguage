/**
 * RArray.h
 * Realization of C dynamic array, in Ray additions.
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

#ifndef __R_ARRAY_H__
#define __R_ARRAY_H__

#include <RBasics.h>

typedef enum RArrayFlags {

    // basic errors
                 no_error,
    temp_allocation_error,
       reallocation_error,
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

static const size_t startSizeOfRArrayDefault      = 20;
static const size_t sizeMultiplierOfRArrayDefault = 2;

class(RArray) //---------------------------------------------------------------------\
                                                                                     //
    size_t    startSize;                     // start size of array in elements      //
    size_t    sizeMultiplier;                // size multiplier when auto-add-size   //
    size_t    count;                         // size of elements in array           //
    size_t    freePlaces;                    // size of free places for elements    //
    void    (*destructorDelegate)(pointer);  // destructor of elements delegate      //
    void    (*printerDelegate)   (pointer);  // printer of elements delegate         //
    pointer  *array;                         // array                                //
                                                                                     //
endOf(RArray) //---------------------------------------------------------------------/

// Constructor - Destructor - Printer
constructor (RArray), RArrayFlags *error);
destructor  (RArray);
printer     (RArray);

// Allocation - Reallocation
method(RArrayFlags,        addSize,                   RArray),    size_t newSize);                    // adds some size, store data
method(void,               flush,                     RArray));                                       // destroys all old elements
method(byte,               sizeToFit,                 RArray));

// Add - Set - Delete
method(RArrayFlags,        addObject,                 RArray),    pointer src);                       // push_back analog
method(void,               setObjectAtIndex,          RArray),    pointer newObject, size_t index);   // be aware with this, addObject cause memory leak with this
method(void,               deleteLast,                RArray));                                       // pop_back analog
method(void,               deleteObjects,             RArray),    RRange range);                      // delete with shift
method(RArrayFlags,        deleteObjectAtIndex,       RArray),    size_t index);                      // delete with shift
method(RArrayFlags,        fastDeleteObjectAtIndexIn, RArray),    size_t index);                      // delete, and the last object will be on its place

// Get - Find
method(RFindResult,        findObjectWithDelegate,    RArray),    RCompareDelegate *delegate);        // returns reference
method(RArray *,           getSubarray,               RArray),    RRange range);
method(pointer,            elementAtIndex,            RArray),    size_t index);
method(inline pointer,     lastObject,                RArray));

// Sorts
method(void,               bubbleSortWithDelegate,    RArray),    byte (*comparator)(pointer, pointer));
method(void,               quickSortWithDelegate,     RArray),    size_t first, size_t last, byte (*comparator)(pointer, pointer));
method(void,               sort,                      RArray));

// Work
method(void,               shift,                     RArray),    byte side, RRange range);           // do not call destructor

// Info
method(static inline byte, checkIfIndexIn,            RArray),    size_t index);

// standart comparator
byte RArrayStandartComporator(pointer first, pointer second);

//----------------------------------------------------------------------------------

#define makeRArray()                          $(nil, c(RArray)), nil)
#define printRA(dynamicArray)                 $(dynamicArray, p(RArray)))
#define addObjectToRA(dynamicArray, object)   $(dynamicArray, m(addObject, RArray)), object)
#define sortRA(dynamicArray)                  $(dynamicArray, m(sort, RArray)))
#define elementAtIndexRA(dynamicArray, index) $(dynamicArray, m(elementAtIndex, RArray)), index)
#define sizeToFitRA(dynamicArray)             $(dynamicArray, m(sizeToFit, RArray)) )

#endif /*__R_ARRAY_H__*/