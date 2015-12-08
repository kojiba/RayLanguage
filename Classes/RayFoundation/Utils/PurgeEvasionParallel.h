/**
 * PurgeEvasionParallel.h
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

#ifndef __PURGE_EVASION_PARALLEL__
#define __PURGE_EVASION_PARALLEL__

#ifndef RAY_EMBEDDED

#include "RayFoundation/REncoding/PurgeEvasionUtilsRay.h"

void* encryptPurgeEvasionParallel(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize, unsigned workers); // key and data not changed

#endif /* RAY_EMBEDDED */
#endif /*__PURGE_EVASION_PARALLEL__*/
