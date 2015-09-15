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

//    char string[] = "hello brothers";
//    uint64_t key[8] = {};
//    memcpy(key, "key", sizeof(key));
//
//
//    size_t outSize;
//    size_t decSize;
//    char *encrypted = encryptPurgeEvasion(string, sizeof(string), key, &outSize);
//
//    memset(key, 0, purgeBytesCount);
//    memcpy(key, "key", sizeof(key));
//
//
//    char *decrypted = decryptPurgeEvasion(encrypted, outSize, key, &decSize);
//    RPrintf("%s\n", decrypted);
//
//    deallocator(encrypted);
//    deallocator(decrypted);
//
//
//    memset(key, 0, purgeBytesCount);
//    memcpy(key, "key", sizeof(key));
//
//    RByteArray *keyArray = makeRByteArray(key, sizeof(key));
//    RByteArray *array = makeRByteArray(string, sizeof(string));
//
//    RByteArray *encryptedArray = $(array, m(encryptPurgeEvasion, RByteArray)), keyArray);
//
//    p(RByteArray)(encryptedArray);
//
//    RByteArray *decryptedArray = $(encryptedArray, m(decryptPurgeEvasion, RByteArray)), keyArray);
//
//    p(RByteArray)(decryptedArray);
//
//    deallocator(array);
//    deallocator(keyArray);
//    deleter(encryptedArray, RByteArray);
//    deleter(decryptedArray, RByteArray);

    RString *result2 = $(RS("Lorem ipsum dolor sit amet, consectetur adipiscing elit, \n"
                                    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. \n"
                                    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut\n"
                                    "aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in\n"
                                    "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint\n"
                                    "occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit\n"
                                    "anim id est laborum."),
                         m(encryptPurgeEvasionBase64, RString)), RS("Key"));

    p(RCString)(result2);

    RString *decrypted = $(result2, m(decryptPurgeEvasionBase64, RString)), RS("Key"));

    p(RCString)(decrypted);

    deleter(decrypted, RString);
    deleter(result2, RString);

    endRay();
}