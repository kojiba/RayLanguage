/**
 * PurgeEvasionSocket.h
 * TCP connection with encryption
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

#ifndef __PURGE_EVASION_CONNECTION__
#define __PURGE_EVASION_CONNECTION__

#include "RayFoundation/RayConfig.h"

#include "RayFoundation/RMemoryOperations/RData.h"

#ifndef RAY_EMBEDDED
    #include "RayFoundation/RNetwork/RSocket.h"

    extern const byte networkOperationErrorCryptConst; // if can't encrypt or decrypt
    extern const byte networkOperationErrorAllocationConst;
#endif

typedef struct PEConnectionContext PEConnectionContext;
#pragma mark Context

PEConnectionContext* initPEContext(uint64_t masterKey[8]); // master-key will be flushed
destructor(PEConnectionContext);

RData* encryptDataWithConnectionContext(const RData *data, PEConnectionContext* context);
RData* decryptDataWithConnectionContext(      RData *data, PEConnectionContext* context);

#pragma mark Connection

#ifndef RAY_EMBEDDED
    typedef struct PEConnection        PEConnection;

    PEConnection* PEConnectionInit     (RSocket *socket,      PEConnectionContext *context);
    destructor   (PEConnection);

    // main
    byte          PEConnectionSend     (PEConnection *object, const RData *toSend);
    byte          PEConnectionReceive  (PEConnection *object, RData **result);

    // wrappers
    extern
    byte          PEConnectionSendBytes(PEConnection *object, const pointer buffer, size_t size);
    extern
    byte          PEConnectionSendString(PEConnection *object, const RString *string);
#endif /* RAY_EMBEDDED */

#endif /*__PURGE_EVASION_CONNECTION__*/
