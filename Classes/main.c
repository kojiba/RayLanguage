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

autoPoolNamed(ThreadPool);

pointer func1(pointer arg) {
    RArray * array = makeRArray();
    RByteArray * byteArray = makeRByteArray(10);
    deleter(array, RArray);
    deleter(byteArray, RArray);
    return 0;
}

int main(int argc, const char *argv[]) {
    initPointers();
    RPool;
    RCTSingleton;
    ComplexTest();
    // place your code here
//    RThread *thread1 = $(nil, c(RThread)), nil, func1, nil);
//    RThread *thread2 = $(nil, c(RThread)), nil, func1, nil);

//    $(thread1, m(join, RThread)));
//    $(thread2, m(join, RThread)));


    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);

    return 0;
}