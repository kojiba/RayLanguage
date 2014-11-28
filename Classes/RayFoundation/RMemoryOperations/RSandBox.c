/**
 * RSandBox.c
 * Defines continue heap block,
 * with random pointer allocation, deallocation,
 * encryption and management table, etc.
 * May be used like auto-release pool in obj-c.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 30.10.14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RSandBox.h"
#include "../RClassTable/RClassTable.h"

constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount){
    object = allocator(RSandBox);
    if(object != nil) {
        object->descriptorTable = RAlloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart         = makeRByteArray(sizeOfMemory);

        if(object->memPart != nil && object->descriptorTable != nil) {
            object->classId               = registerClassOnce(toString(RSandBox));
            object->descriptorsInfo.size  = descriptorsCount;
            object->descriptorsInfo.start = 0;

            object->innerMalloc           = malloc;
            object->innerRealloc          = realloc;
            object->innerCalloc           = calloc;
            object->innerFree             = free;

            object->allocationMode        = RSandBoxAllocationModeStandart;
            object->delegate              = nil;
        }
    }
    return object;
}

destructor(RSandBox) {
    switchFromSandBox(object);

    $(object->memPart, d(RByteArray)) );
    deallocator(object->memPart);
    deallocator(object->descriptorTable);
}

printer(RSandBox) {
    // store old malloc
    switchToSandBox(object);

    size_t iterator;
    RPrintf("%s object - %p {\n", toString(RSandBox), object);
    RPrintf("\t Mem total  - %lu (bytes)\n", object->memPart->size);
    if(object->allocationMode == RSandBoxAllocationModeStandart
            || object->allocationMode == RSandBoxAllocationModeRandom) {
        RPrintf("\t Mem placed - %lu (bytes)\n", $(object, m(memoryPlaced,RSandBox))));
    }
    RPrintf("\t Descriptors size - %lu\n", object->descriptorsInfo.size);
    RPrintf("\t Descriptors filled - %lu\n", object->descriptorsInfo.start);
    forAll(iterator, object->descriptorsInfo.start) {
        RPrintf("\t\t [%lu : %lu]\n", object->descriptorTable[iterator].memRange.start, object->descriptorTable[iterator].memRange.size);
    }
    RPrintLn("}\n");

    // switch to old
    switchFromSandBox(object);
}

#pragma mark Workings

method(rbool, isRangeFree, RSandBox), RRange range) {
    size_t iterator;
    switch (object->allocationMode) {
        case RSandBoxAllocationModeRandom : {
            forAll(iterator, object->descriptorsInfo.start) {
                if(isOverlapping(object->descriptorTable[iterator].memRange, range) == yes) {
                    return no;
                }
            }
            return yes;
        }
        case RSandBoxAllocationModeStandart : {
            // if start smaller than last size + start
            if(range.start > object->descriptorTable[object->descriptorsInfo.start - 1].memRange.size +
                    object->descriptorTable[object->descriptorsInfo.start - 1].memRange.start) {
                return yes;
            } else {
                return no;
            }
        }
        case RSandBoxAllocationModeDelegated : {
            return object->delegate->isRangeFree(object);
        };
    }

    return yes;
}

method(void, addFilledRange, RSandBox), RRange range) {
    if(object->descriptorsInfo.start != object->descriptorsInfo.size) {
        object->descriptorTable[object->descriptorsInfo.start].memRange = range;
        ++object->descriptorsInfo.start;
    } else {
        RError("RSandBox. Not enought descriptor places.", object);
    }
}

method(size_t, rangeForPointer, RSandBox), pointer ptr) {
    ssize_t shift = ptr - (pointer)(object->memPart->array);
    if(shift < 0
            || shift > object->memPart->size) {
        RError("RSandBox. Pointer wasn't allocated with sandBox.", object);
        return object->descriptorsInfo.start;
    } else {
        size_t iterator;
        forAll(iterator, object->descriptorsInfo.start) {
            if(object->descriptorTable[iterator].memRange.start == shift) {
                return iterator;
            }
        }
        return object->descriptorsInfo.start;
    }
}

method(size_t, memoryPlaced, RSandBox)) {
    size_t iterator;
    switch(object->allocationMode) {
        case RSandBoxAllocationModeStandart : {

            return object->descriptorTable[object->descriptorsInfo.start - 1].memRange.start
                    + object->descriptorTable[object->descriptorsInfo.start - 1].memRange.size;
        }

        case RSandBoxAllocationModeRandom : {
            size_t result = 0;
            forAll(iterator, object->descriptorsInfo.start) {
                result += object->descriptorTable[iterator].memRange.size;
            }
            return result;
        }

        case RSandBoxAllocationModeDelegated : {
            return object->delegate->memoryPlaced(object);
        }
    }
}

#pragma mark Main methods

method(pointer, malloc, RSandBox), size_t sizeInBytes) {
    if(object->descriptorsInfo.size != object->descriptorsInfo.start) {
        // store old malloc
        switchFromSandBox(object);

        RRange placeToAlloc;
        placeToAlloc.size = sizeInBytes;
        switch (object->allocationMode) {

            case RSandBoxAllocationModeRandom : {
                // based on std rand
                placeToAlloc.start = rand() % (object->memPart->size - sizeInBytes);
                while($(object, m(isRangeFree, RSandBox)), placeToAlloc) == no) {
                    placeToAlloc.start = rand() % (object->memPart->size - sizeInBytes);
                }
            } break;

            case RSandBoxAllocationModeStandart : {
                // if first
                if(object->descriptorsInfo.start == 0) {
                    placeToAlloc.start = 0;

                // will be next to last
                } else {
                placeToAlloc.start =
                        object->descriptorTable[object->descriptorsInfo.start - 1].memRange.start
                                +
                        object->descriptorTable[object->descriptorsInfo.start - 1].memRange.size;
                }
            } break;

            case RSandBoxAllocationModeDelegated : {
                // based on delegate
                placeToAlloc.start = object->delegate->rangeGenerator(object);
            } break;
        }

        $(object, m(addFilledRange, RSandBox)), placeToAlloc);

        // switch to old
        switchToSandBox(object);
        if(placeToAlloc.start + sizeInBytes > object->memPart->size) {
            RError("RSandBox. Not enought memory", object);
            return nil;
        } else {
            return object->memPart->array + placeToAlloc.start;
        }
    } else {
        RError("RSandBox. Not enought descriptors", object);
        return nil;
    }
}

method(pointer, realloc, RSandBox), pointer ptr, size_t newSize) {
    size_t iterator = $(object, m(rangeForPointer, RSandBox)), ptr);
    if(iterator != object->descriptorsInfo.start) {
        pointer some = $(object, m(malloc, RSandBox)), newSize);
        if(some != nil) {
            RMemCpy(some, ptr, object->descriptorTable[iterator].memRange.size);
            $(object, m(free, RSandBox)), ptr);
            return some;
        }
    }
    return nil;
}

method(pointer, calloc, RSandBox), size_t sizeInBytes, size_t blockSize) {
    pointer some = $(object, m(malloc, RSandBox)), sizeInBytes * blockSize);
    if(some != nil) {
        flushAllToByte(some, sizeInBytes * blockSize, 0);
    }
    return some;
}

method(void, free, RSandBox), pointer ptr) {
    size_t rangeIterator = $(object, m(rangeForPointer, RSandBox)), ptr);
    if (rangeIterator != object->descriptorsInfo.start) {
        RMemMove(object->descriptorTable + rangeIterator, object->descriptorTable + rangeIterator + 1, sizeof(RControlDescriptor));
        --object->descriptorsInfo.start;
    } else {
        RErrStr "RSandBox. Bad ptr - %p, wasn't allocated with sandBox - &p\n", ptr, object);
    }
}


#pragma mark Simple crypt

method(void, XorCrypt, RSandBox), RByteArray *key) {
    Xor(object->memPart->array,  key, object->memPart->size, key->size);         // crypt memory chunk
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                    // cryptr memory ptr
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.size * sizeof(RControlDescriptor), key->size); // crypt descriptors table
    Xor(object, key, sizeof(RSandBox), key->size); // crypt pointers
}

method(void, XorDecrypt, RSandBox), RByteArray *key) {
    Xor(object, key, sizeof(RSandBox), key->size); // decrypt pointers
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.size * sizeof(RControlDescriptor), key->size); // decrypt descriptors table
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                       // decrypt memory ptr
    Xor(object->memPart->array,  key, object->memPart->size, key->size);            // decrypt memory chunk
}

#pragma mark Switch

void switchToSandBox(RSandBox *sandBox) {
    RMallocPtr  = sandBox->selfMalloc;
    RCallocPtr  = sandBox->selfCalloc;
    RReallocPtr = sandBox->selfRealloc;
    RFreePtr    = sandBox->selfFree;
}

void switchFromSandBox(RSandBox *sandBox) {
    RMallocPtr  = sandBox->innerMalloc;
    RCallocPtr  = sandBox->innerCalloc;
    RReallocPtr = sandBox->innerRealloc;
    RFreePtr    = sandBox->innerFree;
}

