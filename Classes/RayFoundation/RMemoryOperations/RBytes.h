/**
 * RBytes.h
 * Realization of some operations on bytes array, like getSubArray, etc.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 10/7/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_BYTES__
#define __R_BYTES__

#include "RByteOperations.h"

typedef enum RDataType {
    RDataTypeBytes,
    RDataTypeASCII,
    RDataTypeUTF8,

    RDataTypeOther
} RDataType;

typedef struct RData { // basic sized data struct used for store bytes arrays and strings
    byte  *data;
    size_t size;
    byte   type;
} RData;

RData* makeRData(byte *array, size_t size, byte type);
#define makeRDataBytes(array, size) makeRData(array, size, RDataTypeBytes)

destructor(RData);
void RDataDeleter(RData* object);

method(RData*,      flushAllToByte, RData),    byte symbol);
constMethod(RData*, copy,           RData));

RData* contentOfFile(const char *filename);

#endif /*__R_BYTES__*/
