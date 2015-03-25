/**
 * RArray.c
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

#include <RArray.h>
#include <stdarg.h>

#ifdef RAY_ARRAY_THREAD_SAFE
    #define arrayMutex &object->mutex
    #define RMutexLockArray() RMutexLock(arrayMutex)
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

#pragma mark More Flexible

RArray* makeRArrayOptions(size_t startSize, size_t multiplier, RArrayFlags *error) {
    RArray *object = allocator(RArray);

#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray constructor of %p with size %lu, multiplier %lu \n", object, startSize, multiplier);
#endif

    if (object == nil) {
        if(error != nil) {
            *error = allocation_error;
        }
        RError("RArray. Bad allocation on make.", object);
        return nil;

    } else {
        // default start size in elements
        object->startSize      = startSize;
        object->sizeMultiplier = multiplier;
        object->array          = arrayAllocator(pointer, object->startSize);

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
            object->freePlaces = object->startSize;
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
    object->count              = 0;
    object->freePlaces         = 0;
    object->destructorDelegate = nil;
    object->printerDelegate    = nil;
    RMutexUnlockArray();
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray destructor of %p\n", object);
#endif
}

printer(RArray) {

#ifdef RAY_SHORT_DEBUG
      RPrintf("%s printer of %p \n", toString(RArray), object);
#else
    size_t iterator;
    RMutexLockArray();
    RPrintf("\n%s object %p: { \n", toString(RArray), object);
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
#endif

}

#pragma mark Allocation - Reallocation

method(RArrayFlags, addSize, RArray), size_t newSize) {
    RMutexLockArray();
#ifdef RAY_SHORT_DEBUG
        RPrintf("RArray %p ADD_SIZE\n", object);
#endif
    if(newSize > object->count) {
#ifdef RAY_SHORT_DEBUG
        RPrintf("\t Old array - %p", object->array);
#endif
        object->array = RReAlloc(object->array, newSize * sizeof(pointer));
#ifdef RAY_SHORT_DEBUG
        RPrintf(", new - %p\n", object->array);
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

#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray FLUSH of %p\n", object);
#endif
    RMutexUnlockArray();
}

method(byte, sizeToFit, RArray)){
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray %p SIZE_TO_FIT\n", object);
#endif
    RMutexLockArray();
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

    // needs additional allocation of memory
    if (object->freePlaces == 0) {
#ifdef RAY_SHORT_DEBUG
        RPrintf("RArray %p needs additional allocation\n", object);
#endif
        errors = $(object, m(addSize, RArray)), object->count * object->sizeMultiplier);
    }

    // not need additional allocation
#ifdef RAY_SHORT_DEBUG
    else {
        RPrintf("RArray %p addObject without additional allocation\n", object);
    }
#endif

    // if no error on additional allocation
    if (errors == no_error) {
        $(object, m(addObjectUnsafe, RArray)), src);
    }
    RMutexUnlockArray();
    return errors;
}

method(void, addObjectUnsafe, RArray), pointer src) {
    RMutexLockArray();
    object->array[object->count] = src;
    incrementCount();
    RMutexUnlockArray();
}

method(void, setObjectAtIndex, RArray), pointer newObject, size_t index){
#ifdef RAY_SHORT_DEBUG
        RPrintf("RArray %p setObject atIndex = %lu \n", object, index);
#endif
    RMutexLockArray();
    // if at that index exist some object
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        object->array[index] = newObject;

    } else {

        // if space at index is not allocated
        if(index > (object->freePlaces + object->count)) {
            RWarning("RArray. Setting to a not allocated space, do nothing." , object);
        // if space is allocated
        } else {
            object->array[index] = newObject;
            // note: size is not incrementing, cause we don't know place
            // addObject cause memory to leak, with objects added by setObject
            // destructor not called
        }
    }
    RMutexUnlockArray();
}

method(RArrayFlags, deleteObjectAtIndex, RArray), size_t index){
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray deleteObjectAtIndex of %p\n", object);
#endif
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

method(RArrayFlags, fastDeleteObjectAtIndexIn, RArray), size_t index){
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray fastDeleteObjectAtIndex of %p\n", object);
#endif
    RMutexLockArray();
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        // if not last
        if(index != object->count - 1) {
            object->array[index] = object->array[object->count - 1];
        }
        incrementFreePlaces();
        RMutexUnlockArray();
        return no_error;

    } else {
        RMutexUnlockArray();
        return index_does_not_exist;
    }
}

method(void, deleteObjects, RArray), RRange range){
    size_t iterator;
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray %p delete objects in range [%lu : %lu]\n", object, range.start, range.size);
#endif
    RMutexLockArray();
    if(object->destructorDelegate != nil) {
        inRange(iterator, range) {
            object->destructorDelegate(object->array[iterator]);
        }
    }
    RMutexUnlockArray();
    $(object, m(shift, RArray)), yes, range);
}

method(void, deleteLast, RArray)){
    RMutexLockArray();
    destroyElementAtIndex(object->count - 1);
    incrementFreePlaces();
    RMutexUnlockArray();
}

#pragma mark Get - Find

method(RFindResult, findObjectWithDelegate, RArray), RCompareDelegate *delegate) {
    size_t      iterator;
    RFindResult result;
    result.object = nil;
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray findObjectWithDelegate of %p\n", object);
#endif
    RMutexLockArray();
    result.index = object->count;
    if(delegate != nil) {
        forAll(iterator, object->count) {
            if ($(delegate, m(checkObject, RCompareDelegate)), object->array[iterator]) == equals) {
                result.index  = iterator;
                result.object = object->array[iterator];
                break;
            }
        }
    } else {
        RWarning("RArray. Delegate for searching is nil." , object);
    }
    RMutexUnlockArray();
    return result;
}

inline method(pointer, elementAtIndex, RArray), size_t index) {
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray elementAtIndex of %p\n", object);
#endif
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        return object->array[index];
    } else {
        RWarning("RArray. Index not exist!", object);
        return nil;
    }
}

method(RArray *, getSubarray, RArray), RRange range) {
    if(range.size + range.start <= object->count
            && range.start < object->count) {
        RMutexLockArray();
        RArray *result = makeRArrayOptions(range.size, object->sizeMultiplier, nil);
#ifdef RAY_SHORT_DEBUG
        RPrintf("RArray getSubarray of %p\n", object);
#endif
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

inline method(pointer, lastObject, RArray)) {
    return object->array[object->count - 1];
}

method(RFindResult, enumerate, RArray),    REnumerateDelegate *delegate, rbool isFromLeft){
    size_t iterator;
    RFindResult result;
    result.index  = object->count;
    result.object = nil;
    RMutexLockArray();
    if(isFromLeft) {
        forAll(iterator, object->count) {
            if(!$(delegate, m(checkObject, REnumerateDelegate)), object->array[iterator], iterator)) {
                break;
            }
        }
    } else {
        for(iterator = object->count - 1; iterator != 0; --iterator) {
            if(!$(delegate, m(checkObject, REnumerateDelegate)), object->array[iterator], iterator)) {
                break;
            }
        }
    }
    if(iterator != object->count) {
        result.index = iterator;
        result.object = object->array[iterator];
    }
    RMutexUnlockArray();
    return result;
}

#pragma mark Sort

method(void, bubbleSortWithDelegate, RArray), byte (*comparator)(pointer, pointer)) {
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray bubbleSortWithDelegate of %p\n", object);
#endif
    register size_t inner;
    register size_t outer;
    RMutexLockArray();

    forAll(outer, object->count - 1) {
        forAll(inner, object->count - outer - 1) {
            if (comparator(object->array[inner], object->array[inner + 1]) == swap_objects) {
                // swap
                swapElementsAtIndexes(inner, inner + 1);
            }
        }
    }
    RMutexUnlockArray();
}

byte RArrayStandartComparator(pointer first, pointer second) {
    // pointer sort
    if (first > second) {
        return swap_objects;
    } else {
        return 0;
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
            if (comparator(object->array[left], pivot) != swap_objects) {
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
#ifdef RAY_SHORT_DEBUG
    RPrintf("RArray sort of %p\n", object);
#endif
    $(object, m(quickSortWithDelegate, RArray)), 0, object->count, RArrayStandartComparator);
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
    } else {
        RWarning("RArray. Shifts of RArray do nothing." , object);
    }
}

#pragma mark Info

method(static inline byte, checkIfIndexIn, RArray), size_t index) {
    RMutexLockArray();
    if (index < object->count) {
        RMutexUnlockArray();
        return index_exists;
    } else {
        RMutexUnlockArray();
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
    }
    return array;
}



