/**
 * PurgeEvasionParallel.c
 * Parallel encryption/decryption for
 * purgeEvasion using RThreadPool.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/8/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "PurgeEvasionParallel.h"
#include "RayFoundation/RThread/RThreadPool.h"
#include "RayFoundation/REncoding/PurgeEvasionUtils.h"

#ifndef RAY_EMBEDDED

typedef struct PrivatePEWokerData {
     uint8_t keyTemp[purgeBytesCount];
    uint64_t cipherCount;
     uint8_t *partStart;

} PrivatePEWokerData;

void privatePEEncryptPart(PrivatePEWokerData *worker) {
    uint64_t iterator;
     uint8_t keyTemp[purgeBytesCount];

    forAll(iterator, worker->cipherCount) {
        evasionRand((uint64_t *) worker->keyTemp);
        memcpy(keyTemp, worker->keyTemp, purgeBytesCount);
        purgeEncrypt((uint64_t *) (worker->partStart + iterator * purgeBytesCount), (uint64_t *) keyTemp);
    }

    deallocator(worker);
}

void* encryptPurgeEvasionParallel(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize, unsigned workers) {
    uint8_t *result = nil;
    uint64_t hash[8];
    uint64_t totalSize = size + sizeof(uint64_t);
     uint8_t workerKey[purgeBytesCount];
    uint64_t ciphersForWorker;
    uint64_t additionalForLastWorker;
    unsigned iterator;
    uint64_t keyIterator;
    uint64_t cipherCount = totalSize / purgeBytesCount;
    uint64_t addition    = totalSize % purgeBytesCount;

    if(addition != 0) {
        totalSize += purgeBytesCount - addition;
        ++cipherCount;
    }
    result = RAlloc(totalSize);

    if(result) {
        RThreadPool *pool = nil;
        *cryptedSize = 0;

        memcpy(result, &size, sizeof(uint64_t));       // add size in front
        memcpy(result + sizeof(uint64_t), text, size); // copy other text

        if (addition != 0) { // add some zeros if needed
            memset(result + size + sizeof(uint64_t), 0, purgeBytesCount - addition);
        }

        evasionHashData(result + sizeof(uint64_t), totalSize - sizeof(uint64_t), (uint64_t *)hash); // hash data with padding

               ciphersForWorker = cipherCount / workers;
        additionalForLastWorker = cipherCount - ciphersForWorker * workers;

        pool = c(RThreadPool)(nil);
        if(pool != nil) {
            $(pool, m(setDelegateFunction, RThreadPool)), (RThreadFunction) privatePEEncryptPart);

            forAll(iterator, workers) {
                PrivatePEWokerData *arg = allocator(PrivatePEWokerData);
                if(arg != nil){

                    // setup part key
                    if(iterator != 0) {
                        forAll(keyIterator, ciphersForWorker) {
                            evasionRand((uint64_t *) workerKey);
                        }
                    } else {
                        memcpy(workerKey, key, purgeBytesCount);
                    }

                    memcpy(arg->keyTemp, workerKey, purgeBytesCount);
                    arg->partStart = result + iterator * ciphersForWorker * purgeBytesCount;

                    if(iterator == workers - 1) {
                        arg->cipherCount = ciphersForWorker + additionalForLastWorker;
                    } else {
                        arg->cipherCount = ciphersForWorker;
                    }

                    $(pool, m(addWithArg, RThreadPool)), arg, no);

                } else {
                    RError("encryptPurgeEvasionParallel. Can't allocate workers argument.", pool);
                    deleter(pool, RThreadPool);
                    return nil;
                }
            }
            $(pool, m(join, RThreadPool))); // wait workers

            deleter(pool, RThreadPool);

            // add hash and encrypt last
            // setup last key for hash
            forAll(keyIterator, ciphersForWorker + additionalForLastWorker + 1) {
                evasionRand((uint64_t *) workerKey);
            }

            // crypt hash by last key
            purgeEncrypt(hash, (uint64_t *) workerKey); // encrypt hash
            memset(workerKey, 0, purgeBytesCount);

            // append hash
            result = RReAlloc(result, totalSize + evasionBytesCount);
            if(result != nil) {
                memcpy(result + totalSize, hash, evasionBytesCount); // append hash
                *cryptedSize = totalSize; // store
                *cryptedSize += evasionBytesCount;
            }  elseError(
                    RError("encryptPurgeEvasionParallel. Error realloc result for hash.", nil);
            )

        } elseError(
            RError("encryptPurgeEvasionParallel. Can't allocate thread pool.", nil);
        )
    } elseError(
            RError("encryptPurgeEvasionParallel. Can't allocate result array.", nil);
    )

    return result;
}

#endif /* RAY_EMBEDDED */