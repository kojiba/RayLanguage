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
    // place your code here
    byte *data = makeFlushedBytes(10, 1);

    byte *key = makeFlushedBytes(10, 2);


    Xor(data,key, 10, 10);
    printByteArrayInHex(data, 10);

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}