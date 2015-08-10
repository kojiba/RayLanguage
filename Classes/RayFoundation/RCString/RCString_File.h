/**
 * RCString_File.h
 * File i/o for strings.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
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

#include "RCString.h"

method(void, appendToFile, RCString), const char *filename);
RCString* stringFromFile(const char *filename);

/*
 * Example
 *
 *  RPrintLn("Input some:");
 *  RCString *input = getInputString(); // all to '\n' symbol size is automatic
 *  p(RCString)(input);
 *  deleter(input, RCString);
 *
 *  Note: Reads string from stdin,
 *  returns created RCString which must be destructed.
*/
RCString * getInputString();

#endif /*__R_C_STRING_FILE_H__*/
