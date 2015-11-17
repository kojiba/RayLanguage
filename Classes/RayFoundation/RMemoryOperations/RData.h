/**
 * RData.h
 * Realization of some operations on bytes array, like getSubArray, etc.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

RData* makeRData(const byte *array, size_t size, byte type);
#define makeRDataBytes(array, size) makeRData(array, size, RDataTypeBytes)
#define makeRDataAllocated(size) makeRData(arrayAllocator(byte, size), size, RDataTypeBytes)

destructor(RData);
void RDataDeleter(RData* object);

extern printer(RData);

constMethod(void, printInFile,           RData), FILE *file);


     method(void,   insert,         RData),    const RData *data, size_t place);
     method(RData*, flushAllToByte, RData),    byte symbol);
constMethod(RData*, copy,           RData));

constMethod(RArray*, dataSeparatedByBytes,           RData), const RData *separatorsArray);
constMethod(RArray*, dataSeparatedByArray,           RData), const RData *separator);
constMethod(RArray*, dataSeparatedByArrayWithShield, RData), const RData *separator, const RData *shield);

constMethod(RCompareFlags, compareWith, RData),    const RData *checkData);
constMethod(rbool,         isEqualTo,   RData),    const RData *checkData);

RArray* DataSeparatedByBytes(const byte *array, size_t size, const byte *separatorsArray, size_t separatorsSize);

RData* contentOfFile(const char *filename);
RArray* DataSeparatedByBytes(const byte *array, size_t size, const byte *separatorsArray, size_t separatorsSize);
RArray* DataSeparatedByArrayWithShield(const byte *array, size_t size, const byte *separatorArray, size_t separatorSize, const byte *shield, size_t shieldSize);
extern
RArray* DataSeparatedByArray(const byte *array, size_t size, const byte *separatorArray, size_t separatorSize);

#endif /*__R_BYTES__*/
