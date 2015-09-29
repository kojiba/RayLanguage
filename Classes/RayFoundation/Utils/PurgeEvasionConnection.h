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

#ifndef RAY_EMBEDDED

#include "RayFoundation/RMemoryOperations/RByteOperations.h"
#include "RayFoundation/RNetwork/RSocket.h"

extern const byte networkOperationErrorCryptConst; // if can't encrypt or decrypt
extern const byte networkOperationErrorAllocationConst;

typedef struct PEConnectionContext PEConnectionContext;
typedef struct PEConnection        PEConnection;

#pragma mark Context

PEConnectionContext* initPEContext(uint64_t masterKey[8]); // masterkey will be flushed
destructor(PEConnectionContext);

RByteArray* encryptDataWithConnectionContext(const RByteArray *data, PEConnectionContext* context);
RByteArray* decryptDataWithConnectionContext(      RByteArray *data, PEConnectionContext* context);

#pragma mark Connection

PEConnection* PEConnectionInit     (RSocket *socket,      PEConnectionContext *context);
destructor   (PEConnection);

// main
byte          PEConnectionSend     (PEConnection *object, RByteArray *toSend);
byte          PEConnectionReceive  (PEConnection *object, RByteArray **result);

// wrappers
extern
byte          PEConnectionSendBytes(PEConnection *object, const pointer buffer, size_t size);
extern
byte          PEConnectionSendString(PEConnection *object, const RString *string);

#endif /* RAY_EMBEDDED */

#endif /*__PURGE_EVASION_CONNECTION__*/
