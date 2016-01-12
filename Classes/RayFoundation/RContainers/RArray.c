/**
 * @file RArray.c
 * @brief Implementation of C dynamic array of pointers.
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


#include "RArray.h"
#include <stdarg.h>

#ifdef RAY_ARRAY_THREAD_SAFE
    #define arrayMutex         (RMutex*)(const RMutex*)(&((const RArray* )object)->mutex) // warning removes
    #define RMutexLockArray()   RMutexLock(arrayMutex)
    #define RMutexUnlockArray() RMutexUnlock(arrayMutex)
#else
    // sets empty
    #define arrayMutex
    #define RMutexLockArray(some)
    #define RMutexUnlockArray(some)
#endif

#define destroyElementAtIndex(index) if (object->destructorDelegate != nil) \
                                        object->destructorDelegate(object->array[index])

#define printElementAtIndex(index) if (object->printerDelegate != nil) \
                                        object->printerDelegate(object->array[iterator])

#define swapElementsAtIndexes(index1, index2) pointer temp = object->array[index1]; \
                                                object->array[index1] = object->array[index2]; \
                                                object->array[index2] = temp

#define incrementCount() ++object->count; \
                       --object->freePlaces

#define incrementFreePlaces() --object->count; \
                              ++object->freePlaces

#define unsafeDeleteObjectAtIndex(index) destroyElementAtIndex(index);\
                                         object->array[index] = object->array[object->count - 1];\
                                         incrementFreePlaces()

#define virtualEnumeratorCall delegate->virtualEnumerator(delegate->context, object->array[iterator], iterator)

#pragma mark More Flexible

RArray* makeRArrayOptions(size_t startSize, size_t multiplier, RArrayFlags *error) {
    RArray *object = allocator(RArray);

    printDebugTrace2("Size %lu, multiplier %lu", startSize, multiplier);
    if (object == nil) {
        if(error != nil) {
            *error = allocation_error;
        }
        RError("RArray. Bad allocation on make.", object);
        return nil;

    } else {
        // default start size in elements
        object->sizeMultiplier = multiplier;
        object->array          = arrayAllocator(pointer, startSize);

        if (object->array == nil) {
            if(error != nil) {
                *error = allocation_error;
            }
            return nil;
        } else {

            // registers its class identifier like 0
            object->classId = 0;
            // set up members
            object->count = 0;
            object->freePlaces = startSize;
            // set up delegates
            object->destructorDelegate = nil;
            object->printerDelegate    = nil;
#ifdef RAY_ARRAY_THREAD_SAFE
            mutexWithType(&object->mutex, RMutexRecursive);
#endif
            return object;
        }
    }
}

#pragma mark Constructor - Destructor - Printer

inline constructor(RArray), RArrayFlags *error) {
    return makeRArrayOptions(startSizeOfRArrayDefault, sizeMultiplierOfRArrayDefault, error);
}

destructor(RArray) {
    RMutexLockArray();
    size_t iterator;
    // call destructors for all of objects in array or do nothing
    if(object->destructorDelegate != nil) {
        forAll(iterator, object->count) {
            object->destructorDelegate(object->array[iterator]);
        }
    }
    // dealloc array pointer
    deallocator(object->array);
    RMutexUnlockArray();
#ifdef RAY_ARRAY_THREAD_SAFE
    RMutexDestroy(arrayMutex);
#endif
    printDebugTrace();
}

printer(RArray) {
    size_t iterator;
    RMutexLockArray();
    RPrintf("%s object %p : { \n", toString(RArray), object);
    RPrintf(" Count : %lu \n", object->count);
    RPrintf(" Free  : %lu \n", object->freePlaces);
    forAll(iterator, object->count) {
        RPrintf("\t %lu - ", iterator);
        printElementAtIndex(iterator); // or print value
        else {
            RPrintf("%p \n", object->array[iterator]);
        }
    }
    RPrintf("} end of %s object %p \n\n", toString(RArray), object);
    RMutexUnlockArray();
}

#pragma mark Allocation - Reallocation

method(RArrayFlags, addSize, RArray), size_t newSize) {
    RMutexLockArray();
    printDebugTrace();
    if(newSize > object->count) {
#ifdef RAY_SHORT_DEBUG
        RPrintf("\t Old array - %p\n", object->array);
#endif
        object->array = RReAlloc(object->array, newSize * sizeof(pointer));
#ifdef RAY_SHORT_DEBUG
        RPrintf("\t New array - %p\n", object->array);
#endif
        if (object->array == nil) {
            RMutexUnlockArray();
            return reallocation_error;
        } else {
            object->freePlaces = newSize - object->count; // add some free
            RMutexUnlockArray();
            return no_error;
        }
    } else {
        RWarning("RArray. Bad new size, do nothing." , object);
        RMutexUnlockArray();
        return bad_size;
    }
}

method(void, flush, RArray)) {
    size_t iterator;
    RMutexLockArray();
    printDebugTrace();
    if (object->array != nil) {
        // call destructors for all of objects in array or do nothing
        if(object->destructorDelegate != nil) {
            forAll(iterator, object->count) {
                object->destructorDelegate(object->array[iterator]);
            }
        }
        object->freePlaces += object->count;
        object->count = 0;
    }
    RMutexUnlockArray();
}

method(byte, sizeToFit, RArray)){
    RMutexLockArray();
    printDebugTrace();
    object->array = RReAlloc(object->array, object->count * sizeof(pointer));
    if (object->array == nil) {
        RMutexUnlockArray();
        return temp_allocation_error;
    } else {
        object->freePlaces = 0;
        RMutexUnlockArray();
        return no_error;
    }
}

#pragma mark Add - Set - Delete

method(RArrayFlags, addObject, RArray), pointer src) {
    RArrayFlags errors;
    RMutexLockArray();
    errors = no_error;
    printDebugTrace();
    // needs additional allocation of memory
    if (object->freePlaces == 0) {
#ifdef RAY_SHORT_DEBUG
        RPrintf("Needs additional allocation\n");
#endif
        errors = $(object, m(addSize, RArray)), object->count * object->sizeMultiplier);
    }

    // if no error on additional allocation
    if (errors == no_error) {
        $(object, m(addObjectUnsafe, RArray)), src);
    }
    RMutexUnlockArray();
    return errors;
}

inline method(void, addObjectUnsafe, RArray), pointer src) {
    RMutexLockArray();
    object->array[object->count] = src;
    incrementCount();
    RMutexUnlockArray();
}

method(void, setObjectAtIndex, RArray), pointer newObject, size_t index){
    RMutexLockArray();
    printDebugTrace1("Index %lu", index);
    // if at that index exist some object
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        object->array[index] = newObject;

    } else {

        // if space is allocated
        if(index < (object->freePlaces + object->count)) {
            object->array[index] = newObject;
            // note: size is not incrementing, cause we don't know place
            // addObject cause memory to leak, with objects added by setObject
            // destructor not called

        // if space at index is not allocated
        } elseWarning(
            RWarning("RArray. Setting to a not allocated space, do nothing." , object);
        );
    }
    RMutexUnlockArray();
}

method(void, deleteObject, RArray), pointer toDelete) {
    if(toDelete != nil
       && object->count != 0) {
        size_t iterator;
        RMutexLockArray();
        printDebugTrace1("Object %p", toDelete);
#ifdef RAY_WARNINGS_ON
        size_t oldFreeCount = object->freePlaces; // store old free count
#endif
        forAll(iterator, object->count) {
            if(object->array[iterator] == toDelete) {
                destroyElementAtIndex(iterator);
                $(object, m(shift, RArray)), yes, makeRRange(iterator, 1));
                break;
            }
        }
        ifWarning(iterator == object->count
                  && oldFreeCount == object->freePlaces,
            RWarning1("RArray. deleteObject. There are no object %p in array.", object, toDelete)
        );

        RMutexUnlockArray();
    } elseWarning(
            RWarning1("RArray. deleteObject. Argument pointer is nil, or array is empty (size = %zu).", object, object->count)
    );
}

method(void, deleteObjectFast, RArray), pointer toDelete) {
    if(toDelete != nil
            || object->count == 0) {
        size_t iterator;
        RMutexLockArray();
        printDebugTrace1("Object %p", toDelete);
#ifdef RAY_WARNINGS_ON
        size_t oldFreeCount = object->freePlaces; // store old free count
#endif
        forAll(iterator, object->count) {
            if(object->array[iterator] == toDelete) {
                unsafeDeleteObjectAtIndex(iterator);
                break;
            }
        }
        ifWarning(iterator == object->count
                  && oldFreeCount == object->freePlaces,
            RWarning1("RArray. deleteObjectFast. There are no object %p in array.", object, toDelete)
        );

        RMutexUnlockArray();
    } elseWarning(
            RWarning1("RArray. deleteObjectFast. Argument pointer is nil, or array is empty (size = %zu).", object, object->count)
    );
}

method(void, deleteLast, RArray)){
    RMutexLockArray();
    printDebugTrace();
    destroyElementAtIndex(object->count - 1);
    incrementFreePlaces();
    RMutexUnlockArray();
}

method(RArrayFlags, deleteObjectAtIndex, RArray), size_t index){
    printDebugTrace1("Index %lu", index);
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        RMutexLockArray();
        destroyElementAtIndex(index);
        $(object, m(shift, RArray)), yes, makeRRange(index, 1));
        RMutexUnlockArray();
        return no_error;

    } else {
        RMutexUnlockArray();
        return index_does_not_exist;
    }
}


method(RArrayFlags, deleteObjectAtIndexFast, RArray), size_t index){
    RMutexLockArray();
    printDebugTrace1("Index %lu", index);
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        unsafeDeleteObjectAtIndex(index);
        RMutexUnlockArray();
        return no_error;

    } else {
        RMutexUnlockArray();
        return index_does_not_exist;
    }
}

method(void, deleteObjects, RArray), RRange range){
    size_t iterator;
    RMutexLockArray();
    printDebugTrace2("Range [%lu : %lu]", range.start, range.size);
    if(object->destructorDelegate != nil) {
        inRange(iterator, range) {
            object->destructorDelegate(object->array[iterator]);
        }
    }
    RMutexUnlockArray();
    $(object, m(shift, RArray)), yes, range);
}

method(void, deleteWithPredicate, RArray), REnumerateDelegate *delegate) {
    size_t iterator;
    printDebugTrace();
    if(delegate->virtualEnumerator != nil) {
        RMutexLockArray();
        if (object->destructorDelegate != nil) {
            for (iterator = 0; iterator < object->count;) {
                if (virtualEnumeratorCall) {
                    object->destructorDelegate(object->array[iterator]);
                    object->array[iterator] = object->array[object->count - 1];
                    incrementFreePlaces();
                    --iterator;
                } else {
                    ++iterator;
                }
            }
        } else {
            for (iterator = 0; iterator < object->count;) {
                if (virtualEnumeratorCall) {
                    object->array[iterator] = object->array[object->count - 1];
                    incrementFreePlaces();
                    --iterator;
                } else {
                    ++iterator;
                }
            }
        }
        RMutexUnlockArray();
    } elseWarning(
            RWarning("RArray. deleteWithPredicate. Delegate virtual function is nil.", object)
    );
}

#pragma mark Get - Find

method(RFindResult, findObjectWithDelegate, RArray), RCompareDelegate *delegate) {
    size_t      iterator;
    RFindResult result;
    result.object = nil;
    result.index = RNotFound;
    printDebugTrace();
    if(delegate != nil
            && delegate->virtualCompareMethod != nil) {
        RMutexLockArray();
        forAll(iterator, object->count) {
            if (delegate->virtualCompareMethod(delegate->etaloneObject, object->array[iterator]) == equals) {
                result.index  = iterator;
                result.object = object->array[iterator];
                break;
            }
        }
        RMutexUnlockArray();
    } elseWarning(
        RWarning("RArray. findObjectWithDelegate. Delegate for searching is nil." , object);
    );
    return result;
}

inline constMethod(pointer, objectAtIndex, RArray), size_t index) {
    printDebugTrace();
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        return object->array[index];
    } elseWarning(
        RWarning("RArray. objectAtIndex. Index not exist!", object);
    );
    return nil;
}

constMethod(size_t, indexOfObject, RArray), pointer toFind) {
    if(toFind != nil
       || object->count == 0) {
        size_t iterator;
        RMutexLockArray();
        printDebugTrace1("Object %p", toFind);
        forAll(iterator, object->count) {
            if(object->array[iterator] == toFind) {
                break;
            }
        }
        RMutexUnlockArray();
        return iterator;
    }
    return RNotFound;
}

method(RArray *, getSubarray, RArray), RRange range) {
    if(range.size + range.start <= object->count
            && range.start < object->count) {
        RMutexLockArray();
        RArray *result = makeRArrayOptions(range.size, object->sizeMultiplier, nil);
        printDebugTrace2("Range [%lu : %lu]", range.start, range.size);
        if (result != nil) {
            size_t iterator;
            // set up subArray delegates
            result->destructorDelegate = object->destructorDelegate;
            result->printerDelegate = object->printerDelegate;
            inRange(iterator, range) {
                $(result, m(addObjectUnsafe, RArray)), object->array[iterator]);
            }
            RMutexUnlockArray();
        }
        return result;
    }
    return nil;
}

method(RArray *, subarrayWithPredicate, RArray), REnumerateDelegate *delegate) {
    size_t  iterator;
    RArray *result = nil;
    printDebugTrace();
    if(delegate != nil
       && delegate->virtualEnumerator != nil) {
        result = makeRArray();
        if(result != nil) {
            result->destructorDelegate = object->destructorDelegate;
            result->printerDelegate    = object->printerDelegate;

            RMutexLockArray();
            forAll(iterator, object->count) {
                if (virtualEnumeratorCall) {
                    $(result, m(addObject, RArray)), object->array[iterator]);
                }
            }
            RMutexUnlockArray();
            $(result, m(sizeToFit, RArray)));
        } elseError(
                RError("RArray. subarrayWithPredicate. Cant allocate result array." , object);
        );
    } elseWarning(
            RWarning("RArray. subarrayWithPredicate. Delegate for searching is nil." , object);
    );
    return result;
}


inline constMethod(pointer, lastObject, RArray)) {
    return object->array[object->count - 1];
}

method(RFindResult, enumerate, RArray), REnumerateDelegate *delegate, rbool isFromLeft){
    size_t iterator;
    RFindResult result;
    result.index  = RNotFound;
    result.object = nil;
    printDebugTrace();
    if(delegate->virtualEnumerator != nil) {
        RMutexLockArray();
        if(isFromLeft) {
            forAll(iterator, object->count) {
                if(!virtualEnumeratorCall) {
                    break;
                }
            }
        } else {
            for(iterator = object->count; iterator != 0; --iterator) {
                if(!delegate->virtualEnumerator(delegate->context, object->array[iterator - 1], iterator - 1)) {
                    break;
                }
            }
        }

        if(iterator != object->count) {
            result.index = iterator;
            result.object = object->array[iterator];
        }
        RMutexUnlockArray();
    } elseWarning(
            RWarning("RArray. enumerate. Delegate virtual function is nil.", object)
    );

    return result;
}

inline
method(RArray *, copy, RArray)) {
    return $(object, m(getSubarray, RArray)), makeRRange(0, object->count));
}

#pragma mark Sort

method(void, bubbleSortWithDelegate, RArray), byte (*comparator)(pointer, pointer)) {
    register size_t inner;
    register size_t outer;
    RMutexLockArray();
    printDebugTrace();

    forAll(outer, object->count - 1) {
        forAll(inner, object->count - outer - 1) {
            if (comparator(object->array[inner], object->array[inner + 1]) == yes) {
                // swap
                swapElementsAtIndexes(inner, inner + 1);
            }
        }
    }
    RMutexUnlockArray();
}

rbool RArrayDefaultComparator(pointer first, pointer second) {
    // pointer sort
    if (first > second) {
        return yes;
    } else {
        return no;
    }
}

method(void, quickSortWithDelegate, RArray), size_t first, size_t last, byte (*comparator)(pointer, pointer)) {
    RMutexLockArray(); // may be locked many times
#ifdef RAY_SHORT_DEBUG
    static size_t number = 0;
    RPrintf("RArray quickSortWithDelegate of %p recursive #%lu\n", object, number);
    ++number;
#endif

    if (last > first) {
        pointer pivot = object->array[first];
        size_t left = first;
        size_t right = last;
        while (left < right) {
            if (comparator(object->array[left], pivot) == no) {
                left += 1;
            } else {
                right -= 1;
                // swap
                swapElementsAtIndexes(left, right);
            }
        }

        left -= 1;

        // swap
        swapElementsAtIndexes(first, left);

        $(object, m(quickSortWithDelegate, RArray)), first, left, comparator);
        $(object, m(quickSortWithDelegate, RArray)), right, last, comparator);
    }
    RMutexUnlockArray();
}

method(void, sort, RArray)) {
    printDebugTrace();
    $(object, m(quickSortWithDelegate, RArray)), 0, object->count, RArrayDefaultComparator);
}

#pragma mark Work

method(void, shift, RArray), rbool isToLeft, RRange range) {
    size_t iterator;
#ifdef RAY_SHORT_DEBUG
    char *sideName;
    if(isToLeft) {
         sideName = "left";
    } else {
         sideName = "right";
    } RPrintf("RArray shift of %p on %s\n", object, sideName);
#endif
    if(range.size != 0) {
        RMutexLockArray();
        if (isToLeft) {
            // do not call destructor
            for(iterator = range.start; iterator < object->count - range.size; ++iterator) {
                object->array[iterator] = object->array[iterator + range.size];
            }
        } else {
            for(iterator = range.size + range.start; iterator != 0; --iterator) {
                object->array[iterator] = object->array[iterator - range.size];
            }
        }
        object->count -= range.size;
        object->freePlaces += range.size;
        RMutexUnlockArray();

    } elseWarning(
        RWarning("RArray. Shifts of RArray do nothing." , object);
    );
}

#pragma mark Info

static inline
constMethod(byte, checkIfIndexIn, RArray), size_t index) {
    if (index < object->count) {
        return index_exists;
    } else {
        return index_does_not_exist;
    }
}

#pragma mark Casts

method(RList *, toRList, RArray)) {
    RList *result = $(nil, c(RList)));
    if(result != nil) {
        size_t iterator;
        RMutexLockArray();
        result->destructorDelegate = object->destructorDelegate;
        result->printerDelegate = object->printerDelegate;
        forAll(iterator, object->count) {
            $(result, m(addHead, RList)), object->array[iterator]);
        }
        RMutexUnlockArray();
    }
    return result;
}

#pragma mark Init from scratch

RArray* initFromArray(pointer pointerToArray, size_t stepToNextPtr, size_t countTotal) {
    size_t  iterator = 0;
    size_t  totalSize = countTotal * stepToNextPtr;
    RArray *result   = makeRArray();

    if(result != nil) {
        while(iterator != totalSize) {
            $(result, m(addObject, RArray)), pointerToArray + iterator);
            iterator += stepToNextPtr;
        }
        $(result, m(sizeToFit, RArray)));
    }
    return result;
}

RArray* initFromArrayWithSizes(pointer pointerToArray, size_t *sizesArray) {
    size_t  iterator   = 0;
    size_t  totalShift = 0;
    RArray *result     = makeRArray();

    if(result != nil) {
        while(sizesArray[iterator] != 0) {
            $(result, m(addObject, RArray)), pointerToArray + totalShift);
            totalShift += sizesArray[iterator];
            ++iterator;
        }
        $(result, m(sizeToFit, RArray)));
    }
    return result;
}

RArray* arrayFromArray(pointer start, ...) {
    va_list args;
    pointer temp;
    RArray *array = makeRArray();

    if(array != nil) {
        $(array, m(addObject, RArray)), start);
        va_start(args, start);

        temp = va_arg(args, pointer);
        while(temp != nil) {
            $(array, m(addObject, RArray)), temp);
            temp = va_arg(args, pointer);
        }
        va_end(args);
        $(array, m(sizeToFit, RArray)));
    }
    return array;
}

#pragma mark Delegates
inline
method(PrinterDelegate, printerDelegate, RArray)) {
    return object->printerDelegate;
}

inline
method(void, setPrinterDelegate, RArray), PrinterDelegate delegate) {
    RMutexLockArray();
    object->printerDelegate = delegate;
    RMutexUnlockArray();
}

inline
method(PrinterDelegate, destructorDelegate, RArray)) {
    return object->destructorDelegate;
}

inline
method(void, setDestructorDelegate, RArray), DestructorDelegate delegate) {
    RMutexLockArray();
    object->destructorDelegate = delegate;
    RMutexUnlockArray();
}