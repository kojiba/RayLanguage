/**
 * @file RArray.h
 * @brief Realization of C dynamic array of pointers.
 * In Ray additions.
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 9/10/2014
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

#ifndef __R_ARRAY_H__
#define __R_ARRAY_H__

#include "RayFoundation/RBasics/RBasics.h"
#include "RList.h"

#ifdef RAY_ARRAY_THREAD_SAFE
    #include "RayFoundation/RThread/RThread.h"
#endif

struct RArray;

typedef enum RArrayFlags {    ///< basic errors

                 no_error,    ///< no_error flag (operation success)
    temp_allocation_error,    ///< error, allocating temporary storage
       reallocation_error,    ///< error, reallocation memory
         allocation_error,    ///< error, allocating memory

             index_exists,    ///< object at index exist
     index_does_not_exist,    ///< object at index not exist

    /// flag, determines bad newSize
    /// (smaller that exist)
    /// in addSize function
                 bad_size,

} RArrayFlags;

#define startSizeOfRArrayDefault      20    ///< default size of arrays, created with constructor
#define sizeMultiplierOfRArrayDefault 2     ///< default multiplier value of arrays, created with constructor

class(RArray) //---------------------------------------------------------------------

    size_t sizeMultiplier;                    ///< size multiplier (array longs-up on add object, when needed)
    size_t count;                             ///< count of elements in array
    size_t freePlaces;                        ///< count of free places for elements

    DestructorDelegate destructorDelegate;    ///< destructor of elements delegate
    PrinterDelegate    printerDelegate;       ///< printer of elements delegate

    pointer  *array;                          ///< array for store pointers

#ifdef RAY_ARRAY_THREAD_SAFE
    RMutex mutex;    ///< single thread lock (mutex)
#endif

endOf(RArray) //--------------------------------------------------------------------

#pragma mark Flexible constructor

/**
 * Main optioned constructor for RArray instances.
 *
 * @param      startSize     Start size of array, allocates immediately
 * @param      multiplier    Multiplier, used when array longs-up on addObject
 * @param[out] error         Optional error of RArrayFlags, 'allocation_error' when allocator fails
 *
 * @return Created instance of RArray or nil if error occurs.
 */
RArray* makeRArrayOptions(size_t startSize, size_t multiplier, RArrayFlags *error);

#pragma mark Constructor - Destructor - Printer

/**
 * Inline default constructor for RArray instances, uses makeRArrayOptions,
 * with startSizeOfRArrayDefault and sizeMultiplierOfRArrayDefault.
 *
 * @param[out] error    Optional error of RArrayFlags, 'allocation_error' when allocator fails
 *
 * @return Created instance of RArray or nil if error occurs.
 */
extern
constructor (RArray), RArrayFlags *error);

/**
 * Destructor for RArray instances, free inner array,
 * perform destructorDelegate on inner stored pointers.
 */
destructor  (RArray);

/**
 * Printer for RArray instances, print to stdout size and object pointer,
 * perform printerDelegate on inner stored pointers.
 */
printer     (RArray);

#pragma mark Allocation - Reallocation

/**
 * Method used for long-up array, add size to freePlaces.
 * Store pointers to object.
 * If newSize < currentCount show RWarning, and do nothing.
 *
 * @param newSize    New pointer array size
 *
 * @return no_error if ok, bad_size if newSize < currentCount and reallocation_error.
 */
method(RArrayFlags,        addSize,                   RArray),    size_t newSize);                    // adds some size, store data

/**
 * Calls destructors for all of objects in array,
 * or do nothing if destructorDelegate is nil.
 */
method(void,               flush,                     RArray));                                       // destroys all old elements

/**
 * Reallocs pointer array making freePlaces 0.
 *
 * @return temp_allocation_error if fails.
 */
method(byte,               sizeToFit,                 RArray));

#pragma mark Add - Set - Delete

/**
 * Store pointer, calls RArray.addSize if there are no freePlaces.
 * Increment count, decrement freePlaces.
 * Std::vector push_back analog.
 *
 * @param src     Pointer to any instance
 *
 * @return no_error if ok, or addSize flags.
 */
method(RArrayFlags,        addObject,                 RArray),    pointer src);                       // push_back analog

/**
 * Like addObject, but NOT not check freePlaces.
 * Note: not checking size, and not self-longs, but locks mutex.
 *
 * @param src     Pointer to any instance
 */
extern
method(void,               addObjectUnsafe,           RArray),    pointer src);                       // not checking size, and not self-longs, but locks mutex

