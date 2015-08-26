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

method(RString *, encryptPurgeEvasionBase64, RString), const RString *key) {
    RString *temp, *result = makeRCString();
    uint64_t resultSize;
    byte tempKey[purgeBytesCount] = {}; // 512 bits of zeros

    memcpy(&tempKey, key->baseString, key->size); // fixme not crypto-safe
    if(result != nil) {
        result->baseString = encryptPurgeEvasion(object->baseString, object->size,
                                                 (uint64_t *) &tempKey, &resultSize);
        if(resultSize > UINT32_MAX
                && sizeof(size_t) == 4) {
            RError("encryptPurgeEvasionBase64RString. Lose data at 32-bit system and very long encryption.\n"
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