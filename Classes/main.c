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

pointer exec(pointer data) {
    size_t iterator;
    pointer ptr;

    forAll(iterator, 3) {
        ptr = metaAlloc(10, "In tread 1");
//        free(ptr);
    }

    return nil;
}


int main(int argc, const char *argv[]) {
    size_t iterator;
    initRClock();
    enablePool(RPool);
    ComplexTest();

    RThreadPool *pool = c(RThreadPool)(nil);
    $(pool, m(setDelegateFunction, RThreadPool)), exec);

    forAll(iterator, 1)
        $(pool, m(addWithArg, RThreadPool)), nil);


    $(pool, m(join, RThreadPool)));


    deleter(pool, RThreadPool);
    pointer ptr = metaAlloc(20, "In main");


    tickRClock();

    endRay();
}
