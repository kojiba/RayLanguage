/**
 * PurgeEvasionIUtilsRay.c
 * Utils for work with cipher for libRay.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 8/26/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "PurgeEvasionUtilsRay.h"
#include "PurgeEvasionUtils.h"
#include "RBase64.h"

constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key) {
    RString *temp, *result = makeRCString();
    uint64_t resultSize;
    byte tempKey[purgeBytesCount]; // 512 bits

    // memset and memcpy must have one calculating speed
    memcpy(&tempKey, key->baseString, key->size);
    memset(&tempKey, 0,               purgeBytesCount - key->size); // add some zeros
    if(result != nil) {
        result->baseString = encryptPurgeEvasion(object->baseString, object->size,
                                                 (uint64_t *) &tempKey, &resultSize);
        if(resultSize > UINT32_MAX
                && sizeof(size_t) == 4) {
            RError("RString. encryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                           "Return pure not base64 array to store data.", result);
            return result;
        }

        if (result->baseString != nil && resultSize > 8) {
            result->size = resultSize;
            temp = $(result, m(encodeBase64, RString)));
            deleter(result, RString);
            result = temp;
            return result;
        }
    }
    return nil;
}

constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key) {
    RString *result;
    char    *temp;
    uint64_t resultSize;
    byte tempKey[purgeBytesCount]; // 512 bits

    memcpy(&tempKey, key->baseString, key->size);
    memset(&tempKey, 0,               purgeBytesCount - key->size); // add some zeros

    result = $(object, m(decodeBase64, RString)));

    if(result != nil) {
        temp = decryptPurgeEvasion(result->baseString, result->size,
                                                 (uint64_t *) &tempKey, &resultSize);
        d(RString)(result);

        if(resultSize > UINT32_MAX
           && sizeof(size_t) == 4) {
            RError("RString. decryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                           "Return pure not base64 array to store data.", result);
        }
        result->baseString = temp;
        result->size = resultSize;
        return result;
    }
    return nil;
}