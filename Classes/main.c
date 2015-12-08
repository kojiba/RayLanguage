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
#include <RayFoundation/REncoding/PurgeEvasionUtils.h>
#include <RayFoundation/Utils/PurgeEvasionParallel.h>

#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    uint64_t key[8] = {};
    const uint64_t messageSize = 1024 * 16;
    uint64_t messageTemp[messageSize] = {};
    uint64_t resultSize;

    initRClock();
    byte* encrypted = encryptPurgeEvasion(messageTemp,
                                       messageSize,
                                       (uint64_t *) &key,
                                       &resultSize);

    tickRClock();

    RPrintf("Simple [%llu]\n", resultSize);
    printByteArrayInHexWithScreenSize(encrypted, resultSize, 64);

    deallocator(encrypted);

    uint64_t key2[8] = {};
    uint64_t messageTemp2[messageSize] = {};
    uint64_t resultSize2;

    tickRClock();

    byte* encryptedParallel = encryptPurgeEvasionParallel(messageTemp2,
                                                          messageSize,
                                                          (uint64_t *) &key2,
                                                          &resultSize2, processorsCount());
    tickRClock();

    RPrintf("\nParallel [%llu]\n\n", resultSize2);
    printByteArrayInHexWithScreenSize(encryptedParallel, resultSize2, 64);

    deallocator(encryptedParallel);


    endRay();
}