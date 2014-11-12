#include "RSandBox.h"
#include "../RClassTable/RClassTable.h"

#define storeOldPtrs() pointer (*oldMalloc)(size_t) = RMallocPtr;  void (*oldFree)(void* pointer) = RFreePtr; RMallocPtr = object->innerMallocPtr; RFreePtr = object->innerFreePtr;
#define revertPtrs()   RMallocPtr = oldMalloc; RFreePtr = oldFree;

void emptyFree(pointer ptr) {
    RPrintf("\tFree for %p\n", ptr);
    return;
}

constructor(RSandBox), size_t sizeOfMemory, size_t descriptorsCount, pointer (*innerMallocPtr)(size_t), void (*innerFreePtr)(pointer)) {
    // store old malloc
    pointer (*oldMalloc)(size_t) = RMallocPtr;
    // switch to inner malloc
    RMallocPtr = innerMallocPtr;

    object = RAlloc(sizeof(RSandBox));
    if(object != nil) {
        object->classId               = registerClassOnce(toString(RSandBox));
        object->descriptorTable       = RAlloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart               = makeRByteArray(sizeOfMemory);
        object->descriptorsInfo.count = descriptorsCount;
        object->descriptorsInfo.from  = 0;
        object->innerMallocPtr        = innerMallocPtr;
        object->innerFreePtr          = innerFreePtr;
        object->allocationMode        = RSandBoxAllocationModeRandom;
        object->rangeGenerator        = nil;
    }

    // switch to old
    RMallocPtr = oldMalloc;
    return object;
}

destructor(RSandBox) {
    // higher lvl free
    RFreePtr = object->innerFreePtr;

    $(object->memPart, d(RByteArray)) );
    deallocator(object->memPart);
    deallocator(object->descriptorTable);
}

printer(RSandBox) {
    // store old malloc
    storeOldPtrs();

    size_t iterator;
    RPrintf("%s object - %p {\n", toString(RSandBox), object);
    RPrintf("\t Mem total  - %lu (bytes)\n", object->memPart->size);
    if(object->allocationMode == RSandBoxAllocationModeStandart
            || object->allocationMode == RSandBoxAllocationModeRandom) {
        RPrintf("\t Mem placed - %lu (bytes)\n", $(object, m(memoryPlaced,RSandBox))));
    }
    RPrintf("\t Descriptors count - %lu\n", object->descriptorsInfo.count);
    RPrintf("\t Descriptors filled - %lu\n", object->descriptorsInfo.from);
    forAll(iterator, object->descriptorsInfo.from) {
        RPrintf("\t\t [%lu : %lu]\n", object->descriptorTable[iterator].memRange.from, object->descriptorTable[iterator].memRange.count);
    }
    RPrintLn("}\n");

    // switch to old
    revertPtrs();
}

singleton(RSandBox) {
    static RSandBox *instance = nil;
    if(instance == nil) {
        $(instance, c(RSandBox)), 16348, 128, RTrueMalloc, RTrueFree);
    }
    return instance;
}

#pragma mark Workings

method(rbool, isRangeFree, RSandBox), RRange range) {
    size_t iterator;
    forAll(iterator, object->descriptorsInfo.from) {
        if(isOverlapping(object->descriptorTable[iterator].memRange, range) == yes) {
            return no;
        }
    }
    return yes;
}

method(void, addFilledRange, RSandBox), RRange range) {
    if(object->descriptorsInfo.from != object->descriptorsInfo.count) {
        object->descriptorTable[object->descriptorsInfo.from].memRange = range;
        ++object->descriptorsInfo.from;
    } else {
        RError("RSB. Not enought descriptor places.", object);
    }
}

method(size_t, sizeForPointer, RSandBox), pointer ptr) {
    ssize_t shift = ptr - (pointer)(object->memPart->array);
    if(shift < 0) {
        RError("Pointer wasn't allocated with sandBox.", object);
        return 0;
    } else {
        size_t iterator;
        forAll(iterator, object->descriptorsInfo.from) {
            if(object->descriptorTable[iterator].memRange.from == shift) {
                return object->descriptorTable[iterator].memRange.count;
            }
        }
        return 0;
    }
}

method(size_t, memoryPlaced, RSandBox)) {
    size_t iterator;
    switch(object->allocationMode) {
        case RSandBoxAllocationModeStandart : {

            return object->descriptorTable[object->descriptorsInfo.from - 1].memRange.from
                    + object->descriptorTable[object->descriptorsInfo.from - 1].memRange.count;
        }

        case RSandBoxAllocationModeRandom : {
            size_t result = 0;
            forAll(iterator, object->descriptorsInfo.from) {
                result +=object->descriptorTable[iterator].memRange.count;
            }
            return result;
        }

        default:
            RError("Bad case for memPlaced. Only ModeStandart and ModeRandom\n", object);
            return 0;
    }
}

#pragma mark Main methods

method(pointer, malloc, RSandBox), size_t sizeInBytes) {
    if(object->descriptorsInfo.count != object->descriptorsInfo.from) {
        // store old malloc
        storeOldPtrs();

        RRange placeToAlloc;
        placeToAlloc.count = sizeInBytes;
        switch (object->allocationMode) {

            case RSandBoxAllocationModeRandom : {
                // based on std rand
                placeToAlloc.from = rand() % (object->memPart->size - sizeInBytes);
                while($(object, m(isRangeFree,RSandBox)), placeToAlloc) == no) {
                    placeToAlloc.from = rand() % (object->memPart->size - sizeInBytes);
                }
            } break;

            case RSandBoxAllocationModeStandart : {
                // if first
                if(object->descriptorsInfo.from == 0) {
                    placeToAlloc.from = 0;

                // will be next to last
                } else {
                placeToAlloc.from =
                        object->descriptorTable[object->descriptorsInfo.from - 1].memRange.from
                                +
                        object->descriptorTable[object->descriptorsInfo.from - 1].memRange.count;
                }
            } break;

            case RSandBoxAllocationModeDelegated : {
                // based on delegate
                placeToAlloc.from = object->rangeGenerator(object);
            } break;
        }

        $(object, m(addFilledRange, RSandBox)), placeToAlloc);

        // switch to old
        revertPtrs();
        if(placeToAlloc.from + sizeInBytes > object->memPart->size) {
            RError("RSB. Not enought memory", object);
            return nil;
        } else {
            return object->memPart->array + placeToAlloc.from;
        }
    } else {
        RError("RSB. Not enought descriptors", object);
        return nil;
    }
}

#pragma mark Simple crypt

method(void, XorCrypt, RSandBox), RByteArray *key) {
    Xor(object->memPart->array,  key, object->memPart->size, key->size);         // crypt memory chunk
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                    // cryptr memory ptr
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.count * sizeof(RControlDescriptor), key->size); // crypt descriptors table
    Xor(object, key, sizeof(RSandBox), key->size); // crypt pointers
}

method(void, XorDecrypt, RSandBox), RByteArray *key) {
    Xor(object, key, sizeof(RSandBox), key->size); // decrypt pointers
    Xor(object->descriptorTable, key,
            object->descriptorsInfo.count * sizeof(RControlDescriptor), key->size); // decrypt descriptors table
    Xor(object->memPart, key, sizeof(RByteArray), key->size);                       // decrypt memory ptr
    Xor(object->memPart->array,  key, object->memPart->size, key->size);            // decrypt memory chunk
}

