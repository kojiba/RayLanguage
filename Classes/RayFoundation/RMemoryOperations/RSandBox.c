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

#include <RSandBox.h>

#ifdef RAY_SAND_BOX_THREAD_SAFE
    #define sandboxMutex &object->mutex
    #define RMutexLockSandbox() RMutexLock(sandboxMutex)
    #define RMutexUnlockSandbox() RMutexUnlock(sandboxMutex)
#else
    #define sandboxMutex
    #define RMutexLockSandbox()
    #define RMutexUnlockSandbox()
#endif

#define storePtrs() pointer (*oldMalloc) (size_t size) = RMallocPtr;\
                    pointer (*oldRealloc)(pointer ptr, size_t oldSize) = RReallocPtr;\
                    pointer (*oldCalloc) (size_t size, size_t blockSize) = RCallocPtr;\
                    void    (*oldFree)   (pointer ptr) = RFreePtr

#define backPtrs()  RMallocPtr = oldMalloc;\
                    RReallocPtr = oldRealloc;\
                    RCallocPtr = oldCalloc;\
                    RFreePtr = oldFree


constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount){
    object = allocator(RSandBox);
    if(object != nil) {
        object->descriptorTable = RAlloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart         = makeRByteArray(sizeOfMemory);

        if(object->memPart != nil && object->descriptorTable != nil) {
            object->classId               = 4;
            object->descriptorsInfo.size  = descriptorsCount;
            object->descriptorsInfo.start = 0;

            object->innerMalloc           = malloc;
            object->innerRealloc          = realloc;
            object->innerCalloc           = calloc;
            object->innerFree             = free;

            object->allocationMode        = RSandBoxAllocationModeRandom;
            object->delegate              = nil;

#ifdef RAY_SAND_BOX_THREAD_SAFE
            object->mutex = mutexWithType(RMutexRecursive);
#endif
        } else {
            RError("RSandBox. Can't allocate descriptors table or memory part.", object);
        }
    }
    return object;
}

destructor(RSandBox) {
    disableSandBox(object);
    if(object->allocationMode == RSandBoxAllocationModeRandom
            || object->allocationMode == RSandBoxAllocationModeDelegated) {
            // totally fresh all to 0
            flushAllToByte(object->memPart->array, object->memPart->size, 0);
            deleter(object->memPart, RByteArray);
            flushAllToByte((byte *) object->descriptorTable, object->descriptorsInfo.size * sizeof(RControlDescriptor), 0);
            deallocator(object->descriptorTable);
            flushAllToByte((byte *) object, sizeof(RSandBox), 0);
            return;
    } else {
        // simple cleanup
        deleter(object->memPart, RByteArray);
        deallocator(object->descriptorTable);
    }
}

printer(RSandBox) {
    // store old malloc
    storePtrs();
    RMutexLockSandbox();
    disableSandBox(object);

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

    RPrintf("\t Descriptors total  - %lu\n", object->descriptorsInfo.size);
    RPrintf("\t Descriptors in use - %lu\n", object->descriptorsInfo.start);
    forAll(iterator, object->descriptorsInfo.start) {
        RPrintf("\t\t [%lu : %lu] (%p)\n",
                object->descriptorTable[iterator].memRange.start,
                object->descriptorTable[iterator].memRange.size,
                (pointer) (object->memPart->array + object->descriptorTable[iterator].memRange.start));
    }
    RPrintLn("}\n");

    RMutexUnlockSandbox();
    // switch to old
    backPtrs();
}

#pragma mark Workings

method(rbool, isRangeFree, RSandBox), RRange range) {
    size_t iterator;
    RMutexLockSandbox();
    storePtrs();
    disableSandBox(object);
    switch (object->allocationMode) {
        case RSandBoxAllocationModeRandom : {
            forAll(iterator, object->descriptorsInfo.start) {
                if(isOverlapping(object->descriptorTable[iterator].memRange, range) == yes) {
                    backPtrs();
                    RMutexUnlockSandbox();
                    return no;
                }
            }
            backPtrs();
            RMutexUnlockSandbox();
            return yes;
        }
        case RSandBoxAllocationModeStandart : {
            // if start smaller than last size + start
            if(range.start > object->descriptorTable[object->descriptorsInfo.start - 1].memRange.size +
                    object->descriptorTable[object->descriptorsInfo.start - 1].memRange.start) {
                backPtrs();
                RMutexUnlockSandbox();
                return yes;
            } else {
                backPtrs();
                RMutexUnlockSandbox();
                return no;
            }
        }
        case RSandBoxAllocationModeDelegated : {
            backPtrs();
            RMutexUnlockSandbox();
            return object->delegate->isRangeFree(object);
        };
    }
    backPtrs();
    RMutexUnlockSandbox();
    return yes;
}

method(void, addFilledRange, RSandBox), RRange range) {
    RMutexLockSandbox();
    object->descriptorTable[object->descriptorsInfo.start].memRange = range;
    ++object->descriptorsInfo.start;
    RMutexUnlockSandbox();
}

method(size_t, rangeForPointer, RSandBox), pointer ptr) {
    ssize_t shift = ptr - (pointer)(object->memPart->array);
    RMutexLockSandbox();
    if(shift < 0
            || shift > object->memPart->size) {
        RErrStr "ERROR. RSandBox. Pointer - %p wasn't allocated with sandBox - %p.\n", ptr, object);
        RMutexUnlockSandbox();
        return object->descriptorsInfo.start;
    } else {
        size_t iterator;
        forAll(iterator, object->descriptorsInfo.start) {
            if(object->descriptorTable[iterator].memRange.start == shift) {
                RMutexUnlockSandbox();
                return iterator;
            }
        }
        RMutexUnlockSandbox();
        return object->descriptorsInfo.start;
    }
}

