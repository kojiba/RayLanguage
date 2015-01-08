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

int main(int argc, const char *argv[]) {
    initPointers();
    RPool;
    RCTSingleton;
    ComplexTest();
    // place your code here
    RSender *sender = $(nil, c(RSender)), 7777);
    $(sender, m(setReceiverAddress, RSender)), "8.8.8.8");
    $(sender, m(send, RSender)), RSC("Hello"));

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}