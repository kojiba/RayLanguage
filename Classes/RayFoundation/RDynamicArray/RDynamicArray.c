/**
 * @file RDynamicArray.c
 * @brief Implementation of C dynamic array, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include <stdio.h>
#include "RDynamicArray.h"

#define destroyElementAtIndex(index) if (object->destructor != NULL) object->destructor(object->array[index])
#define printElemementAtIndex(index) if (object->printer != NULL) $(object->array[iterator], object->printer))
#define swapElementsAtIndexes(index1, index2) pointer temp = object->array[index1]; \
                                                object->array[index1] = object->array[index2]; \
                                                object->array[index2] = temp;

byte RDynamicArrayStandartComporator(pointer first, pointer second) {
    // whats inside higher sort
    if (*((size_t *) first) > *((size_t *) second)) {
        return swap_objects;
    } else {
        return 0;
    }
}

$constructor(RDynamicArray), RDynamicArrayFlags *error) {

    object = allocator(RDynamicArray);

#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray constructor of %p\n", object);
    if(object == NULL){
        printf("RDynamicArray BAD ALLOCATION\n");
    }
#endif

    if (object == NULL) {
        *error = allocation_error;
        return NULL;

    } else {
        // default start size in elements
        object->startSize = 100;
        object->sizeMultiplier = 2;
        object->array = malloc(object->startSize * sizeof(pointer));

        if (object->array == NULL) {
            *error = allocation_error;
            return NULL;
        } else {

            // registers its class identifier like 0
            object->classId = 0;

            object->count = 0;
            object->destructor = NULL;
            object->printer = NULL;
            object->freePlaces = object->startSize;

            return object;
        }

    }
}

$destructor(RDynamicArray) {

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
        object->destructor = NULL;
        object->printer = NULL;
    }

#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray destructor of %p\n", object);
#endif

}

$method(RDynamicArrayFlags, addSize, RDynamicArray), uint64_t newSize) {

#if RAY_SHORT_DEBUG == 1
        printf("RDynamicArray %p addSize\n", object);
#endif
    static uint64_t iterator;

    // create temp array
    pointer *tempArray = malloc((size_t) (object->count * sizeof(pointer)));

    if (tempArray == NULL) {
        return temp_allocation_error;
    } else {

        // copy pointers to temp array
        forAll(iterator, object->count) {
            tempArray[iterator] = object->array[iterator];
        }

        // delete old
        deallocator(object->array);

        // additional memory allocation
        object->array = malloc((size_t) (newSize * sizeof(pointer)));

        // if allocation not successful
        if (object->array == NULL) {

            // cleanup on temp
            forAll(iterator, object->count) {
                destroyElementAtIndex(iterator);
            }
            deallocator(tempArray);
            return allocation_error;

            // if allocation successful
        } else {

            // copy from temp
            forAll(iterator, object->count) {
                object->array[iterator] = tempArray[iterator];
            }

            // add some free
            object->freePlaces = newSize - object->count;

            // delete temp pointer
            deallocator(tempArray);

            return no_error;
        }
    }
}

$method(RDynamicArrayFlags, addObject, RDynamicArray), pointer src) {
    static RDynamicArrayFlags errors;
    errors = no_error;

    // needs additional allocation of memory
    if (object->freePlaces == 0) {
#if RAY_SHORT_DEBUG == 1
        printf("RDynamicArray %p needs additional allocation\n", object);
#endif
        errors = $(object, m(addSize, RDynamicArray)), object->count * object->sizeMultiplier);
    }

    // not need additional allocation
#if RAY_SHORT_DEBUG == 1
    else {
        printf("RDynamicArray %p addObject without additional allocation\n", object);
    }
#endif

    // if no error on additional allocation
    if (errors == no_error) {
        object->array[object->count] = src;
        ++(object->count);
        --(object->freePlaces);
    }

    return errors;
}

$method(pointer, findObjectWithDelegate, RDynamicArray), byte (*finder)(pointer)) {
    static uint64_t iterator;
#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray findObjectWithDelegate of %p\n", object);
#endif

    forAll(iterator, object->count) {
        if (finder(object->array[iterator]) == object_founded) {
            return object->array[iterator];
        }
    }
    return NULL;
}

$method(void, bubbleSortWithDelegate, RDynamicArray), byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray bubbleSortWithDelegate of %p\n", object);
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

$printer(RDynamicArray) {

#if RAY_SHORT_DEBUG == 1
      printf("%s printer of %p \n", toString(RDynamicArray), object);
#else
    static uint64_t iterator;

    printf("\n%s object %p: { \n", toString(RDynamicArray), object);
    forAll(iterator, object->count) {
        printf("\t %qu - ", iterator);
        printElemementAtIndex(iterator); // or print value
        else {
            printf("%p \n", object->array[iterator]);
        }
    }
    printf("} end of %s object %p \n\n", toString(RDynamicArray), object);
#endif

}

$method(void, quickSortWithDelegate, RDynamicArray), uint64_t first, uint64_t last, byte (*comparator)(pointer, pointer)) {

#if RAY_SHORT_DEBUG == 1
    static uint64_t number = 0;
    printf("RDynamicArray quickSortWithDelegate of %p recursive #%qu\n", object, number);
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

        quickSortWithDelegateRDynamicArray(object, first, left, comparator);
        quickSortWithDelegateRDynamicArray(object, right, last, comparator);
    }
}

$method(void, sort, RDynamicArray)) {
#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray sort of %p\n", object);
#endif
    $(object, m(quickSortWithDelegate, RDynamicArray)), 0, object->count, RDynamicArrayStandartComporator);
}

$method(void, flush, RDynamicArray)) {
    static uint64_t iterator;

    if (object != NULL) {

        if (object->array != NULL) {
            forAll(iterator, object->count) {
                // call destructors for all of objects in array
                destroyElementAtIndex(iterator); // or do nothing
            }
            // dealloc array pointer
            deallocator(object->array);

            object->array = malloc(100 * sizeof(pointer));

            if (object->array == NULL) {
                return;
            }

            object->count = 0;
            object->freePlaces = 100;
        }

    } else {
        printf("Warning. Flushing a NULL, do nothing, please delete function call, or fix it.");
    }

#if RAY_SHORT_DEBUG == 1
    printf("RDynamicArray FLUSH of %p\n", object);
#endif
}

$method(byte, checkIfIndexIn, RDynamicArray), uint64_t index) {
    if (index >= 0 && index < object->count) {
        return index_exists;
    } else {
        return index_does_not_exist;
    }
}

$method(void, shift, RDynamicArray), byte side, uint64_t number) {
#if RAY_SHORT_DEBUG == 1
    char *side;
    if(side == shift_left) {
         side = "left";
    } else {
         side = "right";
    } printf("RDynamicArray shift of %p on \n", object);
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
//            fixme
            return;
        }

        // destroying elements, that are rights
        fromStartForAll(iterator, start, end) {
            destroyElementAtIndex(iterator);
        }

        // shifting others
        fromStartForAll(iterator, start, end - 1) {
            swapElementsAtIndexes(iterator, iterator + 1)
        }

    } else {
        printf("Warning. Shifts of RDynamicArray do nothing, please delete function call, or fix it.");
    }
}

$method(RDynamicArrayFlags, deleteObjectAtIndex, RDynamicArray), uint64_t index) {
    if ($(object, m(checkIfIndexIn, RDynamicArray)), index) == index_exists) {
        destroyElementAtIndex(index);
//  fixme
        return no_error;

    } else {
        return index_does_not_exist;
    }
}
