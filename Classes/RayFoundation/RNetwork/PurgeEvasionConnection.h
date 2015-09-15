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

typedef struct PurgeEvasionConnectionContext PurgeEvasionConnectionContext;

PurgeEvasionConnectionContext* initContext(uint64_t masterKey[8]);
RByteArray* encryptDataWithConnectionContext(const RByteArray *data, PurgeEvasionConnectionContext* context);
RByteArray* decryptDataWithConnectionContext(const RByteArray *data, PurgeEvasionConnectionContext* context);


#endif /*__PURGE_EVASION_CONNECTION__*/
