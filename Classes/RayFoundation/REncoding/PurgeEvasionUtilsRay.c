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

constMethod(RBytes *, encryptPurgeEvasion, RBytes), const RBytes *key) {
    RBytes *result = makeRBytes(nil, 0);
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
            RError("RBytes. encryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
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

constMethod(RBytes *, decryptPurgeEvasion, RBytes), const RBytes *key) {
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
        return makeRBytes(temp, resultSize);
    } else {
        return nil;
    }
}

constMethod(RBytes *, evasionHash, RBytes)) {
    uint64_t hash[8];
    evasionHashData(object->data, object->size, hash);
    return makeRBytes(getByteArrayCopy((const byte *) hash, evasionBytesCount), evasionBytesCount);
}

inline
constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key) {
    RBytes *temp = makeRBytes((byte *) key->baseString, key->size),
               *tempObject = makeRBytes((byte *) object->baseString, object->size),
               *tempResult;
    RString    *result = nil;

    if(temp != nil
       && tempObject != nil) {
        tempResult = $(tempObject, m(encryptPurgeEvasion, RBytes)), temp);
        if(tempResult != nil) {
            result = $(tempResult, m(encodeBase64, RBytes)));
            deleter(tempResult, RBytes);
        }
        deallocator(temp);
        deallocator(tempObject);
    }
    return result;
}

inline
constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key) {
    RBytes *tempKey = makeRBytes((byte *) key->baseString, key->size),
               *tempObject = nil,
               *tempResult = nil;
    RString    *result = nil;

    if(tempKey != nil) {
        tempResult = $(object, m(decodeBase64ToBytes, RString)));
        if(tempResult != nil) {
            tempObject = $(tempResult, m(decryptPurgeEvasion, RBytes)), tempKey);
            if(tempObject != nil) {
                result = RCS((const char*)tempObject->data);
                result->baseString = RReAlloc(result->baseString, result->size + 1);
                result->baseString[tempObject->size] = 0;
                deallocator(tempObject);
            }
            deleter(tempResult, RBytes);
        }
        deallocator(tempKey);
    }
    return result;
}

constMethod(RString *, evasionHashBase64, RString)) {
    RBytes *tempObject = makeRBytes((byte *) object->baseString, object->size);
    RString    *result     = nil;
    if(tempObject != nil) {
        RBytes *hash = $(tempObject, m(evasionHash, RBytes)));
        if(hash != nil) {
            result = $(hash, m(encodeBase64, RBytes)));
            deleter(hash, RBytes);
        }
        deallocator(tempObject);
    }
    return result;
}