/**
 * Set objects pointer to index.
 * If at that index exist some object - destroy it.
 * If space is allocated and no object - set ptr.
 * If space is not allocated - RWarning.
 *
 * Note: Not checking size, and not self-longs, but locks mutex.
 *
 * @param newObject    Pointer to any instance
 * @param index        Index to set where
 */
method(void,               setObjectAtIndex,          RArray),    pointer newObject, size_t index);   // be aware with this, addObject cause memory leak with this

/**
 * Deletes pointer from array, if found, and destructs in using destructorDelegate.
 * Shifts array of pointers to left using RArray.shift
 *
 * Note: RWarning if not found.
 *       RWarning if toDelete is nil.
 *
 * @param toDelete    Pointer to delete
 */
method(void,               deleteObject,              RArray),    pointer toDelete);

/**
 * Deletes pointer from array, if found, and destructs in using destructorDelegate.
 * Place on index of destroyed last object in array.
 * Speed up caused by NOT call RArray.shift.
 *
 * Note: RWarning if not found.
 *       RWarning if toDelete is nil.
 *
 * @param toDelete    Pointer to delete
 */
method(void,               deleteObjectFast,          RArray),    pointer toDelete);

/**
 * Destroy last object. Increment free places, decrement count.
 * Std::vector pop_back analog.
 */
method(void,               deleteLast,                RArray));                                       // pop_back analog

/**
 * Destroy object at index and use RArray.shift.
 *
 * @param index Index which delete
 */
method(RArrayFlags,        deleteObjectAtIndex,       RArray),    size_t index);                      // delete with shift

/**
 * Destroy object at index and use RArray.shift.
 * Place on index of destroyed last object in array.
 * Speed up caused by NOT call RArray.shift.
 *
 * @param index    Index which delete
 */
method(RArrayFlags,        deleteObjectAtIndexFast,   RArray),    size_t index);                      // delete, and the last object will be on its place

/**
 * Destroy objects in range and use RArray.shift.
 *
 * @param range    Range of objects to destroy
 */
method(void,               deleteObjects,             RArray),    RRange range);                      // delete with shift

/**
 * Destroy objects checked with delegate.
 * If delegate returns yes -> deletes object like RArray.fastDeleteObject.
 * RWarning if delegate is nil.
 *
 * @param delegate    REnumerateDelegate that checking
 */
method(void,               deleteWithPredicate,       RArray),    REnumerateDelegate *delegate);      // if delegate returns yes -> deletes object like fastDeleteObject

#pragma mark Get - Find

/**
 * Calls virtualCompareMethod of delegate to all objects.
 * RWarning if delegate is nil.
 *
 * @param delegate    RCompareDelegate that checking
 *
 * @return RFindResult with settet object, and place where founded.
 *         If not found, index set to RNotFound, and object is set to nil.
 */
method(RFindResult,        findObjectWithDelegate,    RArray),    RCompareDelegate *delegate);        // returns reference (object != nil, or if not found index == count)

/**
 * If range is valid:
 * creates new RArray and copy pointers in range,
 * also copy delegates.
 *
 * @param range    Range of pointers to copy
 *
 * @return Created instance of RArray or nil.
 */
method(RArray *,           getSubarray,               RArray),    RRange range);                      // delegates copy

/**
 * Creates new RArray and copy pointers if delegate returns yes.
 * also copy delegates.
 * RWarning if delegate is nil.
 *
 * @param delegate    REnumerateDelegate delegate to check pointers.
 *
 * @return Created instance of RArray or nil.
 */
method(RArray *,           subarrayWithPredicate,     RArray),    REnumerateDelegate *delegate);

/**
 * Enumerate all objects uses delegate->virtualEnumerator.
 * Enumerations stops when delegates returns no.
 * RWarning if delegate is nil.
 *
 * @param delegate      REnumerateDelegate delegate to check pointers.
 * @param isFromLeft    Flag used to reverse enumeration
 *
 * @return RFindResult with settet object on which delegate returned no, and place where it was.
 *         If not found, index set to RNotFound, and object is set to nil.
 */
method(RFindResult,        enumerate,                 RArray),    REnumerateDelegate *delegate, rbool isFromLeft);

/**
 * Get pointer at index, or RWarning if index bad.
 *
 * @param index    Index of object to return.
 *
 * @return Pointer to object, or nil if index bad.
 */
extern
constMethod(pointer,       objectAtIndex,             RArray),    size_t index);

