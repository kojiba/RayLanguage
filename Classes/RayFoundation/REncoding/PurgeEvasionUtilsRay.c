/**
 * PurgeEvasionIUtilsRay.c
 * Utils for work with cipher for libRay.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

constMethod(RData *, encryptPurgeEvasion, RData), const RData *key) {
    RData *result = allocator(RData);
    uint64_t    resultSize;
    byte        tempKey[purgeBytesCount]; // 512 bits

    if(key->size >= purgeBytesCount) {
        memcpy(&tempKey, key->data, purgeBytesCount);
    } else {
        // set and cpy must have one calculating speed
        memcpy(&tempKey, key->data, key->size);
        memset(&tempKey, 0,               purgeBytesCount - key->size); // add some zeros
    }
    if(result != nil) {
        result->data = encryptPurgeEvasion(object->data,
                                            object->size,
                                            (uint64_t *) &tempKey,
                                            &resultSize);
        if(resultSize > UINT32_MAX
           && sizeof(size_t) == 4) {
            RError("RData. encryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                           "Return pure not base64 array to store data.", result);
            return result;
        }

        if (result->data != nil && resultSize > 6) {
            result->size = resultSize;
            return result;
        }
    }
    return nil;
}

constMethod(RData *, decryptPurgeEvasion, RData), const RData *key) {
    byte    *temp;
    uint64_t resultSize;
    byte tempKey[purgeBytesCount]; // 512 bits

    if(key->size >= purgeBytesCount) {
        memcpy(&tempKey, key->data, purgeBytesCount);
    } else {
        memcpy(&tempKey, key->data, key->size);
        memset(&tempKey, 0,          purgeBytesCount - key->size); // add some zeros
    }

    temp = decryptPurgeEvasion(object->data,
                               object->size,
                               (uint64_t *) &tempKey,
                               &resultSize);

    if (resultSize > UINT32_MAX
        && sizeof(size_t) == 4) {
        RError("RString. decryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                       "Return pure not base64 array to store data.", object);
    }
    if(temp != nil) {
        return makeRDataBytes(temp, resultSize);
    } else {
        return nil;
    }
}

constMethod(RData *, evasionHash, RData)) {
    uint64_t hash[8];
    evasionHashData(object->data, object->size, hash);
    return makeRDataBytes(getByteArrayCopy((const byte *) hash, evasionBytesCount), evasionBytesCount);
}

inline
constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key) {
    RData  *temp;
    RString *result = nil;
    temp = $(object, m(encryptPurgeEvasion, RData)), temp);
    if(temp != nil) {
        result = $(temp, m(encodeBase64, RData)));
        result->type = RDataTypeASCII;
        deleter(temp, RData);
    }
    return result;
}

inline
constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key) {
    RData *tempResult;
    RString *result = nil;
    tempResult = $(object, m(decodeBase64, RString)));
    if(tempResult != nil) {
        result = $(tempResult, m(decryptPurgeEvasion, RData)), key);

        result->type = RDataTypeASCII;
        deleter(tempResult, RData);
    }
    return result;
}

constMethod(RString *, evasionHashBase64, RString)) {
    RString    *result     = nil;
    RData *hash = $(object, m(evasionHash, RData)));
    if(hash != nil) {
        result = $(hash, m(encodeBase64, RData)));
        result->type = RDataTypeASCII;
        deleter(hash, RData);
    }
    return result;
}