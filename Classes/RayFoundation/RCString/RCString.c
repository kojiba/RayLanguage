#include "RCString.h"

constructor(RCString)) {
    object = allocator(RCString);
    object->baseString = allocator(100 * sizeof(char));
    object->size = 100;
    return object;
}

destructor(RCString) {
    deallocator(object->baseString);
}

method(void, setString, RCString), char *string) {

}