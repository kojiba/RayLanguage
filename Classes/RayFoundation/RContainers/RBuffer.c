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
            object->sizesArray = RAlloc(sizeof(RRange) * sizeOfObjectsOfRBufferDefault);

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
    RPrintf("%s object - %p {\n", toString(RBuffer), object);
    RPrintf("\t Total   size : %lu (bytes)\n", master(object, RByteArray)->size);
    RPrintf("\t Placed  size : %lu (bytes)\n", object->totalPlaced);
    RPrintf("\t Free  places : %lu\n", object->freePlaces);
    RPrintf("\t Count objcts : %lu\n\n", object->count);
    forAll(iterator, object->count) {
        RPrintf("\t\t %lu : size - %lu\n", iterator, object->sizesArray[iterator].size);
        printByteArrayInHex(master(object, RByteArray)->array + object->sizesArray[iterator].start, object->sizesArray[iterator].size);
        RPrintf("\n");
    }
    RPrintf("} %s object - %p\n", toString(RBuffer), object);
}

#pragma mark Reallocation

method(RRange*, addSizeToSizes, RBuffer), size_t newSize) {
    if(newSize > object->count) {
        object->sizesArray = RReAlloc(object->sizesArray, newSize * sizeof(RRange));
        if (object->sizesArray != nil) {
            // add free places
            object->freePlaces = newSize - object->count;
        }
    } else {
        RError("RBuffer. Bad new size for Sizes", object);
    }
    return object->sizesArray;
}

method(RByteArray*, addSizeToMem, RBuffer), size_t newSize) {
    if(newSize > (object->totalPlaced)) {
        master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, newSize);
        if (master(object, RByteArray)->array != nil) {
            // set newSize
            master(object, RByteArray)->size = newSize;
        }
    } else {
        RError("RBuffer. Bad new size for memory", object);
    }
    return master(object, RByteArray);
}

method(void, flush, RBuffer)) {
    object->freePlaces  += object->count;
    object->count        = 0;
    object->totalPlaced  = 0;
}

method(RBuffer*, sizeToFit, RBuffer)) {
    master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, object->totalPlaced);
    object->sizesArray = RReAlloc(object->sizesArray, object->count * sizeof(size_t));
    object->freePlaces = 0;
    return object;
}

#pragma mark Workers

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
        RMemCpy(master(object, RByteArray)->array + object->totalPlaced, data, sizeInBytes);
        object->sizesArray[object->count].start = object->totalPlaced;
        object->sizesArray[object->count].size = sizeInBytes;

        object->totalPlaced += sizeInBytes;
        ++object->count;
        --object->freePlaces;
    }
}

method(pointer, getDataReference, RBuffer), size_t index) {
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {
        return (master(object, RByteArray)->array + object->sizesArray[index].start);
    } else {
        return nil;
    }
}

method(pointer, getDataCopy, RBuffer), size_t index) {
    byte *result = nil;
    pointer *ref = $(object, m(getDataReference, RBuffer)), index);
    if(ref != nil) {
        result = RAlloc(object->sizesArray[index].size);
        if (result != nil) {
            RMemCpy(result, ref, object->sizesArray[index].size);
        } else {
            RError("RBuffer. Bad allocation on getDataCopy.", object);
        }
    }
    return result;
}

method(void, deleteDataAt, RBuffer), size_t index) {
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {

        RMemMove(master(object, RByteArray)->array + object->sizesArray[index].start,
                 master(object, RByteArray)->array + object->sizesArray[index].start + object->sizesArray[index].size,
                 object->totalPlaced - object->sizesArray[index].size);

        RMemMove(object->sizesArray + index,
                 object->sizesArray + index + 1,
                 object->count - index);

        --object->count;
        ++object->freePlaces;
        object->totalPlaced -= object->sizesArray[index].size;
    }
}

#pragma mark Addition to RByteArray

method(RBuffer *, serializeToBuffer, RByteArray), size_t *sizesArray) {
    size_t iterator = 0;

    // search end 0, compute length
    while(sizesArray[iterator] != 0) {
        ++iterator;
    }

    if(iterator != 0) {
        RBuffer *result = allocator(RBuffer);
        if(result != nil) {
            master(result, RByteArray) = $(object, m(copy, RByteArray)));
            if(master(result, RByteArray) != nil) {
                result->count = iterator;

                RRange *newSizesArray = RAlloc(sizeof(RRange) * result->count);
                if(newSizesArray != nil) {
                    size_t sum = 0;

                    // process size array into RRange array
                    forAll(iterator, result->count) {
                        newSizesArray[iterator].start = sum;
                        newSizesArray[iterator].size = sizesArray[iterator];
                        sum += newSizesArray[iterator].size;
                    }

                    // final operations
                    result->sizesArray = newSizesArray;
                    result->totalPlaced = object->size;
                    result->freePlaces = 0;
                    result->classId = registerClassOnce(toString(RBuffer));
                }
            }
        }
        return result;
    }
    return nil;
}