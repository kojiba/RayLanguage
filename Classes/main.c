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

void some(int *trains) {
    printf("cho, chooo, motherfucked\n");
    ++*trains;
}

void endSome() {}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test


    size_t sizeOfSome = (size_t)&endSome - (size_t)&some;

    unsigned char *body = malloc(sizeOfSome);
    memcpy(body, some, sizeOfSome);
    RData *function = makeRData(body, sizeOfSome, RDataTypeBytes);

    p(RData)(function);

    RString *baseHash = $(function, m(evasionHashBase64, RString)));
    baseHash->type = RDataTypeASCII;
    p(RString)(baseHash);

    deleter(baseHash, RString);
    deleter(function, RData);


    endRay();
}