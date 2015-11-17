/**
 * RString_Consts.h
 *
 * Array for storing const RStrings on runtime.
 * Once allocated than reused. Use RS() macro.
 * Not need to be deallocated.
 *
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RString_UTF8.h"
#include "RayFoundation/RContainers/RDictionary.h"

RDictionary * stringConstantsTable();
const RString * constantRString(const char *string);

#define RS(cstring) constantRString(cstring)