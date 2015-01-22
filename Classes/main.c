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

sandBoxNamed(std, 65535);

int main(int argc, const char *argv[]) {
    size_t iterator;
    enablePool(RPool);
    RCTSingleton;
    ComplexTest();
    // place your code here

    std()->allocationMode = RSandBoxAllocationModeStandart;

    forAll(iterator, 20) {
        RCString *temp = randomRCString();
    }

    $(std(), p(RSandBox)));
    deleter(std(), RSandBox);

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}