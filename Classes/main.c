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
#include <assert.h>
#include "Tests.h"

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    RCTSingleton;
    ComplexTest();
    // place your code here

    RBuffer *buffer = $(nil, c(RBuffer)));
    forAll(iterator, 1024) {
        RCString *temp = randomRCString();
        if(temp->size == 0) {
            RError("Size is 0", temp);
        }
        $(buffer, m(addData, RBuffer)), temp->baseString, temp->size);
        deleter(temp, RCString);
    }
    $(buffer, p(RBuffer)));
    $(buffer, m(saveToFile, RBuffer)), "array-buffer-test.bin");
    deleter(buffer, RBuffer);

    RBuffer *newBuffer = RBufferFromFile("array-buffer-test.bin");
    $(newBuffer, p(RBuffer)));
    deleter(newBuffer, RBuffer);

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}