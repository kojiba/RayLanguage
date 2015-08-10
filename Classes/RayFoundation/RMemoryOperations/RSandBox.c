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

#ifdef RAY_SAND_BOX_THREAD_SAFE
    #define sandboxMutex &object->mutex
    #define RMutexLockSandbox() RMutexLock(sandboxMutex)
    #define RMutexUnlockSandbox() RMutexUnlock(sandboxMutex)
#else
    #define sandboxMutex
    #define RMutexLockSandbox()
    #define RMutexUnlockSandbox()
#endif

#define toSandBoxPtrs() setRMalloc (object->innerMalloc);\
                        setRCalloc (object->innerCalloc);\
                        setRRealloc(object->innerRealloc);\
                        setRFree   (object->innerFree)

constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount){
    object = allocator(RSandBox);
    if(object != nil) {
        object->descriptorTable = RAlloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart         = c(RByteArray)(nil, sizeOfMemory);

        if(object->memPart != nil && object->descriptorTable != nil) {
            object->classId               = 4;
            object->descriptorsTotal      = descriptorsCount;
            object->descriptorsCount      = 0;

            object->innerMalloc           = getRMalloc();
            object->innerRealloc          = getRRealloc();
            object->innerCalloc           = getRCalloc();
            object->innerFree             = getRFree();

            object->allocationMode        = RSandBoxAllocationModeStandart;
            object->delegate              = nil;

#ifdef RAY_SAND_BOX_THREAD_SAFE
            mutexWithType(&object->mutex, RMutexNormal);
#endif
        } else {
            RError("RSandBox. Can't allocate descriptors table or memory part.", object);
        }
    }
    return object;
}

destructor(RSandBox) {
    toSandBoxPtrs();
    if(object->allocationMode == RSandBoxAllocationModeRandom
            || object->allocationMode == RSandBoxAllocationModeDelegated) {

            // totally fresh all to 0
            flushAllToByte(object->memPart->array, object->memPart->size, 0);
            deleter(object->memPart, RByteArray);

            flushAllToByte((byte *) object->descriptorTable, object->descriptorsTotal * sizeof(RControlDescriptor), 0);
            deallocator(object->descriptorTable);

            flushAllToByte((byte *) object, sizeof(RSandBox), 0);
            return;

    } else {
        // simple cleanup
        deleter(object->memPart, RByteArray);
        deallocator(object->descriptorTable);
    }
#ifdef RAY_SAND_BOX_THREAD_SAFE
    RMutexDestroy(sandboxMutex);
#endif
}

method(size_t, memoryPlaced, RSandBox)) {
    size_t iterator;
    switch(object->allocationMode) {
        case RSandBoxAllocationModeStandart : {

            return object->descriptorTable[object->descriptorsCount - 1].memRange.start
                   + object->descriptorTable[object->descriptorsCount - 1].memRange.size;
        }

        case RSandBoxAllocationModeRandom : {
            size_t result = 0;
            forAll(iterator, object->descriptorsCount) {
                result += object->descriptorTable[iterator].memRange.size;
            }
            return result;
        }

        case RSandBoxAllocationModeDelegated : {
            return object->delegate->memoryPlaced(object);
        }
    }
    return 0;
}

printer(RSandBox) {
    RMutexLockSandbox();

    size_t iterator;
    RPrintf("%s object - %p {\n", toString(RSandBox), object);
    RPrintf("\t Mem total  - %lu (bytes)\n", object->memPart->size);
    size_t placed = 0;

    if(object->allocationMode == RSandBoxAllocationModeStandart
            || object->allocationMode == RSandBoxAllocationModeRandom) {
        placed = $(object, m(memoryPlaced, RSandBox)));
    }
    if(object->allocationMode == RSandBoxAllocationModeDelegated) {
        placed = object->delegate->memoryPlaced(object);
    }

    RPrintf("\t Mem placed - %lu (bytes)\n", placed);
    RPrintf("\t Mem free   - %lu (bytes)\n", object->memPart->size - placed);

    RPrintf("\t Descriptors total  - %lu\n", object->descriptorsTotal);
    RPrintf("\t Descriptors in use - %lu\n", object->descriptorsCount);
    if(object->descriptorsCount > 0) {
        RPrintf("\t\t Pointer     | Start   | Size \n");
        forAll(iterator, object->descriptorsCount) {
            RPrintf("\t\t %p | %lu | %lu \n",
                    (pointer) (object->memPart->array + object->descriptorTable[iterator].memRange.start),
                    object->descriptorTable[iterator].memRange.start,
                    object->descriptorTable[iterator].memRange.size);
        }
    }
    RPrintLn("}\n");

    RMutexUnlockSandbox();
}

#pragma mark Workings

