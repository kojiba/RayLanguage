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
    ComplexTest();


    printf("Ima main\n");
    __block int counter = 0;

    RThreadJoin(
            RThreadWithBlock(^{
                printf("Ima block, yo!!! Counter %d\n", counter);
                ++counter;

                autoReleaseBlock(^{
                    size_t iterator;
                    printf("Ima inner autorelease block\n");
                    forAll(iterator, 100) {
                        malloc(20); // leaks, but not
                    }
                });
            })
    );

    RThreadJoin(
            makeRThreadWithBlock("I_NEVER_ASK_FOR_THIS", (RThreadBlock) ^(char *arg) {
                printf("Ima block with augumentations, but %s. Counter %d\n", arg, counter);
                ++counter;
            })
    );

    autoReleaseBlock(^{
        size_t iterator;
        printf("Ima main autorelease block\n");
        forAll(iterator, 100) {
            malloc(20); // leaks, but not
        }
    });

    printf("Ima main again! Counter %d\n", counter);

    endRay();
}