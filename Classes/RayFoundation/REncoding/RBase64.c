/**
 * RBase64.c
 * Base64 encoding and decoding for bytes buffers.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/3/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <Foundation/Foundation.h>
#include "RBase64.h"

#pragma mark Base

size_t base64decodeLength(const char *base64Buffer) {
    size_t numberOfBytesDecoded;
    size_t numberOfBytes;
    const byte *bufferPointer = (const byte *) base64Buffer;

    // increment pointer while <= 63
    while (decodingTable64[*(bufferPointer++)] <= 63);

    numberOfBytes = (bufferPointer - (const byte *) base64Buffer) - 1;
    numberOfBytesDecoded = ((numberOfBytes + 3) / 4) * 3;

    return numberOfBytesDecoded + 1;
}

size_t base64encodeLength(size_t len) {
    return ((len + 2) / 3 * 4) + 1;
}

pointer encodeBase64(pointer data, size_t sizeInBytes, size_t *outputSize) {
    // set output size
    *outputSize = 4 * ((sizeInBytes + 2) / 3);
    char *encodedData = RAlloc(*outputSize);
    if (encodedData != nil) {
        // encode if allocation successful
        for (size_t i = 0, j = 0; i < sizeInBytes;) {
            byte     octet_a = i < sizeInBytes ? ((byte *) data)[++i] : (byte)0x0;
            byte     octet_b = i < sizeInBytes ? ((byte *) data)[++i] : (byte)0x0;
            byte     octet_c = i < sizeInBytes ? ((byte *) data)[++i] : (byte)0x0;
            uint32_t triple  = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

            encodedData[++j] = encodingTable64[(triple >> 3 * 6) & 0x3F];
            encodedData[++j] = encodingTable64[(triple >> 2 * 6) & 0x3F];
            encodedData[++j] = encodingTable64[(triple >> 1 * 6) & 0x3F];
            encodedData[++j] = encodingTable64[(triple >> 0 * 6) & 0x3F];
        }

        for (int i = 0; i < modsTable[sizeInBytes % 3]; ++i) {
            encodedData[*outputSize - 1 - i] = '=';
        }
        return encodedData;
    } else {
        RError("Base64. encoding buffer allocation error.", (pointer) encodedData);
    }
    return nil;
}

size_t decodeBase64(pointer destination, pointer encodedData) {
    size_t numberOfBytesDecoded;
    register const byte *bufferIn;
    register       byte *bufferOut;
    register       size_t numberOfBytes;

    bufferIn = (const byte *) encodedData;
    while (decodingTable64[*(bufferIn++)] <= 63);

    numberOfBytes        = (bufferIn - (const byte *) encodedData) - 1;
    numberOfBytesDecoded = ((numberOfBytes + 3) / 4) * 3;

    bufferOut = (byte *) destination;
    bufferIn  = (const byte *) encodedData;

    while (numberOfBytes > 4) {
        *(bufferOut++) =
                (byte) (decodingTable64[*bufferIn] << 2 | decodingTable64[bufferIn[1]] >> 4);
        *(bufferOut++) =
                (byte) (decodingTable64[bufferIn[1]] << 4 | decodingTable64[bufferIn[2]] >> 2);
        *(bufferOut++) =
                (byte) (decodingTable64[bufferIn[2]] << 6 | decodingTable64[bufferIn[3]]);
        bufferIn += 4;
        numberOfBytes -= 4;
    }

    if (numberOfBytes > 1) {
        *(bufferOut++) =
                (byte) (decodingTable64[*bufferIn] << 2 | decodingTable64[bufferIn[1]] >> 4);
    }
    if (numberOfBytes > 2) {
        *(bufferOut++) =
                (byte) (decodingTable64[bufferIn[1]] << 4 | decodingTable64[bufferIn[2]] >> 2);
    }
    if (numberOfBytes > 3) {
        *(bufferOut++) =
                (byte) (decodingTable64[bufferIn[2]] << 6 | decodingTable64[bufferIn[3]]);
    }

    *(bufferOut++) = '\0';
    numberOfBytesDecoded -= (4 - numberOfBytes) & 3;
    return numberOfBytesDecoded;
}

#pragma mark Additions

method(inline RCString*, encodeBase64, RCString)) {
    RCString *result = makeRCString();
    if(result != nil) {
        size_t length;
        char  *string = encodeBase64(object->baseString, object->size, &length);
        if(string != nil) {
            result->size       = length;
            result->baseString = string;
            return result;
        }
    } else {
        RError("RCString. Base64 encode. Bad allocation of result struct.", result);
    }
    return nil;
}

method(inline RCString*, decodeBase64, RCString)) {
    RCString *result = makeRCString();
    if(result != nil) {
        size_t length = base64decodeLength(object->baseString);
        char  *string = RAlloc(length);
        if(string != nil) {
            decodeBase64(string, object->baseString);
            result->size       = length;
            result->baseString = string;
            return result;
        }
    } else {
        RError("RCString. Base64 decode. Bad allocation of result struct.", result);
    }
    return nil;
}


