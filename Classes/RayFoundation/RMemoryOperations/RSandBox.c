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
    if(object != nullPtr) {
        object->classId               = registerClassOnce(toString(RSandBox));
        object->descriptorTable       = RAlloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart               = makeRByteArray(sizeOfMemory);
        object->descriptorsInfo.count = descriptorsCount;
        object->descriptorsInfo.from  = 0;
        object->innerMallocPtr        = innerMallocPtr;
        object->innerFreePtr          = innerFreePtr;
        object->allocationMode        = RSandBoxAllocationModeRandom;
        object->rangeGenerator        = nullPtr;
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
    RPrintf("\t Mem total  - %qu (bytes)\n", object->memPart->size);
    if(object->allocationMode == RSandBoxAllocationModeStandart) {
        RPrintf("\t Mem placed - %qu (bytes)\n",
                object->descriptorTable[object->descriptorsInfo.from - 1].memRange.from
                +
                object->descriptorTable[object->descriptorsInfo.from - 1].memRange.count);
    }
    RPrintf("\t Descriptors count - %qu\n", object->descriptorsInfo.count);
    RPrintf("\t Descriptors filled - %qu\n", object->descriptorsInfo.from);
    forAll(iterator, object->descriptorsInfo.from) {
        RPrintf("\t\t [%qu : %qu]\n", object->descriptorTable[iterator].memRange.from, object->descriptorTable[iterator].memRange.count);
    }
    RPrintLn("}\n");

    // switch to old
    revertPtrs();
}

singleton(RSandBox) {
    static RSandBox *instance = nullPtr;
    if(instance == nullPtr) {
        $(instance, c(RSandBox)), 8192, 128, RTrueMalloc, RTrueFree);
    }
    return instance;
}

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
    __darwin_ptrdiff_t shift = ptr - (pointer)(object->memPart->array);
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
                    placeToAlloc.from = rand() % object->memPart->size;
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
            return nullPtr;
        } else {
            return object->memPart->array + placeToAlloc.from;
        }
    } else {
        RError("RSB. Not enought descriptors", object);
        return nullPtr;
    }
}