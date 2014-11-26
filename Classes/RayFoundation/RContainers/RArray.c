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

#include "RArray.h"

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

#pragma mark Constructor - Destructor - Printer

constructor(RArray), RArrayFlags *error) {

    object = malloc(sizeof(RArray));

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA constructor of %p\n", object);
#endif

    if (object == nil) {
        *error = allocation_error;
        RError("RA. Bad allocation on constructor.", object);
        return nil;

    } else {
        // default start size in elements
        object->startSize      = startSizeOfRArrayDefault;
        object->sizeMultiplier = sizeMultiplierOfRArrayDefault;
        object->array          = RAlloc(object->startSize * sizeof(pointer));

        if (object->array == nil) {
            *error = allocation_error;
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
            return object;
        }
    }
}

destructor(RArray) {

    register size_t iterator;

    if (object != nil) {

        if (object->array != nil) {
            forAll(iterator, object->count) {
                // call destructors for all of objects in array
                destroyElementAtIndex(iterator);
            }
            // dealloc array pointer
            deallocator(object->array);
        }

        object->count              = 0;
        object->freePlaces         = 0;
        object->destructorDelegate = nil;
        object->printerDelegate    = nil;
    } else {
        RWarning("RA. Destructing a nil, do nothing, please delete function call, or fix it.", object);
    }

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA destructor of %p\n", object);
#endif
}

printer(RArray) {

#if RAY_SHORT_DEBUG == 1
      RPrintf("%s printer of %p \n", toString(RArray), object);
#else
    register size_t iterator;

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
#endif

}

#pragma mark Allocation - Reallocation

method(RArrayFlags, addSize, RArray), size_t newSize) {

#if RAY_SHORT_DEBUG == 1
        RPrintf("RA %p ADD_SIZE\n", object);
#endif
    if(newSize > object->count) {
#if RAY_SHORT_DEBUG == 1
        RPrintf("\t Old array - %p", object->array);
#endif

        object->array = RReAlloc(object->array, newSize * sizeof(pointer));

#if RAY_SHORT_DEBUG == 1
        RPrintf(", new - %p\n", object->array);
#endif
        if (object->array == nil) {
            return reallocation_error;
        } else {
            object->freePlaces = newSize - object->count; // add some free
            return no_error;
        }
    } else {
        RWarning("RA. Bad new size, do nothing, please delete function call, or fix it.", object);
        return bad_size;
    }
}

method(void, flush, RArray)) {
    register size_t iterator;
    if (object->array != nil) {
        forAll(iterator, object->count) {
            // call destructors for all of objects in array
            destroyElementAtIndex(iterator); // or do nothing
        }
        object->freePlaces += object->count;
        object->count = 0;
    }

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA FLUSH of %p\n", object);
#endif
}

method(byte, sizeToFit, RArray)){
    object->array = RReAlloc(object->array, object->count * sizeof(pointer));

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA %p SIZE_TO_FIT\n", object);
#endif
    if (object->array == nil) {
        return temp_allocation_error;
    } else {
        object->freePlaces = 0;
        return no_error;
    }
}

#pragma mark Add - Set - Delete

method(RArrayFlags, addObject, RArray), pointer src) {
    register RArrayFlags errors;
    errors = no_error;

    // needs additional allocation of memory
    if (object->freePlaces == 0) {
#if RAY_SHORT_DEBUG == 1
        RPrintf("RA %p needs additional allocation\n", object);
#endif
        errors = $(object, m(addSize, RArray)), object->count * object->sizeMultiplier);
    }

    // not need additional allocation
#if RAY_SHORT_DEBUG == 1
    else {
        RPrintf("RA %p addObject without additional allocation\n", object);
    }
#endif

    // if no error on additional allocation
    if (errors == no_error) {
        object->array[object->count] = src;
        incrementCount();
    }

    return errors;
}

method(void, setObjectAtIndex, RArray), pointer newObject, size_t index){
#if RAY_SHORT_DEBUG == 1
        RPrintf("RA %p setObject atIndex = %lu \n", object, index);
#endif
    // if at that index exist some object
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        object->array[index] = newObject;

    } else {

        // if space at index is not allocated
        if(index > (object->freePlaces + object->count)) {
            RWarning("RA. Setting to a not allocated space, do nothing, please delete function call, or fix it.", object);
        // if space is allocated
        } else {
            object->array[index] = newObject;
            // mark: count is not incrementing, cause we don't know place
            // mark: addObject cause memory to leak, with objects added by setObject
            // mark: destructor not called
        }
    }
}

method(RArrayFlags, deleteObjectAtIndex, RArray), size_t index){
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA deleteObjectAtIndex of %p\n", object);
#endif
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        $(object, m(shift, RArray)), shift_left, makeRRange(index, 1));
        return no_error;

    } else {
        return index_does_not_exist;
    }
}

