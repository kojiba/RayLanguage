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

void evasionHashData(const void *text, uint64_t size, uint64_t *outputHash); // outputHash - preallocated 512 bits (64 bytes, 8 x uint64 blocks)

/**
 * Data format is
 *
 *    +--------------------------------------------------------------+
 *    | dataSize (8 bytes) | text | evasionHash from text (64 bytes) |
 *    +--------------------------------------------------------------+
 *     \_____________________encrypted_section______________________/
 *      \                                                          /
 *       \__________output_encrypted_data_(size @cryptedSize)_____/
 *
 **/

void* encryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *cryptedSize);   // return value is nil or dynamic allocated
void* decryptPurgeEvasion(const void *text, uint64_t size, uint64_t key[8], uint64_t *encryptedSize); // return value is nil or dynamic allocated

#endif /*__PURGE_EVASION_UTILS_H__*/
