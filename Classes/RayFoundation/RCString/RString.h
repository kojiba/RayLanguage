/**
 * RCString.h
 * Realization of wrapper on '\0' - terminated utf8 strings.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 04/23/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_STRING_H__
#define __R_STRING_H__

#include <RSyntax.h>
#include "RCString.h"

rbool utf8GetNextСharacter(const byte     *string,
                                 size_t    stringSize,
                                 size_t   *cursor,
                                 rbool    *isValid,
                                 unsigned *codePoint);

size_t utf8Length(byte *string, size_t sizeInBytes);

typedef RCString RString;

#endif /*__R_STRING_H__*/