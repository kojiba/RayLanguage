/**
 * @file RArray.c
 * @brief Implementation of C dynamic array, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include <stdio.h>
#include "RArray.h"

#define destroyElementAtIndex(index) if (object->destructorDelegate != NULL) \
                                        object->destructorDelegate(object->array[index])

#define printElementAtIndex(index) if (object->printerDelegate != NULL) \
                                        $(object->array[iterator], object->printerDelegate))

#define swapElementsAtIndexes(index1, index2) pointer temp = object->array[index1]; \
                                                object->array[index1] = object->array[index2]; \
                                                object->array[index2] = temp

#define incrementCount() ++object->count; \
                       --object->freePlaces

#define incrementFreePlaceses() --object->count; \
                              ++object->freePlaces

#pragma mark Constructor - Destructor - Reallocation

constructor(RArray), RArrayFlags *error) {

    object = allocator(RArray);

#if RAY_SHORT_DEBUG == 1
    RPrintf("RDA constructor of %p\n", object);
#endif

    if (object == NULL) {
        *error = allocation_error;
        RPrintf("ERROR. RDA BAD ALLOCATION\n");
        return NULL;

    } else {
        // default start size in elements
        object->startSize = startSizeOfRdynamicArrayDefault;
        object->sizeMultiplier = sizeMultiplierOfRdynamicArrayDefault;
        object->array = RAlloc(object->startSize * sizeof(pointer));

        if (object->array == NULL) {
            *error = allocation_error;
            return NULL;
        } else {

            // registers its class identifier like 0
            object->classId = 0;
            // set up members
            object->count = 0;
            object->freePlaces = object->startSize;
            // set up delegates
            object->destructorDelegate = NULL;
            object->printerDelegate = NULL;
            return object;
        }
    }
}

destructor(RArray) {

    static uint64_t iterator;

    if (object != NULL) {

        if (object->array != NULL) {
            forAll(iterator, object->count) {
                // call destructors for all of objects in array
                destroyElementAtIndex(iterator);
            }
            // dealloc array pointer
            deallocator(object->array);
        }

        object->count = 0;
        object->freePlaces = 0;
        object->destructorDelegate = NULL;
        object->printerDelegate = NULL;
    } else {
        RPrintf("Warning. RA. Destructing a NULL, do nothing, please delete function call, or fix it.\n");
    }

#if RAY_SHORT_DEBUG == 1
    RPrintf("RDA destructor of %p\n", object);
#endif
}

method(RArrayFlags, addSize, RArray), uint64_t newSize) {

#if RAY_SHORT_DEBUG == 1
        RPrintf("RDA %p ADD_SIZE\n", object);
#endif
    if(newSize > object->count) {
        static uint64_t iterator;

        // create temp array
        pointer *tempArray = RAlloc((size_t) (newSize * sizeof(pointer)));

        if (tempArray == NULL) {
            return temp_allocation_error;
        } else {

            // copy pointers to temp array
            forAll(iterator, object->count) {
                tempArray[iterator] = object->array[iterator];
            }
            deallocator(object->array); // delete old
            object->array = tempArray; // switch to new
            object->freePlaces = newSize - object->count; // add some free
            return no_error;
        }
    } else {
        RPrintf("Warning. RA. Bad new size, do nothing, please delete function call, or fix it.\n");
        return bad_size;
    }
}

method(void, flush, RArray)) {
    static uint64_t iterator;

    if (object != NULL) {

        if (object->array != NULL) {
            forAll(iterator, object->count) {
                // call destructors for all of objects in array
                destroyElementAtIndex(iterator); // or do nothing
            }
            // dealloc array pointer
            deallocator(object->array);

            object->array = RAlloc(100 * sizeof(pointer));

            if (object->array == NULL) {
                RPrintf("Warning. Flush allocation error.\n");
                return;
            }

            object->count = 0;
            object->freePlaces = 100;
        }

    } else {
        RPrintf("Warning. Flushing a NULL, do nothing, please delete function call, or fix it.\n");
    }

#if RAY_SHORT_DEBUG == 1
    RPrintf("RDA FLUSH of %p\n", object);
#endif
}

method(byte, sizeToFit, RArray)){
    uint64_t iterator;
    // create temp array
    pointer *tempArray = RAlloc((size_t) (object->count * sizeof(pointer)));

#if RAY_SHORT_DEBUG == 1
    RPrintf("RDA %p SIZE_TO_FIT\n", object);
#endif
    if (tempArray == NULL) {
        return temp_allocation_error;
    } else {

        // copy pointers to temp array
        forAll(iterator, object->count) {
            tempArray[iterator] = object->array[iterator];
        }

        // delete old
        deallocator(object->array);

        // switch to new
        object->array = tempArray;
        object->freePlaces = 0;
        return no_error;
    }
}

#pragma mark Add - Set - Delete

method(RArrayFlags, addObject, RArray), pointer src) {
    static RArrayFlags errors;
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

method(void, setObjectAtIndex, RArray), pointer newObject, uint64_t index){
#if RAY_SHORT_DEBUG == 1
    else {
        RPrintf("RA %p setObject atIndex = %qu \n", object, index);
    }
#endif
    // if at that index exist some object
    if($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        object->array[index] = newObject;

    } else {

        // if space at index is not allocated
        if(index > (object->freePlaces + object->count)){

            RPrintf("Error. RA. Setting to a not allocated space, do nothing, please delete function call, or fix it.\n");
            // allocation of more space to index + startSize
//            fixme
//            $(object, m(addSize, RArray)), index + object->startSize);
//            $(object, m(setObjectAtIndex, RArray)), newObject, index);

        // if space is allocated
        } else {
            object->array[index] = newObject;
            // todo mark: count is not incrementing, cause we don't know place
            // todo mark: addObject cause memory to leak, with objects added by setObject
            // todo mark: destructor not called
//            --object->freePlaces;
        }
    }
}

method(RArrayFlags, deleteObjectAtIndexIn, RArray), uint64_t index) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA deleteObjectAtIndex of %p\n", object);
#endif
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
//  fixme
        return no_error;

    } else {
        return index_does_not_exist;
    }
}

method(RArrayFlags, fastDeleteObjectAtIndexIn, RArray), uint64_t index){
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA fastDeleteObjectAtIndex of %p\n", object);
#endif
    if ($(object, m(checkIfIndexIn, RArray)), index) == index_exists) {
        destroyElementAtIndex(index);
        object->array[index] = object->array[object->count - 1];
        incrementFreePlaceses();
        return no_error;

    } else {
        return index_does_not_exist;
    }
}

#pragma mark Get - Find

method(RArrayFindResult *, findObjectWithDelegate, RArray), RFinderDelegate *delegate) {
    static uint64_t iterator;
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA findObjectWithDelegate of %p\n", object);
#endif
    if(delegate != NULL) {
        forAll(iterator, object->count) {
            if ($(delegate, m(checkObject, RFinderDelegate)), object->array[iterator]) == equals) {
                RArrayFindResult *result = allocator(RArrayFindResult);
                result->index = iterator;
                result->result = object->array[iterator];
                return result;
            }
        }
    } else {
        RPrintf("ERROR. RA - %p. Delegate for searching is NULL, please delete function call, or fix it.\n\n", object);
    }
    return NULL;
}

method(pointer, elementAtIndex, RArray), uint64_t index) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA elementAtIndex of %p\n", object);
#endif
    if($(object, m(checkIfIndexIn,RArray)), index) == index_exists) {
        return object->array[index];
    } else {
        RPrintf("ERROR. RA - %p. Index not_exist!\n", object);
        return NULL;
    }
}

method(RArray *, getSubarray, RArray), uint64_t from, uint64_t count){

    uint64_t iterator = 0;
    RArray *result = makeRArray();
#if RAY_SHORT_DEBUG == 1
    RPrintf("RA getSubarray of %p\n", object);
#endif
    if(result != NULL) {

        // set up subArray delegates:
        result->destructorDelegate = object->destructorDelegate;
        result->printerDelegate = object->printerDelegate;

        fromStartForAll(iterator, from, count) {
            if(addObjectToRA(result, elementAtIndexRA(object, iterator)) == no_error) {
                continue;

            // error occurred
            } else {

                // cleanup and alert
                deleteRA(result);
                RPrintf("ERROR. RA - %p. Get-subarray error.\n", object);
                return NULL;
            }
        }
    }
#if RAY_SHORT_DEBUG == 1
    else {
        RPrintf("ERROR. RA - %p. GetSubarray allocation error.\n", object);
    }
#endif

    return result;
}

#pragma mark Sort

method(void, bubbleSortWithDelegate, RArray), byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    RPrintf("RA bubbleSortWithDelegate of %p\n", object);
#endif

    static uint64_t inner;
    static uint64_t outer;

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
    if (*((size_t *) first) > *((size_t *) second)) {
        return swap_objects;
    } else {
        return 0;
    }
}

method(void, quickSortWithDelegate, RArray), uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    static uint64_t number = 0;
    RPrintf("RA quickSortWithDelegate of %p recursive #%qu\n", object, number);
    ++number;
#endif

    if (last > first) {
        pointer pivot = object->array[first];
        uint64_t left = first;
        uint64_t right = last;
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

printer(RArray) {

#if RAY_SHORT_DEBUG == 1
      RPrintf("%s printer of %p \n", toString(RArray), object);
#else
    uint64_t iterator;

    RPrintf("\n%s object %p: { \n", toString(RArray), object);
    RPrintf(" Count : %qu \n", object->count);
    RPrintf(" Free  : %qu \n", object->freePlaces);
    forAll(iterator, object->count) {
        RPrintf("\t %qu - ", iterator);
        printElementAtIndex(iterator); // or print value
        else {
            RPrintf("%p \n", object->array[iterator]);
        }
    }
    RPrintf("} end of %s object %p \n\n", toString(RArray), object);
#endif

}

method(static inline byte, checkIfIndexIn, RArray), uint64_t index) {
    if (index < object->count) {
        return index_exists;
    } else {
        return index_does_not_exist;
    }
}

method(void, shift, RArray), byte side, uint64_t number) {
#if RAY_SHORT_DEBUG == 1
    char *sideName;
    if(side == shift_left) {
         sideName = "left";
    } else {
         sideName = "right";
    } RPrintf("RA shift of %p on %s\n", object, sideName);
#endif
    uint64_t iterator;
    uint64_t start;
    uint64_t end;
    if(number != 0) {

        if (side == shift_left) {
            start = 0;
            end = object->count;
        } else {
            start = object->count - number - 1;
            end = object->count;
//          fixme
            return;
        }
//          fixme
        // destroying elements, that are rights
        fromStartForAll(iterator, start, end) {
            destroyElementAtIndex(iterator);
        }
//          fixme
        // shifting others
        fromStartForAll(iterator, start, end - 1) {
            swapElementsAtIndexes(iterator, iterator + 1);
        }

    } else {
        RPrintf("Warning. RA. Shifts of RArray do nothing, please delete function call, or fix it.\n");
    }
}
