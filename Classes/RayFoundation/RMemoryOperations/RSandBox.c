#include "RSandBox.h"
#include "../RClassTable/RClassTable.h"

constructor(RSandBox), size_t sizeOfMemory, size_t descriptorsCount) {
    object = allocator(RSandBox);
    if(object != NULL) {
        object->classId = registerClassOnce(toString(RSandBox));
    }
    return object;
}

destructor(RSandBox) {

}