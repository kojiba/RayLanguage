/**
 * RObjectField.c
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

#include "RObjectField.h"

RObjectFieldDescriptor fieldWithNameAndType(RString *name, RString *type) {
    RObjectFieldDescriptor some;
    some.name = name;
    some.type = type;
    return some;
}

constMethod(RCompareFlags, compareWith, RObjectFieldDescriptor), RObjectFieldDescriptor *checkDescriptor) {
    if(($(object->name, m(compareWith, RCString)), checkDescriptor->name) == equals)
              && ($(object->type, m(compareWith, RCString)), checkDescriptor->type) == equals)) {
        return equals;
    } else {
        return not_equals;
    }
}

destructor(RObjectField) {
    deleter(object->value, RCString);
}

void RObjectFieldDeleter(RObjectField *object) {
    deleter(object, RObjectField);
}