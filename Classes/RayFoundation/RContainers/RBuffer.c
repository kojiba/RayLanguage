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
    object = allocator(object);
    if(object != nil) {
        // allocation of buffer
        master(object, RByteArray) = makeRByteArray(startSizeOfRBufferDefault);
        if(master(object, RByteArray) != nil) {

            // allocation of sizes array
            object->sizesArray = RAlloc(sizeof(size_t) * sizeOfObjectsOfRBufferDefault);

            if(object->sizesArray  != nil) {
                object->classId     = registerClassOnce(toString(RBuffer));
                object->freePlaces  = startSizeOfRBufferDefault;
                object->count       = 0;
                object->totalPlaced = 0;
            } else {
                RError("RBuffer. Allocation of sizes array failed.", object);

                // cleanup
                $(master(object, RByteArray), d(RByteArray)) );
                deallocator(master(object, RByteArray));
            }
        } else {
            RError("RBuffer. Allocation of master RByteArray failed.", object);
        }
    }
    return object;
}

destructor(RBuffer) {
    // kills buffer
    $(master(object, RByteArray), d(RByteArray)) );
    deallocator(master(object, RByteArray));

    // kills sizes
    deallocator(object->sizesArray);
}

printer(RBuffer) {
    size_t iterator;
    size_t shift = 0;
    RPrintf("%s object - %p {\n", toString(RBuffer), object);
    RPrintf("\t Total   size : %qu (bytes)\n", master(object, RByteArray)->size);
    RPrintf("\t Placed  size : %qu (bytes)\n", object->totalPlaced);
    RPrintf("\t Count objcts : %qu\n", object->count);
    RPrintf("\t Free  places : %qu\n", object->freePlaces);
    forAll(iterator, object->count) {
        RPrintf("\t\t %qu :\n", iterator);
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

#pragma mark Data operations

method(void, addData, RBuffer), pointer *data, size_t sizeInBytes) {

    if(object->freePlaces == 0) {
        // add free to sizes
        $(object, m(addSizeToSizes, RBuffer)), object->count * sizeMultiplierOfRBufferDefault);
    }

    if(object->totalPlaced == master(object, RByteArray)->size) {
        // add free to buffer
        $(object, m(addSizeToMem, RBuffer)), object->totalPlaced * sizeMultiplierOfRBufferDefault);
    }

    if(master(object, RByteArray)->array != nil
            && object->sizesArray != nil) {

        // add object
        RMemMove(master(object, RByteArray)->array + object->totalPlaced, data, sizeInBytes);
        object->sizesArray[object->totalPlaced] = sizeInBytes;

        object->totalPlaced += sizeInBytes;
        ++object->count;
        --object->freePlaces;
    }
}

method(pointer, getDataCopy, RBuffer), size_t index) {
    byte *result = nil;
    if(index < object->count) {
        result = RAlloc(object->sizesArray[index]);
        if (result != nil) {
            size_t shift = $(object, m(shiftForPlace, RBuffer)), index);
            RMemMove(result, master(object, RByteArray)->array + shift, object->sizesArray[index]);
        }
    } else {
        RError("RBuffer. Bad index.", object);
    }
    return result;
}