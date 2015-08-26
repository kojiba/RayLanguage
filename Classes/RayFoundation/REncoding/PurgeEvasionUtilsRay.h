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

constMethod(RString *, encryptPurgeEvasionBase64, RString), const RString *key); // Key size more than 512 bit, use first 512, result will be base64'zed
constMethod(RString *, decryptPurgeEvasionBase64, RString), const RString *key); // Key size more than 512 bit, use first 512, firstly - deBase64, than decrypt

#endif /*__PURGE_EVASION_UTILS_RAY__*/
