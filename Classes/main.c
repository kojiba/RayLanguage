/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation.h>

#include "Tests.h"

rbool stringSerializer(pointer context, pointer object, size_t iterator) {
    SerializerData* temp = ((REnumerateDelegate*)context)->context;

    temp->size              = ((RString*)object)->size;
    temp->serializePtrStart = ((RString*)object)->baseString;
    temp->next = nil;
    return yes;
}

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    ComplexTest();

    initRClock();
    REnumerateDelegate delegate;
    SerializerData *tempSizeData = allocator(SerializerData);
    delegate.context           = tempSizeData;
    delegate.virtualEnumerator = stringSerializer;

    RArray *array = makeRArray();
    stringDelegates(array);

    forAll(iterator, 10000) {
        $(array, m(addObject, RArray)), randomRCString());
    }

    RBuffer *buffer = $(array, m(serializeToBufferDelegate, RArray)), &delegate);
    tickRClock();

    deleter(buffer, RBuffer);
    deleter(array, RArray);
    deallocator(tempSizeData);
    tickRClock();

    endRay();
}