method(rbool, isRangeFree, RSandBox), RRange range) {
    size_t iterator;
    switch (object->allocationMode) {
        case RSandBoxAllocationModeRandom : {
            forAll(iterator, object->descriptorsCount) {
                if(isOverlapping(object->descriptorTable[iterator].memRange, range) == yes) {
                    return no;
                }
            }
            return yes;
        }
        case RSandBoxAllocationModeStandart : {
            // if start smaller than last size + start
            if(range.start > object->descriptorTable[object->descriptorsCount - 1].memRange.size +
                    object->descriptorTable[object->descriptorsCount - 1].memRange.start) {
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
    object->descriptorTable[object->descriptorsCount].memRange = range;
    ++object->descriptorsCount;
}

method(size_t, rangeForPointer, RSandBox), pointer ptr) {
    size_t shift = ptr - (pointer)(object->memPart->array);
    if(shift > object->memPart->size) {
        RError1("RSandBox. Pointer - %p wasn't allocated with sandBox.", object, ptr);
        return object->descriptorsCount;
    } else {
        size_t iterator;
        forAll(iterator, object->descriptorsCount) {
            if(object->descriptorTable[iterator].memRange.start == shift) {
                return iterator;
            }
        }
        return object->descriptorsCount;
    }
}

#pragma mark Main methods

method(pointer, malloc, RSandBox), size_t sizeInBytes) {
    if(sizeInBytes < object->memPart->size) {
        RMutexLockSandbox();
        storePtrs();
        toSandBoxPtrs();

        if(object->descriptorsTotal == object->descriptorsCount + 1) {
            object->descriptorTable = RReAlloc(object->descriptorTable, object->descriptorsTotal * 2 * sizeof(RControlDescriptor));
            if(object->descriptorTable != nil) {
                object->descriptorsTotal *= 2;
            } else {
                backPtrs();
                RError("RSandBox. Can't reallocate descriptors table.", object);
                RMutexUnlockSandbox();
                return nil;
            }
        }

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
                if(object->descriptorsCount == 0) {
                    placeToAlloc.start = 0;
                // will be next to last
                } else {
                    placeToAlloc.start =
                            object->descriptorTable[object->descriptorsCount - 1].memRange.start
                                    +
                                    object->descriptorTable[object->descriptorsCount - 1].memRange.size;
                }
            } break;

            case RSandBoxAllocationModeDelegated : {
                // based on delegate
                placeToAlloc.start = object->delegate->rangeGenerator(object);
            } break;
        }

        if(placeToAlloc.start + sizeInBytes < object->memPart->size) {
            $(object, m(addFilledRange, RSandBox)), placeToAlloc);
            backPtrs();
            RMutexUnlockSandbox();
            return object->memPart->array + placeToAlloc.start;

        } elseError(
                RError("RSandBox. Not enought memory.", object)
        );

        backPtrs();
        RMutexUnlockSandbox();
    } elseError(
            RError("RSandBox. Size to allocate more than sand box total memory.", object)
    );
    return nil;
}

method(pointer, realloc, RSandBox), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RSandBox)),newSize);
    } else if (newSize == 0) {
        $(object, m(free, RSandBox)), ptr);
        return nil;
    } else {
        pointer some = nil;
        size_t iterator;
        RMutexLockSandbox();
        storePtrs();
        iterator = $(object, m(rangeForPointer, RSandBox)), ptr);
        backPtrs();
        RMutexUnlockSandbox();
        if(iterator != object->descriptorsCount) {
            some = $(object, m(malloc, RSandBox)), newSize);
            if(some != nil) {
                RMemCpy(some, ptr, object->descriptorTable[iterator].memRange.size);
                $(object, m(free, RSandBox)), ptr);
                return some;
            }
        }
    }
    return nil;
}

method(pointer, calloc, RSandBox), size_t blockCount, size_t blockSize) {
    RMutexLockSandbox();
    storePtrs();
    toSandBoxPtrs();
//    fixme
    pointer some = RClearAlloc(blockCount, blockSize);
    backPtrs();
    RMutexUnlockSandbox();
    return some;
}

method(void, free, RSandBox), pointer ptr) {
    if(ptr != nil) {
        RMutexLockSandbox();
        storePtrs();
        toSandBoxPtrs();

        size_t rangeIterator = $(object, m(rangeForPointer, RSandBox)), ptr);
        if (rangeIterator != object->descriptorsCount) {
            if(object->allocationMode == RSandBoxAllocationModeRandom
                    || object->allocationMode == RSandBoxAllocationModeDelegated) {
                // totally fresh all to 0
                flushAllToByte(object->memPart->array + object->descriptorTable[rangeIterator].memRange.start, object->descriptorTable[rangeIterator].memRange.size, 0);
            }
            RMemMove(object->descriptorTable + rangeIterator, object->descriptorTable + rangeIterator + 1, (object->descriptorsTotal - rangeIterator) * sizeof(RControlDescriptor));
            --object->descriptorsCount;

        }

        backPtrs();
        RMutexUnlockSandbox();
    } elseWarning(
            RWarning("RSandBox. Free nil.", object)
    );
}


#pragma mark Simple crypt

method(void, XorCrypt, RSandBox), RByteArray *key) {
    Xor(object->memPart->array,  key, object->memPart->size, key->size);           // crypt memory chunk
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                      // crypt memory ptr
    Xor(object->descriptorTable, key,
            object->descriptorsTotal * sizeof(RControlDescriptor), key->size); // crypt descriptors table
    Xor(object, key, sizeof(RSandBox), key->size); // crypt pointers
}

method(void, XorDecrypt, RSandBox), RByteArray *key) {
    Xor(object, key, sizeof(RSandBox), key->size); // decrypt pointers
    Xor(object->descriptorTable, key,
            object->descriptorsTotal * sizeof(RControlDescriptor), key->size); // decrypt descriptors table
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                      // decrypt memory ptr
    Xor(object->memPart->array,  key, object->memPart->size, key->size);           // decrypt memory chunk
}

#pragma mark Switch

void enableSandBox(RSandBox *sandBox) {
    setRMalloc (sandBox->selfMalloc);
    setRCalloc (sandBox->selfCalloc);
    setRRealloc(sandBox->selfRealloc);
    setRFree   (sandBox->selfFree);
}

void disableSandBox(RSandBox *object) {
    toSandBoxPtrs();
}

