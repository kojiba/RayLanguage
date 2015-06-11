/**
 * RStoragePool.h
 * Pool of objects for RStorage.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 6/11/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/


#ifndef __R_STORAGE_POOL_H__
#define __R_STORAGE_POOL_H__

#include <RContainers/RArray.h>
#include "RObjectField.h"

typedef struct RPoolStructure {
    RArray *fields;   // typeof RObjectFieldDescriptor
    rbool   isStrict;
} RPoolStructure;

typedef struct RStoragePool {
    RArray *pools;
    RArray *objects;

    RPoolStructure structure;
} RStoragePool;

#endif /*__R_STORAGE_POOL_H__*/
