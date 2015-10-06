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
    ComplexTest(); // lib testRAY_BLOCKS_ON


    printf("Ima main\n");

    RThreadJoin(
            RThreadWithBlock( ^{
                                printf("Ima block, yo!!!\n");
            })
    );

    RThreadJoin(
            makeRThreadWithBlock( "I_NEVER_ASK_FOR_THIS", (RThreadBlock) ^(char* arg){
                            printf("Ima block with augumentations, but %s\n", arg);
                        })
    );

    printf("Ima main again!\n");

    endRay();
}