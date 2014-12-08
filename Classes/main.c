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

#include "RayFoundation/RayFoundation.h"
#include "Tests.h"

int main(int argc, const char *argv[]) {
    initPointers();
    RPool;
    RCTSingleton;
    ComplexTest();
    size_t iterator;
    RBuffer *buffer = $(nil, c(RBuffer)));

    forAll(iterator, 5) {
        RCString *temp = randomRCString();
        $(buffer, m(addData, RBuffer)), temp->baseString, temp->size);
        deleter(temp, RCString);
    }


    $(buffer, p(RBuffer)));
    $(buffer, m(saveToFile, RBuffer)), "Buffer.test.txt");
    deleter(buffer, RBuffer);

    RBuffer *buffer2 = RBufferFromFile("Buffer.test.txt");
    $(buffer2, p(RBuffer)));
    deleter(buffer2, RBuffer);

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}