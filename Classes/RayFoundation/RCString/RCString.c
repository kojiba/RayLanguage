#include "RCString.h"

$constructor(RCString)) {
    object = allocator(RCString);
    object->string = allocator(100 * sizeof(char));
    object->size = 100;
    return object;
}

$destructor(RCString) {
    deallocator(object->string);
}

$method(void, setString, RCString), char *string) {

}