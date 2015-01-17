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

RArray* arrayTest() {
    static RArray *array = nil;
    if(array == nil) {
        array = makeRArray();
//        array->printerDelegate = (void (*)(pointer)) p(RCString);
        array->destructorDelegate = free;
    }
    return array;
}

pointer func1(pointer arg) {
    enablePool(RPool);
    for(int i = 0; i < 7; ++i) {
        RCString *temp = RS((char*)arg);
        addObjectRArray(arrayTest(), temp);
    }
    return 0;
}

int RThreadTest(void) {
    arrayTest();
    RThread *thread1 = $(nil, c(RThread)), nil, func1, "1");
    RThread *thread2 = $(nil, c(RThread)), nil, func1, "2");
    RThread *thread3 = $(nil, c(RThread)), nil, func1, "3");

    $(thread1, m(join, RThread)));
    $(thread2, m(join, RThread)));
    $(thread3, m(join, RThread)));

    deleter(thread1, RThread);
    deleter(thread2, RThread);
    deleter(thread3, RThread);

    $(arrayTest(), p(RArray)));

    if(arrayTest()->count != 21) {
        RError("RThread. Test error, bad counter.", nil);
        return -1;
    }
    deleter(arrayTest(), RArray);
    return 0;
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    RCTSingleton;
//    ComplexTest();
    // place your code here
    RThreadTest();

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);

    return 0;
}