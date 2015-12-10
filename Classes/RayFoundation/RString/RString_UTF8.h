/**
 * RString.h
 * Realization of wrapper on '\0' - terminated utf8 strings.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#ifndef __R_STRING_UTF8_H__
#define __R_STRING_UTF8_H__

#include "RString.h"

rbool utf8GetNextCharacter(const byte     *string,
                                 size_t    stringSize,
                                 size_t   *cursor,
                                 rbool    *isValid,
                                 unsigned *codePoint);

size_t utf8Length(byte *string, size_t sizeInBytes);

extern
method(size_t, length, RString));

/*
 * Example (print charaters with indexes)
 *
 * rbool characters(RString checker, size_t iterator) {
 *      printf("%lu - ", iterator);
 *      p(RString)(&checker);
 *      printf("\n");
 *      return yes;
 * }
 */
method(RFindResult, enumerate, RString), rbool (*enumerator)(RString string, size_t iterator));

#endif /*__R_STRING_UTF8_H__*/