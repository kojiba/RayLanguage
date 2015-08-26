/**
 * PurgeEvasionUtils.h
 * Data cipher based on purge encryption
 * and evasion in mode pseudo-rand generator.
 * Data hash based on evasion
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 06/02/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __PURGE_EVASION_UTILS_H__
#define __PURGE_EVASION_UTILS_H__

#include "purge.h"
#include "evasion.h"

void* encryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize);   // return value is nil or dynamic allocated
void* decryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *encryptedSize); // return value is nil or dynamic allocated

void  evasionHashData(const void *text, uint64_t size, uint64_t *outputHash); // outputHash - 512 bits (64 bytes, 8 uint64 blocks)

#endif /*__PURGE_EVASION_UTILS_H__*/
