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

// RBytes
typedef struct RData RBytes;

#define makeRBytes(size) makeRData(RAlloc(size), size, RDataTypeBytes)

extern
constructor (RBytes), size_t size);
destructor  (RBytes);
printer     (RBytes);

method(RBytes*,      flushAllToByte, RBytes),    byte symbol);
constMethod(RBytes*, copy,           RBytes));
method(RBytes*,      fromRCString,   RBytes),    RString *string); // not sets size, only copy bytes, returns self
method(RBytes*,      insertInBeginBytes,  RBytes), pointer data, size_t sizeInBytes);
method(RBytes*,      insertInBegin,  RBytes), RBytes *array);

RBytes* contentOfFile(const char *filename);

#define makeFlushedBytes(size, symbol) flushAllToByte(RAlloc(size), size, symbol)
#define RBfromRCS(string)              $(c(RBytes)(nil, (string)->size), m(fromRCString, RBytes)), (string))

#endif /*__R_BYTES__*/
