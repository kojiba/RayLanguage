/**
 * RArray.c
 * Realization of C dynamic memory buffer, in Ray additions.
 * May use like array of sized elements.
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

#include "RBuffer.h"
#include "../RClassTable/RClassTable.h"

constructor(RBuffer)) {
    object = allocator(RBuffer);
    if(object != nil) {
        // allocation of buffer
        master(object, RByteArray) = makeRByteArray(startSizeOfRBufferDefault);
        if(master(object, RByteArray) != nil) {

            // allocation of sizes array
            object->sizesArray = RAlloc(sizeof(size_t) * sizeOfObjectsOfRBufferDefault);

            if(object->sizesArray  != nil) {
                object->classId     = registerClassOnce(toString(RBuffer));
                object->freePlaces  = sizeOfObjectsOfRBufferDefault;
                object->count       = 0;
                object->totalPlaced = 0;
            } else {
                RError("RBuffer. Allocation of sizes array failed.", object);

                // cleanup
                deleter(master(object, RByteArray), RByteArray);
            }
        } else {
            RError("RBuffer. Allocation of master RByteArray failed.", object);
        }
    }
    return object;
}

destructor(RBuffer) {
    // kills buffer
    deleter(master(object, RByteArray), RByteArray);
    // kills sizes
    deallocator(object->sizesArray);
}

printer(RBuffer) {
    size_t iterator;
    size_t shift = 0;
    RPrintf("%s object - %p {\n", toString(RBuffer), object);
    RPrintf("\t Total   size : %lu (bytes)\n", master(object, RByteArray)->size);
    RPrintf("\t Placed  size : %lu (bytes)\n", object->totalPlaced);
    RPrintf("\t Free  places : %lu\n", object->freePlaces);
    RPrintf("\t Count objcts : %lu\n", object->count);
    forAll(iterator, object->count) {
        RPrintf("\t\t %lu : size - %lu\n", iterator, object->sizesArray[iterator]);
        printByteArrayInHex(master(object, RByteArray)->array + shift, object->sizesArray[iterator]);
        shift += object->sizesArray[iterator];
        RPrintf("\n");
    }
    RPrintf("} %s object - %p\n", toString(RBuffer), object);
}

#pragma mark Reallocation

method(size_t*, addSizeToSizes, RBuffer), size_t newSize) {
    object->sizesArray = RReAlloc(object->sizesArray, newSize * sizeof(size_t));
    if(object->sizesArray != nil) {
        // add free places
        object->freePlaces = newSize - object->count;
    }
    return object->sizesArray;
}

method(RByteArray*, addSizeToMem, RBuffer), size_t newSize) {
    master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, newSize);
    if(master(object, RByteArray)->array != nil) {
        // set newSize
        master(object, RByteArray)->size = newSize;
    }
    return master(object, RByteArray);
}

method(RBuffer*, flush, RBuffer)) {
    // kills
    $(master(object, RByteArray), d(RByteArray)) );
    deallocator(master(object, RByteArray));
    deallocator(object->sizesArray);

    // new
    master(object, RByteArray) = makeRByteArray(startSizeOfRBufferDefault);
    object->sizesArray = RAlloc(sizeof(size_t) * sizeOfObjectsOfRBufferDefault);
    object->freePlaces  = startSizeOfRBufferDefault;
    object->count       = 0;
    object->totalPlaced = 0;
    return object;
}

method(RBuffer*, sizeToFit, RBuffer)) {
    master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, object->totalPlaced);
    object->sizesArray = RReAlloc(object->sizesArray, object->count * sizeof(size_t));
    object->freePlaces = 0;
    return object;
}

#pragma mark Workers

method(size_t, shiftForPlace, RBuffer), size_t place) {
    size_t iterator;
    size_t shift = 0;
    forAll(iterator, place) {
        shift += object->sizesArray[iterator];
    }
    return shift;
}

method(rbool, checkIndexWithError, RBuffer), size_t index) {
    if(index < object->count) {
        return yes;
    } else {
        RError("RBuffer. Bad index.", object);
        return no;
    }
}

#pragma mark Data operations

method(void, addData, RBuffer), pointer data, size_t sizeInBytes) {

    while(object->freePlaces == 0) {
        // add free to sizes
        $(object, m(addSizeToSizes, RBuffer)), object->count * sizeMultiplierOfRBufferDefault);
    }
    size_t counter = 1;
    while(sizeInBytes > master(object, RByteArray)->size - object->totalPlaced) {
        // add free to buffer
        $(object, m(addSizeToMem, RBuffer)), object->totalPlaced * sizeMultiplierOfRBufferDefault * counter);
        ++counter;
    }

    if(master(object, RByteArray)->array != nil
            && object->sizesArray != nil) {

        // add object
        RMemMove(master(object, RByteArray)->array + object->totalPlaced, data, sizeInBytes);
        object->sizesArray[object->count] = sizeInBytes;

        object->totalPlaced += sizeInBytes;
        ++object->count;
        --object->freePlaces;
    }
}

method(pointer, getDataReference, RBuffer), size_t index) {
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {
        return (master(object, RByteArray)->array + $(object, m(shiftForPlace, RBuffer)), index));
    } else {
        return nil;
    }
}

method(pointer, getDataCopy, RBuffer), size_t index) {
    byte *result = nil;
    pointer *ref = $(object, m(getDataReference, RBuffer)), index);
    if(ref != nil) {
        result = RAlloc(object->sizesArray[index]);
        if (result != nil) {
            RMemMove(result, ref, object->sizesArray[index]);
        } else {
            RError("RBuffer. Bad allocation on getDataCopy.", object);
        }
    }
    return result;
}

method(void, deleteDataAt, RBuffer), size_t index) {
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {
        size_t shift = $(object, m(shiftForPlace, RBuffer)), index);
        size_t size = object->sizesArray[index];

        RMemMove(master(object, RByteArray)->array + shift,
                 master(object, RByteArray)->array + shift + size,
                 object->totalPlaced - size);

        RMemMove(object->sizesArray + index,
                 object->sizesArray + index + 1,
                 object->count - index);

        --object->count;
        ++object->freePlaces;
        object->totalPlaced -= size;
    }
}

#pragma mark Addition to RByteArray

method(RBuffer *, serializeToBuffer, RByteArray), size_t *sizesArray) {
    size_t iterator = 0;

    // search end, compute length
    while(sizesArray[iterator] != 0) {
        ++iterator;
    }

    if(iterator != 0) {
        RBuffer *result = allocator(RBuffer);
        if(result != nil) {
            master(result, RByteArray) = $(object, m(copy, RByteArray)));
            if(master(result, RByteArray) != nil) {
                result->count = iterator;
                result->freePlaces = 0;
                result->classId = registerClassOnce(toString(RBuffer));

                size_t *newSizesArray = RAlloc(sizeof(size_t) * result->count);
                RMemCpy(newSizesArray, sizesArray, result->count * sizeof(size_t));
                result->sizesArray = newSizesArray;
                result->totalPlaced = object->size;
            }
        }
        return result;
    }
    return nil;
}