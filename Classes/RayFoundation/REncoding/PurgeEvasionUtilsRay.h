/**
 * PurgeEvasionIUtilsRay.h
 * Utils for work with cipher for libRay.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
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

#include "RayFoundation/RCString/RString.h"
#include "RayFoundation/RMemoryOperations/RBytes.h"

// use long keys, at least 256 bit or keys evasion hashes
constMethod(RBytes *, encryptPurgeEvasion, RBytes), const RBytes *key); // Key size more than 512 bit, use first 512
constMethod(RBytes *, decryptPurgeEvasion, RBytes), const RBytes *key);
constMethod(RBytes *, evasionHash,         RBytes));

extern constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key); // result will be base64'zed
extern constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key); // Key size more than 512 bit, use first 512, firstly - deBase64, than decrypt
extern constMethod(RString *, evasionHashBase64,         RString));                     // evasion hash from string

#endif /*__PURGE_EVASION_UTILS_RAY__*/
