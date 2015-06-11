/**
 * RObjectField.h
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

#ifndef __R_OBJECT_FIELD__
#define __R_OBJECT_FIELD__

#include <RCString/RString.h>

typedef struct RObjectFieldDescriptor {
    RString *name;
    RString *type;
} RObjectFieldDescriptor;

RObjectFieldDescriptor fieldWithNameAndType(RString *name, RString *type);
constMethod(RCompareFlags, compareWith, RObjectFieldDescriptor), RObjectFieldDescriptor *checkPair);

typedef struct RObjectField {
    RObjectFieldDescriptor fieldDescriptor; // type and name
    RString               *value; // used only like byte array
} RObjectField;

RObjectField objectFieldWithNameTypeValue(RString *name, RString *type, RString *value);
RObjectField objectFieldWithDescriptorAndValue(RObjectFieldDescriptor descriptor, RString *value);

#endif /*__R_OBJECT_FIELD__*/
