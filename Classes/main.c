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

#include <RayFoundation/RayFoundation.h>

#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    size_t iterator;


    initRClock();
    forAll(iterator, 1024 * 1024) {
        malloc(4096);
    }
    tickRClock();

    deleter(stringConstantsTable(), RDictionary);
                     deleter(RCTSingleton, RClassTable);
//                     p(RAutoPool)(RPool);
                     deleter(RPool, RAutoPool);
                     stopConsole();
                     return 0;
}