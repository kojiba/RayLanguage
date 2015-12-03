/**
 * PurgeEvasionIUtilsRay.h
 * Utils for work with cipher for libRay.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 8/26/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __PURGE_EVASION_UTILS_RAY__
#define __PURGE_EVASION_UTILS_RAY__

#include "RayFoundation/RString/RString_UTF8.h"
#include "RayFoundation/RMemoryOperations/RData.h"

// please, use long keys, at least 256 bit or keys evasion hashes
constMethod(RData *, encryptPurgeEvasion, RData), const RData *key); // Key size more than 512 bit, use first 512
constMethod(RData *, decryptPurgeEvasion, RData), const RData *key); // decrypted data or nil
constMethod(RData *, evasionHash,         RData));                   // evasionBytesCount data or nil

extern constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key); // result will be base64'zed
extern constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key); // Key size more than 512 bit, use first 512, firstly - deBase64, than decrypt
extern constMethod(RString *, evasionHashBase64,         RString));                     // evasion hash from string

#endif /*__PURGE_EVASION_UTILS_RAY__*/