/**
 * Get index of object.
 *
 * @param toFind    Pointer to find.
 *
 * @return Index of object, or RNotFound if not found.
 */
extern
constMethod(size_t,        indexOfObject,             RArray),    pointer toFind);

/**
 * NOT locks mutex.
 *
 * @return Pointer of last object.
 */
extern
constMethod(pointer,       lastObject,                RArray));

/**
 * Uses RArray.getSubarray with range [0, count].
 *
 * @return Created RArray with full copy of objects or nil if fail.
 */
extern
     method(RArray *,      copy,                      RArray));

#pragma mark Sorts

/**
 * Preform a bubble sort using comparator.
 *
 * @param comparator    Function to check objects.
 *                      Comparator must return yes to swap.
 */
method(void,               bubbleSortWithDelegate,    RArray),    rbool (*comparator)(pointer, pointer));

/**
 * Preform a quick sort using comparator.
 * One pivot quicksort with first and last index.
 * NOT check bounds.
 *
 * @param comparator    Function to check objects.
 *                      Comparator must return yes to swap.
 */
method(void,               quickSortWithDelegate,     RArray),    size_t first, size_t last, rbool (*comparator)(pointer, pointer));

/**
 * Preform a quick sort in full range with defaultComparator.
 */
method(void,               sort,                      RArray));

#pragma mark Work

/**
 * Shifts pointers array. NOT cycled.
 *
 * @param isToLeft    To left direction flag
 * @param range       Range of cycled objects
 */
method(void,               shift,                     RArray),    rbool isToLeft, RRange range);           // do not call destructor

#pragma mark Info

/**
 * Checks inputed index.
 *
 * @param index    Index to check
 *
 * @returns index_exists or index_does_not_exist.
 */
static inline
constMethod(byte,          checkIfIndexIn,            RArray),    size_t index);

#pragma mark Casts

/**
 * Function to change store struct type.
 *
 * @returns Created RList with copy of pointers of current array, delegates also copied, or nil if creation fails.
 */
method(struct RList *,     toRList,                   RArray));

#pragma mark Standart comparator

/**
 * Function to copmare pointers
 *
 * @returns yes if pointers is equal, and no if not equal
 */
byte RArrayDefaultComparator(pointer first, pointer second);

#pragma mark Delegates

extern
method(PrinterDelegate,   printerDelegate,            RArray));
extern
method(void,              setPrinterDelegate,         RArray), PrinterDelegate delegate);
extern
method(PrinterDelegate,   destructorDelegate,         RArray));
extern
method(void,              setDestructorDelegate,      RArray), DestructorDelegate delegate);

#pragma mark Init from scratch

/**
 * Creates RArray instance from array of pointers with one size.
 *
 * Example:
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
 *
 * @param pointerToArray    Pointer to source array, from will be copied ptrs.
 * @param stepToNextPtr     Offset to get nex ptr from ptr array.
 * @param countTotal        Count of prts will be taken.
 *
 * @return Created RArray instance, and sizeToFit'ed.
*/
RArray* initFromArray(pointer pointerToArray, size_t stepToNextPtr, size_t countTotal); // if size step is fixed, printer and destructor must be set manually

/**
 * Creates RArray instance from array of pointers with different sizes.
 *
 * Example:
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
 *
 * @param pointerToArray    Pointer to source array, from will be copied ptrs.
 * @param sizesArray        Offset's array to next pointer, must be 0-terminated.
 *
 * @return Created RArray instance, and sizeToFit'ed.
*/
RArray* initFromArrayWithSizes(pointer pointerToArray, size_t *sizesArray); // if size not fixed, last in sizeArray must be 0, printer and destructor must be set manually

/**
 * Creates RArray instance from pointers argument list.
 * Last object must be nil.
 *
 * Example:
 *
 * RArray* array = arrayFromArray(
 *         RS("hello"), RS("omg"), RS("lol"), nil
 * );
 * array->printerDelegate = (void (*)(pointer)) p(RString);
 * array->destructorDelegate = RFree;
 * p(RArray)(array);
 * deleter(array, RArray);
 *
 * @return Created RArray instance, and sizeToFit'ed.
*/

RArray* arrayFromArray(pointer firstObject, ...); // array from array of pointers, last must be nil

//----------------------------------------------------------------------------------

#define makeRArray()                          $(nil, c(RArray)), nil)
#define RA                                    arrayFromArray ///

#endif /*__R_ARRAY_H__*/