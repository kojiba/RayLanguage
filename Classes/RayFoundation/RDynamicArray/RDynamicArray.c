#include <stdio.h>
#include "RDynamicArray.h"

#define startSize 5

byte RDynamicArrayStandartComporator(pointer first, pointer second) {
    // whats inside higher sort
    if (*((size_t *) first) > *((size_t *) second)) {
        return swap_objects;
    } else {
        return 0;
    }
}

$constructor(RDynamicArray), RDynamicArrayErrors *error) {

    object = $allocator(RDynamicArray);

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

        object->array = malloc(startSize * sizeof(pointer));

        if (object->array == NULL) {
            *error = allocation_error;
            return NULL;
        } else {

            // registers its class identifier like 0
            object->classId = 0;

            object->count = 0;
            object->destructor = NULL;
            object->printer = NULL;
            object->freePlaces = startSize;

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
                if (object->destructor != NULL) {
                    object->destructor(object->array[iterator]);
                }
            }
            // dealloc array pointer
            $deallocator(object->array);
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

$method(RDynamicArrayErrors, addSize, RDynamicArray), uint64_t newSize) {

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
        $deallocator(object->array);

        // additional memory allocation
        object->array = malloc((size_t) (newSize * sizeof(pointer)));

        // if allocation not successful
        if (object->array == NULL) {

            // cleanup on temp
            forAll(iterator, object->count) {
                if (object->destructor != NULL) {
                    object->destructor(tempArray[iterator]);
                }
            }
            $deallocator(tempArray);
            return allocation_error;

            // if allocation successful
        } else {

            // copy from temp
            forAll(iterator, object->count) {
                object->array[iterator] = tempArray[iterator];
            }

            // add some free
            object->freePlaces += newSize;

            // delete temp pointer
            $deallocator(tempArray);

            return no_error;
        }
    }
}

$method(RDynamicArrayErrors, addObject, RDynamicArray), pointer src) {
    static RDynamicArrayErrors errors;
    errors = no_error;

    // needs additional allocation of memory
    if (object->freePlaces == 0) {
#if RAY_SHORT_DEBUG == 1
        printf("RDynamicArray %p needs additional allocation\n", object);
#endif
        errors = $(object, m(addSize, RDynamicArray)), object->count * 2);
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
                pointer temp = object->array[inner];
                object->array[inner] = object->array[inner + 1];
                object->array[inner + 1] = temp;
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
        if (object->printer != NULL) {
            $(object->array[iterator], object->printer));
        } else {
            printf("0x%p \n", object->array[iterator]);
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
        uint64_t l = first;
        uint64_t r = last;
        while (l < r) {
            if (comparator(object->array[l], pivot) != swap_objects) {
                l += 1;
            } else {
                r -= 1;
                // swap
                pointer temp = object->array[l];
                object->array[l] = object->array[r];
                object->array[r] = temp;
            }
        }

        l -= 1;

        // swap
        pointer temp = object->array[first];
        object->array[first] = object->array[l];
        object->array[l] = temp;

        quickSortWithDelegateRDynamicArray(object, first, l, comparator);
        quickSortWithDelegateRDynamicArray(object, r, last, comparator);
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
                if (object->destructor != NULL) {
                    object->destructor(object->array[iterator]);
                }
            }
            // dealloc array pointer
            $deallocator(object->array);

            object->array = malloc(100 * sizeof(pointer));

            if (object->array == NULL) {
                return;
            }

            object->count = 0;
            object->freePlaces = 100;
        }

    } else {
        printf("Flusting a NULL pointer");
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

$method(RDynamicArrayErrors, deleteObjectAtIndex, RDynamicArray), uint64_t index) {
    if ($(object, m(checkIfIndexIn, RDynamicArray)), index) == index_exists) {

        if (object->destructor != NULL)
            object->destructor(object->array[index]);

        object->array[index] = NULL;
//  fixme
        return no_error;

    } else {
        return index_does_not_exist;
    }
}
