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

constMethod(RByteArray *, encryptPurgeEvasion, RByteArray), const RByteArray *key) {
    RByteArray *result = makeRByteArray(nil, 0);
    uint64_t    resultSize;
    byte        tempKey[purgeBytesCount]; // 512 bits

    if(key->size > purgeBytesCount) {
        memcpy(&tempKey, key->array, purgeBytesCount);
    } else {
        // set and cpy must have one calculating speed
        memcpy(&tempKey, key->array, key->size);
        memset(&tempKey, 0,               purgeBytesCount - key->size); // add some zeros
    }
    if(result != nil) {
        result->array = encryptPurgeEvasion(object->array,
                                            object->size,
                                            (uint64_t *) &tempKey,
                                            &resultSize);
        if(resultSize > UINT32_MAX
           && sizeof(size_t) == 4) {
            RError("RByteArray. encryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                           "Return pure not base64 array to store data.", result);
            return result;
        }

        if (result->array != nil && resultSize > 6) {
            result->size = resultSize;
            return result;
        }
    }
    return nil;
}

constMethod(RByteArray *, decryptPurgeEvasion, RByteArray), const RByteArray *key) {
    byte    *temp;
    uint64_t resultSize;
    byte tempKey[purgeBytesCount]; // 512 bits

    if(key->size > purgeBytesCount) {
        memcpy(&tempKey, key->array, purgeBytesCount);
    } else {
        memcpy(&tempKey, key->array, key->size);
        memset(&tempKey, 0,          purgeBytesCount - key->size); // add some zeros
    }

    temp = decryptPurgeEvasion(object->array,
                               object->size,
                               (uint64_t *) &tempKey,
                               &resultSize);

    if (resultSize > UINT32_MAX
        && sizeof(size_t) == 4) {
        RError("RString. decryptPurgeEvasionBase64. Lose data at 32-bit system and very long encryption.\n"
                       "Return pure not base64 array to store data.", object);
    }

    return makeRByteArray(temp, resultSize);
}

constMethod(RByteArray *, evasionHash, RByteArray)) {
    uint64_t hash[8];
    evasionHashData(object->array, object->size, hash);
    return makeRByteArray(getByteArrayCopy((const byte *) hash, evasionBytesCount), evasionBytesCount);
}

inline
constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key) {
    RByteArray *temp = makeRByteArray((byte *) key->baseString, key->size),
               *tempObject = makeRByteArray((byte *) object->baseString, object->size),
               *tempResult;
    RString    *result = nil;

    if(temp != nil
       && tempObject != nil) {
        tempResult = $(tempObject, m(encryptPurgeEvasion, RByteArray)), temp);
        if(tempResult != nil) {
            result = $(tempResult, m(encodeBase64, RByteArray)));
            deleter(tempResult, RByteArray);
        }
        deallocator(temp);
        deallocator(tempObject);
    }
    return result;
}

inline
constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key) {
    RByteArray *temp = makeRByteArray((byte *) key->baseString, key->size),
               *tempObject,
               *tempResult;
    RString    *result = nil;

    if(temp != nil) {
        tempResult = $(object,     m(decodeBase64ToBytes, RString)));
        tempObject = $(tempResult, m(decryptPurgeEvasion, RByteArray)), temp);

        if(tempObject != nil) {
            result = RCS((const char*)tempObject->array);
            result->baseString = RReAlloc(tempObject->array, result->size);
            deallocator(tempObject);
        }
        deleter(tempResult, RByteArray);
        deallocator(temp);
    }
    return result;
}

constMethod(RString *, evasionHashBase64, RString)) {
    RByteArray *tempObject = makeRByteArray((byte *) object->baseString, object->size);
    RString    *result     = nil;
    if(tempObject != nil) {
        RByteArray *hash = $(tempObject, m(evasionHash, RByteArray)));
        if(hash != nil) {
            result = $(hash, m(encodeBase64, RByteArray)));
            deleter(hash, RByteArray);
        }
        deallocator(tempObject);
    }
    return result;
}