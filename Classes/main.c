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

    forAll(iterator, 3) {
        malloc((size_t) (rand() % 10) + 1);
    }

    return nil;
}


int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    ComplexTest();

    RThreadPool *pool = c(RThreadPool)(nil);
    $(pool, m(setDelegateFunction, RThreadPool)), exec);

    forAll(iterator, 20)
        $(pool, m(addWithArg, RThreadPool)), nil);


    $(pool, m(join, RThreadPool)));

    p(RThreadPool)(pool);

    deleter(pool, RThreadPool);
    endRay();
}
