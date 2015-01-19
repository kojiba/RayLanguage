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

#define TEST_COUNT 50

RArray *arrayTest;

pointer func1(pointer arg) {
//    enablePool(RPool);
    initPointers();
    for(int i = 0; i < TEST_COUNT; ++i) {
        $(arrayTest, m(addObject, RArray)), randomRCString());
    }
    return 0;
}

int RThreadTest(void) {
    arrayTest = makeRArray();
    RThread *thread1 = $(nil, c(RThread)), nil, func1, "1");

    for(int i = 0; i < TEST_COUNT; ++i) {
        $(arrayTest, m(addObject, RArray)), randomRCString());
    }

    $(arrayTest, p(RArray)));

    $(thread1, m(join, RThread)));
    deleter(thread1, RThread);
    return 0;
}

int main(int argc, const char *argv[]) {
//    enablePool(RPool);
    RCTSingleton;
//    ComplexTest();
    // place your code here
    RThreadTest();

    deleter(RCTSingleton, RClassTable);
//    $(RPool, p(RAutoPool)));
//    deleter(RPool, RAutoPool);

    return 0;
}