method(RArrayFlags, fastDeleteObjectAtIndexIn, RArray), size_t index){
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA fastDeleteObjectAtIndex of %p\n", object);
#endif
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        if(index != object->count - 1) {
            object->array[index] = object->array[object->count - 1];
        }
        incrementFreePlaces();
        return no_error;

    } else {
        return index_does_not_exist;
    }
}

method(void, deleteObjects, RArray), RRange range){
    register size_t iterator;
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA deleteObjectsInRange of %p, from - %lu, count - %lu \n", object, range.from, range.count);
#endif
    fromStartForAll(iterator, range.from, range.count) {
        destroyElementAtIndex(iterator);
    }
    $(object, m(shift, RArray)), shift_left, range);
}

method(void, deleteLast, RArray)){
    destroyElementAtIndex(object->count - 1);
    incrementFreePlaces();
}

#pragma mark Get - Find

method(RFindResult, findObjectWithDelegate, RArray), RCompareDelegate *delegate) {
    register size_t      iterator;
             RFindResult result;
    result.object = nil;
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA findObjectWithDelegate of %p\n", object);
#endif
    if(delegate != nil) {
        forAll(iterator, object->count) {
            if ($(delegate, m(checkObject, RCompareDelegate)), object->array[iterator]) == equals) {
                result.index  = iterator;
                result.object = object->array[iterator];
                break;
            }
        }
    } else {
        RError("RA. Delegate for searching is nil, please delete function call, or fix it.", object);
    }
    return result;
}

method(pointer, elementAtIndex, RArray), size_t index) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA elementAtIndex of %p\n", object);
#endif
    if($(object, m(checkIfIndexIn,RArray)), index) == index_exists) {
        return object->array[index];
    } else {
        RError("RA. Index not exist!", object);
        return nil;
    }
}

method(RArray *, getSubarray, RArray), RRange range){

    size_t iterator = 0;
    RArray *result = makeRArray();
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA getSubarray of %p\n", object);
#endif
    if(result != nil) {

        // set up subArray delegates:
        result->destructorDelegate = object->destructorDelegate;
        result->printerDelegate    = object->printerDelegate;

        fromStartForAll(iterator, range.from, range.count) {
            if(addObjectToRA(result, elementAtIndexRA(object, iterator)) == no_error) {
                continue;

            // error occurred
            } else {

                // cleanup and alert
                deleter(result, RArray);
                RError("RA. Get subarray error.", object);
                return nil;
            }
        }
    }
#if RAY_SHORT_DEBUG == 1
    else {
        RError("RA. GetSubarray allocation error.\n", object);
    }
#endif

    return result;
}

method(pointer, lastObject, RArray)) {
    return object->array[object->count - 1];
}

#pragma mark Sort

method(void, bubbleSortWithDelegate, RArray), byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA bubbleSortWithDelegate of %p\n", object);
#endif

    register size_t inner;
    register size_t outer;

    forAll(outer, object->count - 1) {
        forAll(inner, object->count - outer - 1) {
            if (comparator(object->array[inner], object->array[inner + 1]) == swap_objects) {
                // swap
                swapElementsAtIndexes(inner, inner + 1);
            }
        }
    }
}

byte RArrayStandartComporator(pointer first, pointer second) {
    // whats-inside-higher, sort
    if (first > second) {
        return swap_objects;
    } else {
        return 0;
    }
}

method(void, quickSortWithDelegate, RArray), size_t first, size_t last, byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    static size_t number = 0;
    RPrintf("RA quickSortWithDelegate of %p recursive #%lu\n", object, number);
    ++number;
#endif

    if (last > first) {
        register pointer pivot = object->array[first];
        register size_t left = first;
        register size_t right = last;
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
}

method(void, sort, RArray)) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA sort of %p\n", object);
#endif
    $(object, m(quickSortWithDelegate, RArray)), 0, object->count, RArrayStandartComporator);
}

#pragma mark Work

method(void, shift, RArray), byte side, RRange range) {
#if RAY_SHORT_DEBUG == 1
    char *sideName;
    if(side == shift_left) {
         sideName = "left";
    } else {
         sideName = "right";
    } RPrintf("RA shift of %p on %s\n", object, sideName);
#endif
    register size_t iterator;
    if(range.count != 0) {
        if (side == shift_left) {
            // do not call destructor
            for(iterator = range.from; iterator < object->count - range.count; ++iterator) {
                object->array[iterator] = object->array[iterator + range.count];
            }
        }
//        fixme
//        else {
//            for(iterator = object->count - range->count; iterator < object->count; ++iterator) {
//                object->array[iterator] = object->array[iterator - object->count + range->count];
//            }
//        }
        object->count -= range.count;
        object->freePlaces += range.count;
    } else {
        RWarning("RA. Shifts of RArray do nothing, please delete function call, or fix it.", object);
    }
}

#pragma mark Info

method(static inline byte, checkIfIndexIn, RArray), size_t index) {
    if (index < object->count) {
        return index_exists;
    } else {
        return index_does_not_exist;
    }
}


