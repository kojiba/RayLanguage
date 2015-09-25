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

#include "RayFoundation/RMemoryOperations/RByteOperations.h"
#include "RayFoundation/RNetwork/RSocket.h"

extern const byte networkOperationErrorCryptConst; // if can't encrypt or decrypt

typedef struct PEConnectionContext PEConnectionContext;
typedef struct PEConnection        PEConnection;

PEConnectionContext* initPEContext(uint64_t masterKey[8]); // masterkey will be flushed
destructor(PEConnectionContext);

/**
 * Data format is
 *
 *    +-----------------------------------------+
 *    | packetNo (8 bytes) |   encrypted part   |
 *    +-----------------------------------------+
 *
 **/

RByteArray* encryptDataWithConnectionContext(const RByteArray *data, PEConnectionContext* context);
RByteArray* decryptDataWithConnectionContext(      RByteArray *data, PEConnectionContext* context);

PEConnection* PEConnectionInit     (RSocket *socket,      PEConnectionContext *context);
destructor   (PEConnection);
byte          PEConnectionSend     (PEConnection *object, RByteArray *toSend);
extern byte   PEConnectionSendBytes(PEConnection *object, const pointer buffer, size_t size);
byte          PEConnectionReceive  (PEConnection *object, RByteArray *result);

#endif /*__PURGE_EVASION_CONNECTION__*/
