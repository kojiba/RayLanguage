#include "RBuffer.h"
#include "../RClassTable/RClassTable.h"

constructor(RBuffer)) {
    object = allocator(object);
    if(object != nullPtr) {
        // allocation of buffer
        master(object, RByteArray) = makeRByteArray(startSizeOfRBufferDefault);
        if(master(object, RByteArray) != nullPtr) {

            // allocation of sizes array
            object->sizesArray = RAlloc(sizeof(size_t) * sizeOfObjectsOfRBufferDefault);

            if(object->sizesArray != NULL) {
                object->classId = registerClassOnce(toString(RBuffer));
                object->freePlaces = startSizeOfRBufferDefault;
                object->count = 0;
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
    RPrintf("%s object - %p {\n", toString(RBuffer), object);
    RPrintf("\t Total   size : %qu (bytes)\n", master(object, RByteArray)->size);
    RPrintf("\t Placed  size : %qu (bytes)\n", object->totalPlaced);
    RPrintf("\t Count objcts : %qu\n", object->count);
    RPrintf("\t Free  places : %qu\n", object->freePlaces);
    forAll(iterator, object->count) {
        RPrintf("");
    }
    RPrintf("} %s object - %p\n", toString(RBuffer), object);
}

#pragma mark Reallocation
//
//method(RByteArray*, addSize,   RBuffer),    size_t newSize); // adds some size, store data, returns self
//method(RByteArray*, flush,     RBuffer));                    // flushes buffer, returns self
//method(RByteArray*, sizeToFit, RBuffer));                    // make without free places, store data, returns self
//
//#pragma mark Data operations
//
//method(void, addData, RBuffer), pointer *data) {
//
//}
//
//method(void, getData, RBuffer), size_t iterator);