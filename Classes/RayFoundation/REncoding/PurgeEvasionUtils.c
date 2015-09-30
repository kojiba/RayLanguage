/**
 * PurgeEvasionUtils.h
 * Data cipher based on purge encryption
 * and evasion in mode pseudo-rand generator.
 * Data hash based on evasion
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 06/02/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RayFoundation/RMemoryOperations/RByteOperations.h"
#include "PurgeEvasionUtils.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

#ifndef nil
    #define nil ((void*)0)
#endif

#ifndef forAll
    #define forAll(iterator, count) for(iterator = 0; iterator < (count); ++iterator)
#endif

void evasionHashData(const void *text, uint64_t size, uint64_t *outputHash) {
    uint64_t iterator;
    uint64_t hashTemp[8] = {};
    uint8_t  half = evasionBytesCount / 2;
    uint64_t hashCount = size / half;
    uint64_t addition  = size % half;


    forAll(iterator, hashCount) {
        memcpy((uint8_t*) &hashTemp[0] + half, text + iterator * half, half);
        evasionRand(hashTemp);
    }

    if(addition) {
        memcpy((uint8_t*) hashTemp + half, text + hashCount * half, addition);
        memset((uint8_t*) hashTemp + half + addition, 0, half - addition);
        evasionRand(hashTemp);
    }
    // final
    memcpy(outputHash, hashTemp, evasionBytesCount);
}


void* encryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize) { // key changed and data not
    uint64_t  iterator;
    uint8_t  *textTemp     = nil;
    uint64_t  totalSize   = size + sizeof(uint64_t);
    uint64_t  cipherCount = totalSize / purgeBytesCount;
    uint64_t  addition    = totalSize % purgeBytesCount;
    uint64_t  hash[8];
    uint8_t   keyTemp[purgeBytesCount];

    if(addition != 0) {
        totalSize += purgeBytesCount - addition;
        ++cipherCount;
    }

    textTemp = RAlloc(totalSize);

    if(textTemp != nil) {
        *cryptedSize = 0;

        memcpy(textTemp, &size, sizeof(uint64_t));       // add size in front
        memcpy(textTemp + sizeof(uint64_t), text, size); // copy other text

        if (addition != 0) { // add some zeros if needed
            memset(textTemp + size + sizeof(uint64_t), 0, purgeBytesCount - addition);
        }

        evasionHashData(textTemp + sizeof(uint64_t), totalSize - sizeof(uint64_t), (uint64_t *)hash); // hash data with padding

        forAll(iterator, cipherCount) { // pure crypt with key
            evasionRand(key);
            memcpy(keyTemp, key, purgeBytesCount);
            purgeEncrypt((uint64_t *) (textTemp + iterator * purgeBytesCount), (uint64_t*) keyTemp);
            memset(keyTemp, 0, purgeBytesCount);
        }

        // crypt hash by last key
        evasionRand(key);        // rand key
        memcpy(keyTemp, key, purgeBytesCount);
        purgeEncrypt(hash, (uint64_t *) keyTemp); // encrypt hash
        memset(keyTemp, 0, purgeBytesCount);

        // append hash
        textTemp = RReAlloc(textTemp, totalSize + evasionBytesCount);
        if(textTemp != nil) {
            memcpy(textTemp + totalSize, hash, evasionBytesCount); // append hash
            *cryptedSize = totalSize; // store
            *cryptedSize += evasionBytesCount;
        }
    }
    return textTemp;
}

void* decryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *encryptedSize) { // key changed and data not
    uint64_t iterator;
    uint8_t *textTemp    = nil,
            *plainText   = nil;
    uint64_t cipherCount = (size - evasionBytesCount) / purgeBytesCount;
    uint64_t sizeOfText;
    uint8_t  keyTemp[purgeBytesCount];
    uint64_t hash[8];
    uint8_t *hashPtr = nil;

    if(size % purgeBytesCount) {
        RError("decryptPurgeEvasion. Bad data size. Must be multiple of 64. Data size in bytes\n", nil);
        return nil;
    }

    textTemp = RAlloc(size);

    if(textTemp != nil) {
        *encryptedSize = 0;
        memcpy(textTemp, text, size);
        hashPtr = (textTemp + size - evasionBytesCount);

        forAll(iterator, cipherCount) {
            evasionRand(key);
            memcpy(keyTemp, key, purgeBytesCount);
            purgeDecrypt((uint64_t *) (textTemp + iterator * purgeBytesCount), (uint64_t *) keyTemp);
            memset(keyTemp, 0, purgeBytesCount);
        }

        // decrypt hash by last key
        evasionRand(key);
        memcpy(keyTemp, key, purgeBytesCount);
        purgeDecrypt((uint64_t *) hashPtr, (uint64_t *) keyTemp); // decrypt hash
        memset(keyTemp, 0, purgeBytesCount);

        // get hash
        evasionHashData(textTemp + sizeof(uint64_t), size - evasionBytesCount - sizeof(uint64_t), (uint64_t *)hash);
        // check hash
        if(memcmp(hash, hashPtr, evasionBytesCount) == 0) {
            // get size
            memcpy((uint8_t*) &sizeOfText, textTemp, sizeof(uint64_t));
            plainText = RAlloc(sizeOfText);
            if(plainText != nil) {
                memcpy(plainText, textTemp + sizeof(uint64_t), sizeOfText);
                *encryptedSize = sizeOfText; // store
            }
        } else {
            RError("decryptPurgeEvasion. Hashes isn't equal. Can't decrypt data, must be bad key\n", text);
        }
        deallocator(textTemp);
    }
    return plainText;
}

#pragma GCC pop_options