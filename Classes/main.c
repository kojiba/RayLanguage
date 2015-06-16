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
#include <unistd.h>
#include "Tests.h"

pointer exec(pointer data) {
    size_t iterator;
    pointer ptr;

    forAll(iterator, 3) {
        ptr = malloc((size_t) (rand() % 10) + 1);
        free(ptr);
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

    forAll(iterator, 5000)
        $(pool, m(addWithArg, RThreadPool)), nil, yes);


    $(pool, m(joinSelfDeletes, RThreadPool)));
//    sleep(2);



    deleter(pool, RThreadPool);

    tickRClock();

    endRay();
}
