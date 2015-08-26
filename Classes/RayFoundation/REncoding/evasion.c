/**
 * evasion.h
 * 512-bit block pseudo-rand gen
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

#include "evasion.h"

#include <string.h>


#pragma GCC push_options
#pragma GCC optimize ("O0")

typedef uint8_t byte;

#define forAll(iterator, count) for(iterator = 0; iterator < (count); ++iterator)

// 64 circular shift (rotates)
#define rotateLeft(data,  shift) (((data) << shift) | ((data) >> (64 - shift)))

#define sawSwap(array) temp = array[7]; \
                       array[7] = array[2]; \
                       array[2] = array[5]; \
                       array[5] = array[0]; \
                       array[0] = temp; \
                           temp = 0


uint64_t substituteEvasion(uint64_t data, const byte *block) {
    union bytesTo64 {
        uint64_t number;
        byte bytes[8];
    };
    union bytesTo64 wrapper;

    wrapper.number = data;
    wrapper.bytes[0] = block[wrapper.bytes[0]];
    wrapper.bytes[1] = block[wrapper.bytes[1]];
    wrapper.bytes[2] = block[wrapper.bytes[2]];
    wrapper.bytes[3] = block[wrapper.bytes[3]];
    wrapper.bytes[4] = block[wrapper.bytes[4]];
    wrapper.bytes[5] = block[wrapper.bytes[5]];
    wrapper.bytes[6] = block[wrapper.bytes[6]];
    wrapper.bytes[7] = block[wrapper.bytes[7]];
    return wrapper.number;
}

void rotateBytesEvasion(byte *data, byte count) {
    if(count % evasionBytesCount) {
        byte diff = (byte) (evasionBytesCount - count);
        byte temp[evasionBytesCount];
        memcpy(temp, data, count);
        memcpy(data, data + count, diff);
        memcpy(data + diff, temp, count);
        memset(temp, 0, evasionBytesCount);
    }
}

void evasionRand(uint64_t data[8]) {
    byte iterator;
    uint64_t temp;
    forAll(iterator, evasionRoundsCount) {
        // amplification
        data[0] += evasionAmplificationConstants[0];
        data[1] ^= evasionAmplificationConstants[1];
        data[2] += evasionAmplificationConstants[2];
        data[3] ^= evasionAmplificationConstants[3];
        data[4] += evasionAmplificationConstants[4];
        data[5] ^= evasionAmplificationConstants[5];
        data[6] += evasionAmplificationConstants[6];
        data[7] ^= evasionAmplificationConstants[7];

        // round
        data[1] += data[0];
        data[3] += data[2];
        data[5] += data[4];
        data[7] += data[6];

        data[0] = rotateLeft(data[0], 7);
        data[2] ^= data[1];
        data[4] ^= data[3];
        data[6] ^= data[5];
        data[7] = rotateLeft(data[0], 13);

        // subs
        data[1] = substituteEvasion(data[1], evasionSubstitutionBlock);
        data[3] = substituteEvasion(data[3], evasionSubstitutionBlock);
        data[5] = substituteEvasion(data[5], evasionSubstitutionBlock);

        sawSwap(data);

        // rotate
        rotateBytesEvasion((byte *) data, (byte) (iterator + 1));
    }
}

#pragma GCC pop_options