#include "RSandBox.h"
#include "../RClassTable/RClassTable.h"

constructor(RSandBox), size_t sizeOfMemory, size_t descriptorsCount) {
    object = trueMalloc(sizeof(RSandBox));
    if(object != nullPtr) {
        object->classId = registerClassOnce(toString(RSandBox));
        object->descriptorTable = trueMalloc(sizeof(RControlDescriptor) * descriptorsCount);
        object->memPart = makeRByteArray(sizeOfMemory);
        object->descriptorsInfo.count = descriptorsCount;
        object->descriptorsInfo.from = 0;
    }
    return object;
}

destructor(RSandBox) {
    $(object->memPart, d(RByteArray)) );
    deallocator(object->memPart);
    deallocator(object->descriptorTable);
}
singleton(RSandBox) {
    static RSandBox *instance = nullPtr;
    if(instance == nullPtr) {
        $(instance, c(RSandBox)), 1024, 100 );
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

method(pointer, malloc, RSandBox), size_t sizeInBytes) {
    RRange placeToAlloc;
    placeToAlloc.count = sizeInBytes;
    placeToAlloc.from = rand() % object->memPart->size;
    while($(object, m(isRangeFree,RSandBox)), placeToAlloc) == no) {
        placeToAlloc.from = rand() % object->memPart->size;
    }
    $(object, m(addFilledRange, RSandBox)), placeToAlloc);
    return placeToAlloc.from;
}