/**
 * PurgeEvasionSocket.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 9/14/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RayFoundation/REncoding/purge.h"
#include "PurgeEvasionConnection.h"

struct PurgeEvasionConnectionContext {
    RArray   *usedRandoms;
    uint64_t *masterKey;     // 8 x 64-bit blocks
    uint64_t *connectionKey;
    uint64_t *currentRandom;
};

size_t dataSizeEncrypted(size_t sizeInBytes) {

}

PurgeEvasionConnectionContext* initContext(uint64_t masterKey[8]) {
    PurgeEvasionConnectionContext *result = allocator(PurgeEvasionConnectionContext);
    if(result != nil) {
        result->masterKey = arrayAllocator(uint64_t, 8);
        result->usedRandoms = makeRArray();
        result->currentRandom = arrayAllocator(uint64_t, 8);
        result->connectionKey = arrayAllocator(uint64_t, 8);
        result->usedRandoms->destructorDelegate = getRFree();
    }
    return result;
}

RByteArray* encryptDataWithConnectionContext(const RByteArray *data, PurgeEvasionConnectionContext* context) {
    size_t resultSize = data->size + purgeBytesCount; // data size for encryption + hash
    RByteArray *result = makeRByteArray(makeFlushedBytes(resultSize, 0xFF), resultSize);
}


