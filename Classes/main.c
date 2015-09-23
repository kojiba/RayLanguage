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
#include <RayFoundation/Utils/PurgeEvasionConnection.h>

#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    size_t iterator;

    uint64_t masterKey[8] = {};
    uint64_t masterKey2[8] = {};
    uint64_t *key;

    PEConnectionContext *context = initPEContext(masterKey);

    PEConnectionContext *receiver = initPEContext(masterKey2);

    RByteArray *array = RBfromRCS(RS("Hello"));

    forAll(iterator, 10) {
        RByteArray *result = encryptDataWithConnectionContext(array, context);
        RPrintf("Iteration : %lu\n", iterator);
//        printByteArrayInHexWithScreenSize((const byte *) result->array, sizeof(uint64_t), 64);
        printByteArrayInHexWithScreenSize((const byte *) result->array + sizeof(uint64_t), result->size - sizeof(uint64_t), 32/*64*/);

        RByteArray *decrypted = decryptDataWithConnectionContext(result, receiver);
        if(decrypted != nil) {
            RPrintf("Decrypted\n");
            printByteArrayInHexWithScreenSize((const byte *) decrypted->array, decrypted->size, 64);
            deleter(decrypted, RByteArray);
        }
        RPrintf("\n");

        deleter(result, RByteArray);
    }

    deleter(array, RByteArray);

    deleter(context, PEConnectionContext);
    deleter(receiver, PEConnectionContext);

    endRay();
}