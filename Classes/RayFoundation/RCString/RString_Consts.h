/**
 * RString_Consts.h
 *
 * Array for storing const RStrings on runtime.
 * Once allocated than reused. Use RS() macro.
 * Not need to be deallocated.
 *
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 04/28/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RString.h>
#include <RDictionary.h>

RDictionary * stringConstantsTable();
RCString *    constantString(char *string);

#define RS(cstring) constantString(cstring)