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

#include <RayFoundation/RString/RString_Consts.h>
#include "PurgeEvasionConnection.h"


//#define PE_CONNECTION_VERBOSE_DEBUG

#include "RayFoundation/REncoding/purge.h"
#include "RayFoundation/REncoding/PurgeEvasionUtils.h"
#include "RayFoundation/REncoding/PurgeEvasionUtilsRay.h"

#ifndef RAY_EMBEDDED
    #include "RayFoundation/RString/RString_Char.h"
    #include "RayFoundation/RContainers/RBuffer.h"
#endif

#pragma GCC push_options
#pragma GCC optimize ("O0")

struct PEConnectionContext {
    RArray   *packetNumbers;

    uint64_t *masterKey;     // 8 x 64-bit blocks

    uint64_t *connectionKey;
    uint64_t *currentRandom;
};

PEConnectionContext* initPEContext(uint64_t masterKey[8]) {
    PEConnectionContext *object = allocator(PEConnectionContext);
    if(object != nil) {
        object->masterKey      = arrayAllocator(uint64_t, 8);
        object->packetNumbers  = makeRArray();
        object->currentRandom  = arrayAllocator(uint64_t, 8);
        object->connectionKey  = arrayAllocator(uint64_t, 8);

        if(object->masterKey  != nil
            && object->packetNumbers != nil
            && object->currentRandom != nil
            && object->connectionKey != nil) {

            RMemCpy(object->masterKey, masterKey, purgeBytesCount);
            evasionHashData(masterKey, purgeBytesCount, masterKey);
            flushAllToByte((byte *) masterKey, purgeBytesCount, 0xFF);
        }
    }
    return object;
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
    size_t index = $(context->packetNumbers, m(indexOfObject, RArray)), (pointer) packetNo); // todo not only for x64
    if(index == context->packetNumbers->count) { // not find

        flushAllToByte((byte *) context->currentRandom, evasionBytesCount, 0xFF);

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
        $(context->packetNumbers, m(addObject, RArray)), (pointer) packetNo); // add packetNo
        return context->connectionKey;
    }
    return nil;
}

/**
 * Data format is
 *
 *    +-----------------------------------------+
 *    | packetNo (8 bytes) |   encrypted part   |
 *    +-----------------------------------------+
 *
 **/

RData* encryptDataWithConnectionContext(const RData *data, PEConnectionContext* context) {
    uint64_t currentPacketNo = context->packetNumbers->count;
    PESessionPacketKey(context, currentPacketNo);
    if(context->connectionKey != nil) {
        RData *tempKey = makeRDataBytes((byte *) context->connectionKey, purgeBytesCount);
        if(tempKey != nil) {
            RData *result = nil;

            // encrypt
            result = $(data, m(encryptPurgeEvasion, RData)), tempKey);

            // add packetNo stamp in front
            insertSubArray(result->data, &result->size, (const byte *) &currentPacketNo, sizeof(uint64_t), 0);

            // add size
            result->size += sizeof(uint64_t);

            // cleanup
            deallocator(tempKey);
            return result;
        }
    }
    return nil;
}

RData* decryptDataWithConnectionContext(RData *data, PEConnectionContext* context) {
    uint64_t currentPacketNo;
    RMemCpy(&currentPacketNo, data->data, sizeof(uint64_t));
    PESessionPacketKey(context, currentPacketNo);
    if(context->connectionKey != nil) {
        RData *tempKey = makeRDataBytes((byte *) context->connectionKey, purgeBytesCount);
        if(tempKey != nil) {
            RData *result = nil;

            // remove packetNo
            data->data += sizeof(uint64_t);
            data->size -= sizeof(uint64_t);

            // decrypt
            result = $(data, m(decryptPurgeEvasion, RData)), tempKey);

            // cleanup
            deallocator(tempKey);
            // revert
            data->data -= sizeof(uint64_t);
            data->size += sizeof(uint64_t);

            return result;
        }
    }
    return nil;
}

#ifndef RAY_EMBEDDED

struct PEConnection {
    PEConnectionContext *connectionContext;
    RSocket             *socket;
    RMutex               mutex;
};

#define cmutex &object->mutex

#define TCP_MTU_SIZE 1500

const byte networkOperationErrorCryptConst = 2;
const byte networkOperationErrorAllocationConst = 3;

const char packetEndString[24]       = "PEPacketEND PEPacketEND ";
const char packetEndShieldString[48] = "PEPacketEND PEPacketEND PEPacketEND PEPacketEND ";

