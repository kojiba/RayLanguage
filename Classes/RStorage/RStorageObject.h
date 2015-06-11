/**
 * RStorageObject.h
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


#ifndef __R_STORAGE_OBJECT__
#define __R_STORAGE_OBJECT__

#include <RContainers/RArray.h>
#include <RCString/RString.h>

typedef struct RStorageObject {
    RArray  *fields; // RObjectField type
    RString *name;
} RStorageObject;

constructor(RStorageObject));
destructor(RStorageObject);
void RStorageObjectDeleter(RStorageObject *object);

RStorageObject * deserializeFromStringRStorageObject(RString *string);
method(RString *, serializeToString, RStorageObject));

#endif /*__R_STORAGE_OBJECT__*/
