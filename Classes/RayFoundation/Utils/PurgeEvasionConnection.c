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

#ifndef RAY_EMBEDDED

//#define PE_CONNECTION_VERBOSE_DEBUG

#include "RayFoundation/REncoding/purge.h"
#include "RayFoundation/REncoding/PurgeEvasionUtils.h"
#include "RayFoundation/REncoding/PurgeEvasionUtilsRay.h"
#include "RayFoundation/RCString/RCString_Char.h"
#include "RayFoundation/RContainers/RBuffer.h"

#define cmutex &object->mutex

#define TCP_MTU_SIZE 1500

const byte networkOperationErrorCryptConst = 2;
const byte networkOperationErrorAllocationConst = 3;

const char packetEndString[24]       = "PEPacketEND PEPacketEND ";
const char packetEndShieldString[48] = "PEPacketEND PEPacketEND PEPacketEND PEPacketEND ";

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
    RMutex               mutex;
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
            flushAllToByte(masterKey, purgeBytesCount, 0xFF);
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
void PEConnectionPrepareBuffer(RByteArray *array) {
    static RCString fake;
    if(array != nil) {
        fake.baseString = (char *) array->array;
        fake.size = array->size;
        $(&fake, m(replaceCSubstrings, RCString)), (char *) packetEndString, (char *) packetEndShieldString); // shield strings
    }
}

static inline
void PEConnectionRestoreBuffer(RByteArray *array) {
    static RCString fake;
    if(array != nil) {
        fake.baseString = (char *) array->array;
        fake.size = array->size;
        $(&fake, m(replaceCSubstrings, RCString)), (char *) packetEndShieldString, (char *) packetEndString); // shield strings
    }
}


byte PEConnectionSend(PEConnection *object, RByteArray *toSend) {
    RByteArray *encrypted;
    byte result = networkOperationErrorCryptConst;
    RMutexLock(cmutex);
    encrypted = encryptDataWithConnectionContext(toSend, object->connectionContext); // crypt
    if(encrypted != nil) {
        // send data
#ifdef PE_CONNECTION_VERBOSE_DEBUG
        RPrintf("Unprepared data\n");
        p(RByteArray)(encrypted);
#endif

        PEConnectionPrepareBuffer(encrypted);

#ifdef PE_CONNECTION_VERBOSE_DEBUG
        RPrintf("Prepared data\n");
        p(RByteArray)(encrypted);
#endif
        result = $(object->socket, m(send, RSocket)), encrypted->array, encrypted->size);
        if(result == networkOperationSuccessConst) {
            // send transmission end
            result = $(object->socket, m(send, RSocket)), (pointer const) packetEndString, sizeof(packetEndString));
        }
        deleter(encrypted, RByteArray);
    }
    RMutexUnlock(cmutex);
    return result;
}

byte PEConnectionReceive(PEConnection *object, RByteArray** result) {
    byte buffer[TCP_MTU_SIZE];
    size_t received;
    RBuffer *storage = nil;
    RByteArray temp;
    byte resultFlag = networkOperationSuccessConst;

    RMutexLock(cmutex);
    while(resultFlag != networkConnectionClosedConst
          && resultFlag != networkOperationErrorConst) {
        resultFlag = $(object->socket, m(receive, RSocket)), buffer, TCP_MTU_SIZE, &received);

        if(resultFlag == networkOperationSuccessConst) {
            if(received >= sizeof(packetEndString)
               && !isMemEqual(buffer, packetEndString, sizeof(packetEndString))) {

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
            } else {
                break;
            }
        }

    }

    temp.array = storage->masterRByteArrayObject->array; // makes better
    temp.size  = storage->totalPlaced;

#ifdef PE_CONNECTION_VERBOSE_DEBUG
    RPrintf("Unrestored data\n");
    p(RByteArray)(&temp);
#endif

    // restore
    PEConnectionRestoreBuffer(&temp);

#ifdef PE_CONNECTION_VERBOSE_DEBUG
    RPrintf("Restored buffer\n");
    p(RByteArray)(&temp);
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
    RByteArray *temp = makeRByteArray(buffer, size);
    if(temp != nil) {
        result = PEConnectionSend(object, temp);
        deallocator(temp);
    }
    return result;
}

byte PEConnectionSendString(PEConnection *object, const RString *string) {
    return PEConnectionSendBytes(object, string->baseString, string->size);
}

#pragma GCC pop_options

#endif /* RAY_EMBEDDED */