method(size_t, memoryPlaced, RSandBox)) {
    size_t iterator;
    RMutexLockSandbox();
    switch(object->allocationMode) {
        case RSandBoxAllocationModeStandart : {

            RMutexUnlockSandbox();
            return object->descriptorTable[object->descriptorsInfo.start - 1].memRange.start
                    + object->descriptorTable[object->descriptorsInfo.start - 1].memRange.size;
        }

        case RSandBoxAllocationModeRandom : {
            size_t result = 0;
            forAll(iterator, object->descriptorsInfo.start) {
                result += object->descriptorTable[iterator].memRange.size;
            }
            RMutexUnlockSandbox();
            return result;
        }

        case RSandBoxAllocationModeDelegated : {
            RMutexUnlockSandbox();
            return object->delegate->memoryPlaced(object);
        }
    }
    return 0;
}

#pragma mark Main methods

method(pointer, malloc, RSandBox), size_t sizeInBytes) {
    RMutexLockSandbox();
    storePtrs();
    disableSandBox(object);

    if(object->descriptorsInfo.size == object->descriptorsInfo.start + 1) {
        object->descriptorTable = RReAlloc(object->descriptorTable, object->descriptorsInfo.size * 2 * sizeof(RControlDescriptor));
        if(object->descriptorTable != nil) {
            object->descriptorsInfo.size *= 2;
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

    if(placeToAlloc.start + sizeInBytes > object->memPart->size) {
        RError("RSandBox. Not enought memory.", object);
    } else {
        $(object, m(addFilledRange, RSandBox)), placeToAlloc);
        backPtrs();
        RMutexUnlockSandbox();
        return object->memPart->array + placeToAlloc.start;
    }

    backPtrs();
    RMutexUnlockSandbox();
    return nil;
}

method(pointer, realloc, RSandBox), pointer ptr, size_t newSize) {
    RMutexLockSandbox();
    storePtrs();
    if(ptr == nil) {
        return $(object, m(malloc, RSandBox)),newSize);
    } else {
        size_t iterator = $(object, m(rangeForPointer, RSandBox)), ptr);
        if(iterator != object->descriptorsInfo.start) {
            pointer some = $(object, m(malloc, RSandBox)), newSize);
            if(some != nil) {
                RMemCpy(some, ptr, object->descriptorTable[iterator].memRange.size);
                $(object, m(free, RSandBox)), ptr);
                backPtrs();
                RMutexUnlockSandbox();
                return some;
            }
        }
    }
    backPtrs();
    RMutexUnlockSandbox();
    return nil;
}

method(pointer, calloc, RSandBox), size_t blockCount, size_t blockSize) {
    RMutexLockSandbox();
    storePtrs();
    disableSandBox(object);
    pointer some = RClearAlloc(blockCount, blockSize);
    backPtrs();
    RMutexUnlockSandbox();
    return some;
}

method(void, free, RSandBox), pointer ptr) {
    RMutexLockSandbox();
    storePtrs();
    disableSandBox(object);

    size_t rangeIterator = $(object, m(rangeForPointer, RSandBox)), ptr);
    if (rangeIterator != object->descriptorsInfo.start) {
        if(object->allocationMode == RSandBoxAllocationModeRandom
                || object->allocationMode == RSandBoxAllocationModeDelegated) {
            // totally fresh all to 0
            flushAllToByte(object->memPart->array + object->descriptorTable[rangeIterator].memRange.start, object->descriptorTable[rangeIterator].memRange.size, 0);
        }
        RMemMove(object->descriptorTable + rangeIterator, object->descriptorTable + rangeIterator + 1, (object->descriptorsInfo.size - rangeIterator) * sizeof(RControlDescriptor));
        --object->descriptorsInfo.start;
    } else {
        RErrStr "ERROR. RSandBox. Bad ptr - %p, wasn't allocated with sandBox - %p\n", ptr, object);
    }
    backPtrs();
    RMutexUnlockSandbox();
}


#pragma mark Simple crypt

method(void, XorCrypt, RSandBox), RByteArray *key) {
    Xor(object->memPart->array,  key, object->memPart->size, key->size);           // crypt memory chunk
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                      // crypt memory ptr
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.size * sizeof(RControlDescriptor), key->size); // crypt descriptors table
    Xor(object, key, sizeof(RSandBox), key->size); // crypt pointers
}

method(void, XorDecrypt, RSandBox), RByteArray *key) {
    Xor(object, key, sizeof(RSandBox), key->size); // decrypt pointers
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.size * sizeof(RControlDescriptor), key->size); // decrypt descriptors table
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                      // decrypt memory ptr
    Xor(object->memPart->array,  key, object->memPart->size, key->size);           // decrypt memory chunk
}

#pragma mark Switch

void enableSandBox(RSandBox *sandBox) {
    RMallocPtr  = sandBox->selfMalloc;
    RCallocPtr  = sandBox->selfCalloc;
    RReallocPtr = sandBox->selfRealloc;
    RFreePtr    = sandBox->selfFree;
}

void disableSandBox(RSandBox *sandBox) {
    RMallocPtr  = sandBox->innerMalloc;
    RCallocPtr  = sandBox->innerCalloc;
    RReallocPtr = sandBox->innerRealloc;
    RFreePtr    = sandBox->innerFree;
}

