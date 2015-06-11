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