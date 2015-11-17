/**
 * RArray_Blocks.c
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 9/29/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RArray_Blocks.h"

#ifdef RAY_BLOCKS_ON

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

#define incrementFreePlaces() --object->count; \
                              ++object->freePlaces


#define swapElementsAtIndexes(index1, index2) pointer temp = object->array[index1]; \
                                              object->array[index1] = object->array[index2]; \
                                              object->array[index2] = temp

method(void, deleteWithBlock, RArray), REnumerateBlock enumerator) {
    if(object->count != 0
       && enumerator != nil) {
        size_t iterator;
        RMutexLockArray();
        forAll(iterator, object->count) {
            if(enumerator(object->array[iterator], iterator)) {
                if(object->destructorDelegate != nil) {
                    object->destructorDelegate(object->array[iterator]);
                }
                $(object, m(shift, RArray)), yes, makeRRange(iterator, 1));
                break;
            }
        }
        RMutexUnlockArray();
    } elseWarning(
            RWarning1("RArray. deleteWithBlock. Array is empty (size = %zu) or enumerator nil.", object, object->count)
    );
}

method(void, deleteFastWithBlock, RArray), REnumerateBlock enumerator) {
    size_t iterator;
    printDebugTrace();
    if(enumerator != nil) {
        RMutexLockArray();
        if (object->destructorDelegate != nil) {
            for (iterator = 0; iterator < object->count;) {
                if (enumerator(object->array[iterator], iterator)) {
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
                if (enumerator(object->array[iterator], iterator)) {
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
            RWarning("RArray. deleteWithBlock. Block is nil.", object)
    );
}

method(RArray *, subarrayWithBlock, RArray), REnumerateBlock enumerator) {
    size_t  iterator;
    RArray *result = nil;
    printDebugTrace();
    if(enumerator != nil) {
        result = makeRArray();
        if(result != nil) {
            result->destructorDelegate = object->destructorDelegate;
            result->printerDelegate    = object->printerDelegate;

            RMutexLockArray();
            forAll(iterator, object->count) {
                if (enumerator(object->array[iterator], iterator)) {
                    $(result, m(addObject, RArray)), object->array[iterator]);
                }
            }
            RMutexUnlockArray();
            $(result, m(sizeToFit, RArray)));
        } elseError(
                RError("RArray. subarrayWithBlock. Cant allocate result array." , object);
        );
    } elseWarning(
            RWarning("RArray. subarrayWithBlock. Block for enumerating is nil." , object);
    );
    return result;
}

method(RFindResult, enumerateWithBlock, RArray), REnumerateBlock enumerator) {
    size_t iterator;
    RFindResult result;
    result.index  = object->count;
    result.object = nil;
    printDebugTrace();
    if(enumerator != nil) {
        RMutexLockArray();
        forAll(iterator, object->count) {
            if(!enumerator(object->array[iterator], iterator)) {
                break;
            }
        }

        if(iterator != object->count) {
            result.index = iterator;
            result.object = object->array[iterator];
        }
        RMutexUnlockArray();
    } elseWarning(
            RWarning("RArray. enumerateWithBlock. Enumerator block is nil.", object)
    );

    return result;
}

method(void, quickSortWithBlock, RArray), size_t first, size_t last, byte (^comparator)(pointer, pointer)) {
    RMutexLockArray(); // may be locked many times
#ifdef RAY_SHORT_DEBUG
    static size_t number = 0;
    RPrintf("RArray quickSortWithBlock of %p recursive #%lu\n", object, number);
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

        $(object, m(quickSortWithBlock, RArray)), first, left, comparator);
        $(object, m(quickSortWithBlock, RArray)), right, last, comparator);
    }
    RMutexUnlockArray();
}

#endif
