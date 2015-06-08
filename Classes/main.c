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

autoPoolNamed(somePool);

autoPoolNamed(somePool2);

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    ComplexTest();

    enablePool(somePool());

    forAll(iterator, 5) {
        malloc(13);
    }

        enablePool(somePool2());

        forAll(iterator, 5) {
            malloc(7);
        }

        p(RAutoPool)(somePool());
        p(RAutoPool)(somePool2());
        deleter(somePool2(), RAutoPool);

    p(RAutoPool)(somePool());
    deleter(somePool(), RAutoPool);

    endRay();
}
