/**
 * RString_File.h
 * File i/o for strings.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 8/10/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_C_STRING_FILE_H__
#define __R_C_STRING_FILE_H__

#include "RString.h"

method(void, appendToFile, RString), const char *filename);
RString* stringFromFile(const char *filename);

/*
 * Example
 *
 *  RPrintLn("Input some:");
 *  RString *input = getInputString(); // all to '\n' symbol size is automatic
 *  p(RString)(input);
 *  deleter(input, RString);
 *
 *  Note: Reads string from stdin,
 *  returns created RString which must be destructed.
*/
RString * getInputString();

#endif /*__R_C_STRING_FILE_H__*/
