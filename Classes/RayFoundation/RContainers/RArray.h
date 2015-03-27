/**
 * RArray.h
 * Realization of C dynamic array of pointers
 * In Ray additions.
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
#include <RList.h>

#ifdef RAY_ARRAY_THREAD_SAFE
    #include <RThread.h>
#endif

struct RArray;

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

    // flag, determines bad newSize
    // (smaller that exist)
    // in addSize function
                 bad_size,

} RArrayFlags;

#define startSizeOfRArrayDefault      20
#define sizeMultiplierOfRArrayDefault 2

typedef void (* DestructorDelegate)(pointer);
typedef void (* PrinterDelegate)(pointer);

typedef struct RArray RArray;

class(RArray) //---------------------------------------------------------------------

    size_t sizeMultiplier;                // size multiplier when auto-add-size
    size_t count;                         // count of elements in array
    size_t freePlaces;                    // count of free places for elements

    DestructorDelegate destructorDelegate;   // destructor of elements delegate
    PrinterDelegate    printerDelegate;      // printer of elements delegate

    pointer  *array;                         // array of pointers

#ifdef RAY_ARRAY_THREAD_SAFE
    RMutex mutex;
#endif

endOf(RArray) //--------------------------------------------------------------------

#pragma mark Flexible constructor
RArray* makeRArrayOptions(size_t startSize, size_t multiplier, RArrayFlags *error);

#pragma mark Constructor - Destructor - Printer
extern inline
constructor (RArray), RArrayFlags *error);
destructor  (RArray);
printer     (RArray);

#pragma mark Allocation - Reallocation
method(RArrayFlags,        addSize,                   RArray),    size_t newSize);                    // adds some size, store data
method(void,               flush,                     RArray));                                       // destroys all old elements
method(byte,               sizeToFit,                 RArray));

#pragma mark Add - Set - Delete
method(RArrayFlags,        addObject,                 RArray),    pointer src);                       // push_back analog

extern inline
method(void,               addObjectUnsafe,           RArray),    pointer src);                       // not checking size, and not self-longs, but locks mutex
method(void,               setObjectAtIndex,          RArray),    pointer newObject, size_t index);   // be aware with this, addObject cause memory leak with this

method(void,               deleteLast,                RArray));                                       // pop_back analog
method(void,               deleteObjects,             RArray),    RRange range);                      // delete with shift
method(RArrayFlags,        deleteObjectAtIndex,       RArray),    size_t index);                      // delete with shift
method(RArrayFlags,        fastDeleteObjectAtIndexIn, RArray),    size_t index);                      // delete, and the last object will be on its place

#pragma mark Get - Find
method(RFindResult,        findObjectWithDelegate,    RArray),    RCompareDelegate *delegate);        // returns reference (object != nil, or if not found index == count)
method(RArray *,           getSubarray,               RArray),    RRange range);
method(RFindResult,        enumerate,                 RArray),    REnumerateDelegate *delegate, rbool isFromLeft);

extern inline
method(pointer,            elementAtIndex,            RArray),    size_t index);
extern inline
method(pointer,            lastObject,                RArray));

#pragma mark Sorts
method(void,               bubbleSortWithDelegate,    RArray),    byte (*comparator)(pointer, pointer));
method(void,               quickSortWithDelegate,     RArray),    size_t first, size_t last, byte (*comparator)(pointer, pointer));
method(void,               sort,                      RArray));

#pragma mark Work
method(void,               shift,                     RArray),    rbool isToLeft, RRange range);           // do not call destructor

#pragma mark Info
static inline
method(byte,               checkIfIndexIn,            RArray),    size_t index);

#pragma mark Casts
method(struct RList *,     toRList,      RArray));

#pragma mark Standart comparator
byte RArrayStandartComparator(pointer first, pointer second);

#pragma mark Init from scratch

/*
 * Example
 *
 * void printInt(pointer obj) {
 *    printf("%d\n", *(int*) obj);
 *}
 *
 * void PrintExample() {
 *    int array[10] = {1,2,3,4,5,6,7,8,9,0};
 *    RArray *dynamic = initFromArray(array, sizeof(int), 10);
 *    dynamic->printerDelegate = printInt;
 *    $(dynamic, p(RArray)));
 * }
*/
RArray* initFromArray(pointer pointerToArray, size_t stepToNextPtr, size_t countTotal); // if size step is fixed, printer and destructor must be set manually

/*
 * Example
 *
 * void printInt(pointer obj) {
 *    printf("%d\n", *(int*) obj);
 *}
 *
 * void PrintExample() {
 *    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
 *    size_t sizes[] = {sizeof(int), sizeof(int), sizeof(int), sizeof(int),
 *                      sizeof(int), sizeof(int), sizeof(int), sizeof(int),
 *                      sizeof(int), sizeof(int), 0};
 *    RArray *dynamic = initFromArrayWithSizes(array, sizes);
 *    dynamic->printerDelegate = printInt;
 *    $(dynamic, p(RArray)));
 * }
*/
RArray* initFromArrayWithSizes(pointer pointerToArray, size_t *sizesArray); // if size not fixed, last in sizeArray must be 0, printer and destructor must be set manually

/*
    RArray* array = arrayFromArray(
            RS("hello"), RS("omg"), RS("lol"), nil
    );
    array->printerDelegate = (void (*)(pointer)) p(RCString);
    array->destructorDelegate = RFree;
    p(RArray)(array);
    deleter(array, RArray);
*/

RArray* arrayFromArray(pointer firstObject, ...); // array from array of pointers, last must be nil

//----------------------------------------------------------------------------------

#define makeRArray()                          $(nil, c(RArray)), nil)
#define printRA(dynamicArray)                 $(dynamicArray, p(RArray)))
#define addObjectToRA(dynamicArray, object)   $(dynamicArray, m(addObject, RArray)), object)
#define sortRA(dynamicArray)                  $(dynamicArray, m(sort, RArray)))
#define elementAtIndexRA(dynamicArray, index) $(dynamicArray, m(elementAtIndex, RArray)), index)
#define sizeToFitRA(dynamicArray)             $(dynamicArray, m(sizeToFit, RArray)) )
#define RA arrayFromArray

#endif /*__R_ARRAY_H__*/