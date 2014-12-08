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

    RByteArray *temp = makeRByteArray(10);
    temp->array[0] = 0x2;
    temp->array[1] = 0x1;
    temp->array[2] = 0x2;
    temp->array[3] = 0x3;
    temp->array[4] = 0x4;
    temp->array[5] = 0x5;

    size_t array[] = {1, temp->array[0], temp->array[0] + 1, 0};

    RBuffer *buffer = $(temp, m(serializeToBuffer, RByteArray)), array );
    $(buffer, p(RBuffer)));
    $(buffer, m(saveToFile, RBuffer)), "Buffer.test.txt");

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}