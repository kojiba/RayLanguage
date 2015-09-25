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
#include "RayFoundation/RCString/RCString_Char.h"

const byte networkOperationErrorCryptConst = 2;

#pragma GCC push_options
#pragma GCC optimize ("O0")

struct PEConnectionContext {
    RArray   *packetNumbers;

    uint64_t *masterKey;     // 8 x 64-bit blocks

    uint64_t *connectionKey;
    uint64_t *currentRandom;
};

struct PEConnection {
    PEConnectionContext *connectionContext;
    RSocket             *socket;
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

            RMemCpy(result->masterKey, masterKey, purgeBytesCount);
            evasionHashData(masterKey, purgeBytesCount, masterKey);
            flushAllToByte(masterKey, purgeBytesCount, 0xFF);
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

        flushAllToByte(context->currentRandom, evasionBytesCount,  0xFF);

        forAll(index, packetNo) { // get random manyTimes from pure array
            evasionRand(context->currentRandom);
        }

        RMemCpy(context->connectionKey, context->masterKey, purgeBytesCount); // cpy key

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
            $(result, m(insertInBeginBytes, RByteArray)), &currentPacketNo, sizeof(uint64_t));

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
    RMemCpy(&currentPacketNo, data->array, sizeof(uint64_t));
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

PEConnection* PEConnectionInit(RSocket *socket, PEConnectionContext *context) {
    PEConnection *result = allocator(PEConnection);
    if(result != nil) {
        if(socket != nil && context != nil) {
            result->socket = socket;
            result->connectionContext = context;
        } else {

            // cleanups
            deallocator(socket);
            deallocator(context);
        }
    }
    return result;
}

destructor(PEConnection) {
    deleter(object->socket, RSocket);
    deleter(object->connectionContext, PEConnectionContext);
}

void PEConnectionPrepareBuffer(RByteArray *array) {
    static RCString fake;
    RCString *temp = &(RCString){0, (char *) cryptedMessageStart, 11};
    if(array != nil) {
        fake.baseString = (char *) array->array;
        fake.size = array->size;
        $(&fake, m(replaceCSubstrings, RCString)), "PEPacketBGN", "PEPacketBGNPEPacketBGN"); // shield strings
        $(&fake, m(replaceCSubstrings, RCString)), "PEPacketEND", "PEPacketENDPEPacketEND"); // shield strings
    }
    $(&fake, m(insertSubstringAt, RCString)), temp, 0);
    $(&fake, m(appendString, RCString)), "PEPacketEND"); // here add \0 in end
}

void PEConnectionRestoreBuffer(RByteArray *array) {
//    static RCString fake;
//    $(array, m(insertInBeginBytes, RByteArray)), (pointer) cryptedMessageStart, 10);
//    if(array != nil) {
//        fake.baseString = (char *) array->array;
//        fake.size = array->size;
//        $(&fake, m(replaceCSubstrings, RCString)), "PEPacketBGNPEPacketBGN", "PEPacketBGN"); // shield strings
//        $(&fake, m(replaceCSubstrings, RCString)), "PEPacketENDPEPacketEND", "PEPacketEND"); // shield strings
//    }
//    $(&fake, m(appendString, RCString)), "PEPacketEND"); // here add \0 in end
}


byte PEConnectionSend(PEConnection *object, RByteArray *toSend) {
    RByteArray *encrypted = encryptDataWithConnectionContext(toSend, object->connectionContext);
    byte result = networkOperationErrorCryptConst;
    if(encrypted != nil) {
        // prepare data
        PEConnectionPrepareBuffer(encrypted);
        result = $(object->socket, m(send, RSocket)), encrypted->array, encrypted->size - 1); // not send last \0
        deleter(encrypted, RByteArray);
    }
    return result;
}

inline byte PEConnectionSendBytes(PEConnection *object, const pointer buffer, size_t size) {
    byte result = networkOperationErrorCryptConst;
    RByteArray *temp = makeRByteArray(buffer, size);
    if(temp != nil) {
        result = PEConnectionSend(object, temp);
        deallocator(temp);
    }
    return result;
}

byte PEConnectionReceive(PEConnection *object, RByteArray* result) {
    // unprepare data

}

#pragma GCC pop_options