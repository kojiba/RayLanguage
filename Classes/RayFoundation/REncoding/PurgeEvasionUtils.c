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

#include "PurgeEvasionUtils.h"
#include "RayFoundation/RSyntax.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

#define nil ((void*)0)
#define forAll(iterator, count) for(iterator = 0; iterator < (count); ++iterator)
#define xor(first, second)

void* encryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize) { // key changed and data not
    uint64_t  iterator;
    uint8_t  *textTemp     = nil;
    uint64_t  totalSize   = size + sizeof(uint64_t);
    uint64_t  cipherCount = totalSize / purgeBytesCount;
    uint64_t  addition    = totalSize % purgeBytesCount;

    uint8_t keyTemp[purgeBytesCount];

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

        forAll(iterator, cipherCount) {
            evasionRand(key);
            memcpy(keyTemp, key, purgeBytesCount);
            purgeEncrypt((uint64_t *) (textTemp + iterator * purgeBytesCount), (uint64_t*) keyTemp);
            memset(keyTemp, 0, purgeBytesCount);
        }
        *cryptedSize = totalSize; // store
    }
    return textTemp;
}

void* decryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *encryptedSize) { // key changed and data not
    uint64_t iterator;
    uint8_t *textTemp    = nil,
            *plainText   = nil;
    uint64_t cipherCount = size / purgeBytesCount;
    uint64_t sizeOfText;
    uint8_t keyTemp[purgeBytesCount];

    if(size % purgeBytesCount) {
        perror("Bad data size. Must be multiple of 64. Data size in bytes\n");
        return nil;
    }

    textTemp = RAlloc(size);

    if(textTemp != nil) {
        *encryptedSize = 0;
        memcpy(textTemp, text, size); // add size in front

        forAll(iterator, cipherCount) {
            evasionRand(key);
            memcpy(keyTemp, key, purgeBytesCount);
            purgeDecrypt((uint64_t *) (textTemp + iterator * purgeBytesCount), (uint64_t *) keyTemp);
            memset(keyTemp, 0, purgeBytesCount);
        }

        // get size
        memcpy((uint8_t*) &sizeOfText, textTemp, sizeof(uint64_t));
        if(sizeOfText > MAX_SIZE_FOR_DECRYPT) {
            RError1("decryptPurgeEvasion. Can't decrypt, must be bad size %lu", text, sizeOfText);
            return nil;
        }
        plainText = RAlloc(sizeOfText);
        if(plainText != nil) {
            memcpy(plainText, textTemp + sizeof(uint64_t), sizeOfText);
            *encryptedSize = sizeOfText; // store
        }
        deallocator(textTemp);
    }
    return plainText;
}

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

#pragma GCC pop_options