PEConnection* PEConnectionInit(RSocket *socket, PEConnectionContext *context) {
    PEConnection *object = allocator(PEConnection);
    if(object != nil) {
        if(socket != nil && context != nil) {
            object->socket = socket;
            object->connectionContext = context;
            mutexWithType(cmutex, RMutexNormal);
        } else {

            // cleanups
            deallocator(socket);
            deallocator(context);
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(PEConnection) {
    RMutexLock(cmutex);
    deleter(object->socket, RSocket);
    deleter(object->connectionContext, PEConnectionContext);
    RMutexUnlock(cmutex);
    RMutexDestroy(cmutex);
}

static inline
void PEConnectionPrepareBuffer(RData *array) {
    static RString fake;
    if(array != nil) {
        fake.data = array->data;
        fake.size = array->size;
        $(&fake, m(replaceCSubstrings, RString)), (char *) packetEndString, (char *) packetEndShieldString); // shield strings
    }
}

static inline
void PEConnectionRestoreBuffer(RData *array) {
    static RString fake;
    if(array != nil) {
        fake.data = array->data;
        fake.size = array->size;
        $(&fake, m(replaceCSubstrings, RString)), (char *) packetEndShieldString, (char *) packetEndString); // shield strings
    }
}


byte PEConnectionSend(PEConnection *object, RData *toSend) {
    RData *encrypted;
    byte result = networkOperationErrorCryptConst;
    RMutexLock(cmutex);
    encrypted = encryptDataWithConnectionContext(toSend, object->connectionContext); // crypt
    if(encrypted != nil) {
        // send data
#ifdef PE_CONNECTION_VERBOSE_DEBUG
        RPrintf("Unprepared data\n");
        p(RData)(encrypted);
#endif

        PEConnectionPrepareBuffer(encrypted);

#ifdef PE_CONNECTION_VERBOSE_DEBUG
        RPrintf("Prepared data\n");
        p(RData)(encrypted);
#endif
        result = $(object->socket, m(send, RSocket)), encrypted->data, encrypted->size);
        if(result == networkOperationSuccessConst) {
            // send transmission end
            result = $(object->socket, m(send, RSocket)), (pointer const) packetEndString, sizeof(packetEndString));
        }
        deleter(encrypted, RData);
    }
    RMutexUnlock(cmutex);
    return result;
}

byte PEConnectionReceive(PEConnection *object, RData** result) {
    byte buffer[TCP_MTU_SIZE];
    size_t received;
    RBuffer *storage = nil;
    RData temp;
    byte resultFlag = networkOperationSuccessConst;

    RMutexLock(cmutex);
    while(resultFlag != networkConnectionClosedConst
          && resultFlag != networkOperationErrorConst) {
        resultFlag = $(object->socket, m(receive, RSocket)), buffer, TCP_MTU_SIZE, &received);

#ifdef PE_CONNECTION_VERBOSE_DEBUG
        RPrintf("Received some data\n");
        printByteArrayInHex(buffer, received);
#endif

        if(resultFlag == networkOperationSuccessConst) {

                if(storage == nil) {
                    storage = c(RBuffer)(nil);
                }

                if(storage != nil) {
                    $(storage, m(addData, RBuffer)), buffer, received);
                } else {
                    RError("PEConnectionReceive. Can't allocate buffer.", object);
                    RMutexUnlock(cmutex);
                    return networkOperationErrorAllocationConst;
                }
//            if(isContainsSubBytes(buffer, received, (byte *) packetEndString, 24)) {
//                break;
//            }
        }

    }

    temp.data = storage->masterRDataObject->data; // makes better
    temp.size = storage->totalPlaced;

#ifdef PE_CONNECTION_VERBOSE_DEBUG
    RPrintf("Unrestored data\n");
    p(RData)(&temp);
#endif

    // restore
    PEConnectionRestoreBuffer(&temp);

#ifdef PE_CONNECTION_VERBOSE_DEBUG
    RPrintf("Restored buffer\n");
    p(RData)(&temp);
#endif

    // decrypt
    *result = decryptDataWithConnectionContext(&temp, object->connectionContext);
    if(*result == nil) {
        resultFlag = networkOperationErrorCryptConst;
    }
    // cleanup
    deleter(storage, RBuffer);
    RMutexUnlock(cmutex);
    return resultFlag;
}

#pragma mark Wrappers

inline byte PEConnectionSendBytes(PEConnection *object, const pointer buffer, size_t size) {
    byte result = networkOperationErrorCryptConst;
    RData *temp = makeRDataBytes(buffer, size);
    if(temp != nil) {
        result = PEConnectionSend(object, temp);
        deallocator(temp);
    }
    return result;
}

byte PEConnectionSendString(PEConnection *object, const RString *string) {
    return PEConnectionSendBytes(object, string->data, string->size);
}

#endif /* RAY_EMBEDDED */

#pragma GCC pop_options