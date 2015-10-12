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

#include "RBase64.h"

#pragma mark Base

size_t base64decodeLength(const char *base64Buffer) {
    size_t numberOfBytesDecoded;
    size_t numberOfBytes;
    const byte *bufferPointer = (const byte *) base64Buffer;

    // increment pointer while <= 63
    while (decodingTableBase64[*(bufferPointer++)] <= 63);

    numberOfBytes = (bufferPointer - (const byte *) base64Buffer) - 1;
    numberOfBytesDecoded = ((numberOfBytes + 3) / 4) * 3;

    return numberOfBytesDecoded + 1;
}

inline size_t base64encodeLength(size_t length) {
    return ((length + 2) / 3 * 4) + 1;
}

size_t encodeBase64(char **destination, const char *data, size_t sizeInBytes) {
    *destination = RAlloc(base64encodeLength(sizeInBytes));
    if (*destination != nil) {
        size_t i;
        char *p = *destination;
        for (i = 0; i < sizeInBytes - 2; i += 3) {
            *p++ = encodingTableBase64[(data[i] >> 2) & 0x3F];
            *p++ = encodingTableBase64[((data[i] & 0x3) << 4)
                    | ((size_t) (data[i + 1] & 0xF0) >> 4)];
            *p++ = encodingTableBase64[((data[i + 1] & 0xF) << 2)
                    | ((size_t) (data[i + 2] & 0xC0) >> 6)];
            *p++ = encodingTableBase64[data[i + 2] & 0x3F];
        }
        if (i < sizeInBytes) {
            *p++ = encodingTableBase64[(data[i] >> 2) & 0x3F];
            if (i == (sizeInBytes - 1)) {
                *p++ = encodingTableBase64[((data[i] & 0x3) << 4)];
                *p++ = '=';
            }
            else {
                *p++ = encodingTableBase64[((data[i] & 0x3) << 4)
                        | ((size_t) (data[i + 1] & 0xF0) >> 4)];
                *p++ = encodingTableBase64[((data[i + 1] & 0xF) << 2)];
            }
            *p++ = '=';
        }

        *p++ = '\0';
        return p - *destination;

    } elseError(
            RError("Base64. encoding buffer allocation error.", nil));

    return 0;
}

size_t decodeBase64(pointer destination, const pointer encodedData) {
    size_t numberOfBytesDecoded;
    register const byte  *bufferIn;
    register       byte  *bufferOut;
    register       size_t numberOfBytes;

    bufferIn = (const byte *) encodedData;
    while (decodingTableBase64[*(bufferIn++)] <= 63);

    numberOfBytes        = (bufferIn - (const byte *) encodedData) - 1;
    numberOfBytesDecoded = ((numberOfBytes + 3) / 4) * 3;

    bufferOut = (byte *) destination;
    bufferIn  = (const byte *) encodedData;

    while (numberOfBytes > 4) {
        *(bufferOut++) =
                (byte) (decodingTableBase64[*bufferIn] << 2 | decodingTableBase64[bufferIn[1]] >> 4);
        *(bufferOut++) =
                (byte) (decodingTableBase64[bufferIn[1]] << 4 | decodingTableBase64[bufferIn[2]] >> 2);
        *(bufferOut++) =
                (byte) (decodingTableBase64[bufferIn[2]] << 6 | decodingTableBase64[bufferIn[3]]);
        bufferIn += 4;
        numberOfBytes -= 4;
    }

    if (numberOfBytes > 1) {
        *(bufferOut++) =
                (byte) (decodingTableBase64[*bufferIn] << 2 | decodingTableBase64[bufferIn[1]] >> 4);
    }
    if (numberOfBytes > 2) {
        *(bufferOut++) =
                (byte) (decodingTableBase64[bufferIn[1]] << 4 | decodingTableBase64[bufferIn[2]] >> 2);
    }
    if (numberOfBytes > 3) {
        *(bufferOut++) =
                (byte) (decodingTableBase64[bufferIn[2]] << 6 | decodingTableBase64[bufferIn[3]]);
    }

    *(bufferOut++) = '\0';
    numberOfBytesDecoded -= (4 - numberOfBytes) & 3;
    return numberOfBytesDecoded;
}

#pragma mark Additions

#pragma mark RCString

constMethod(RString*, encodeBase64, RString)) {
    RString *result = makeRCString();
    if(result != nil) {
        char  *string = nil;
        size_t length = encodeBase64(&string, object->baseString, object->size);
        if(string != nil) {
            result->size       = length - 1;
            result->baseString = string;
            return result;
        }

    } elseError(
            RError("RString. Base64 encode. Bad allocation of result struct.", result)
    );
    return nil;
}

constMethod(RString*, decodeBase64, RString)) {
    RString *result = makeRCString();
    if(result != nil) {
        size_t length = base64decodeLength(object->baseString);
        char  *string = RAlloc(length);
        if(string != nil) {
            length = decodeBase64(string, object->baseString);
            result->size       = length;
            result->baseString = string;
            return result;
        }
    } elseError(
            RError("RString. Base64 decode. Bad allocation of result struct.", result)
    );
    return nil;
}

constMethod(RBytes*, decodeBase64ToBytes, RString)) {
    size_t length = base64decodeLength(object->baseString);
    RBytes *result = makeRBytes(nil, length);
    if(result != nil) {
        char  *string = RAlloc(length);
        if(string != nil) {
            length = decodeBase64(string, object->baseString);
            result->size = length;
            result->data = (byte *) string;
            return result;
        }

    } elseError(
            RError("RString. Base64 decode. Bad allocation of result RBytes struct.", result)
    );
    return nil;
}

#pragma mark RBytes

constMethod(RString*, encodeBase64, RBytes)) {
    RString *result = makeRCString();
    if(result != nil) {
        char  *string = nil;
        size_t length = encodeBase64(&string, (char const *) object->data, object->size);
        if(string != nil) {
            result->size       = length - 1;
            result->baseString = string;
            return result;
        }

    } elseError(
            RError("RBytes. Base64 encode. Bad allocation of result RCString struct.", result) );
    return nil;
}

