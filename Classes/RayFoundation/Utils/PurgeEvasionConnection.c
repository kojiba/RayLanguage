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

#include "PurgeEvasionConnection.h"
#include "RayFoundation/REncoding/purge.h"
#include "RayFoundation/REncoding/PurgeEvasionUtils.h"
#include "RayFoundation/REncoding/PurgeEvasionUtilsRay.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

struct PEConnectionContext {
    RArray   *packetNumbers;

    uint64_t *masterKey;     // 8 x 64-bit blocks

    uint64_t *connectionKey;
    uint64_t *currentRandom;
};

PEConnectionContext* initPEContext(uint64_t masterKey[8]) {
    PEConnectionContext *result = allocator(PEConnectionContext);
    if(result != nil) {
        result->masterKey      = arrayAllocator(uint64_t, 8);
        result->packetNumbers  = makeRArray();
        result->currentRandom  = arrayAllocator(uint64_t, 8);
        result->connectionKey  = arrayAllocator(uint64_t, 8);

        if(result->masterKey  != nil
            && result->packetNumbers != nil
            && result->currentRandom != nil
            && result->connectionKey != nil) {

            memcpy(result->masterKey, masterKey, purgeBytesCount);
            evasionHashData(masterKey, purgeBytesCount, masterKey);
            memset(masterKey, 0xFF, purgeBytesCount);
        }
    }
    return result;
}

destructor(PEConnectionContext) {

    evasionHashData(object->masterKey,     purgeBytesCount, object->masterKey);
    evasionHashData(object->connectionKey, purgeBytesCount, object->connectionKey);
    evasionHashData(object->currentRandom, purgeBytesCount, object->currentRandom);

    deallocator(object->masterKey);
    deallocator(object->connectionKey);
    deallocator(object->currentRandom);
    deleter(object->packetNumbers, RArray);

    object->masterKey     = nil;
    object->connectionKey = nil;
    object->currentRandom = nil;
}

uint64_t* PESessionPacketKey(PEConnectionContext* context, uint64_t packetNo) { // uint64 x 8
    size_t index = $(context->packetNumbers, m(indexOfObject, RArray)), packetNo); // todo not only for x64
    if(index == context->packetNumbers->count) { // not find

        memset(context->currentRandom, 0xFF, evasionBytesCount);

        forAll(index, packetNo) { // get random manyTimes from pure array
            evasionRand(context->currentRandom);
        }

        memcpy(context->connectionKey, context->masterKey, purgeBytesCount); // cpy key

        context->connectionKey[0] ^= context->currentRandom[0]; // add salt
        context->connectionKey[1] ^= context->currentRandom[1];
        context->connectionKey[2] ^= context->currentRandom[2];
        context->connectionKey[3] ^= context->currentRandom[3];
        context->connectionKey[4] ^= context->currentRandom[4];
        context->connectionKey[5] ^= context->currentRandom[5];
        context->connectionKey[6] ^= context->currentRandom[6];
        context->connectionKey[7] ^= context->currentRandom[7];

        evasionHashData(context->connectionKey, purgeBytesCount, context->connectionKey); // hashing
        $(context->packetNumbers, m(addObject, RArray)), packetNo); // add packetNo
        return context->connectionKey;
    }
    return nil;
}


RByteArray* encryptDataWithConnectionContext(const RByteArray *data, PEConnectionContext* context) {
    uint64_t currentPacketNo = context->packetNumbers->count;
    PESessionPacketKey(context, currentPacketNo);
    if(context->connectionKey != nil) {
        RByteArray *tempKey = makeRByteArray((byte *) context->connectionKey, purgeBytesCount);
        if(tempKey != nil) {
            RByteArray *result = nil;
            byte *hashPtr;

            // encrypt
            result = $(data, m(encryptPurgeEvasion, RByteArray)), tempKey);

            // add packetNo stamp in front
            result->array = RReAlloc(result->array, arraySize(byte, result->size + sizeof(uint64_t)));
            memcpy(result->array + sizeof(uint64_t), result->array, result->size);
            memcpy(result->array, &currentPacketNo, sizeof(uint64_t));

            // add size
            result->size += sizeof(uint64_t);

            // hash packetKey
            evasionHashData(context->connectionKey, evasionBytesCount, context->connectionKey);

            // hide hash, for equals messages
            hashPtr = result->array + result->size - evasionBytesCount;
            purgeEncrypt((uint64_t *) hashPtr, context->connectionKey);

            // cleanup
            deallocator(tempKey);
            return result;
        }
    }
    return nil;
}

RByteArray* decryptDataWithConnectionContext(RByteArray *data, PEConnectionContext* context) {
    uint64_t currentPacketNo;
    memcpy(&currentPacketNo, data->array, sizeof(uint64_t));
    PESessionPacketKey(context, currentPacketNo);
    if(context->connectionKey != nil) {
        RByteArray *tempKey = makeRByteArray((byte *) context->connectionKey, purgeBytesCount);
        if(tempKey != nil) {
            RByteArray *result = nil;
            uint64_t keyHash[8];
            byte *hashPtr;

            // remove packetNo
            data->array += sizeof(uint64_t);
            data->size -= sizeof(uint64_t);

            // hash packetKey
            evasionHashData(context->connectionKey, evasionBytesCount, keyHash);

            // get hash
            hashPtr = data->array + data->size - evasionBytesCount;
            purgeDecrypt((uint64_t *) hashPtr, keyHash);

            // decrypt
            result = $(data, m(decryptPurgeEvasion, RByteArray)), tempKey);

            // cleanup
            deallocator(tempKey);
            // revert
            data->array -= sizeof(uint64_t);
            data->size += sizeof(uint64_t);

            return result;
        }
    }
    return nil;
}

#pragma GCC pop_options