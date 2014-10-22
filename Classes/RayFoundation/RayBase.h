/**
 * RayBase.h
 * A ray of light in the realm of darkness.
 * Defines basic c-functions, to may re-format it if needed.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __RAY_BASE_H__
#define __RAY_BASE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// mem management
#define RAlloc          malloc
#define RFree           free
#define RReallock       realloc

// io
#define RPrintf         printf
#define RScanf          scanf

// files-io
#define RFOpen          fopen
#define RFClose         fclose
#define RFRead          fread
#define RFSeek          fseek
#define RFTell          ftell
#define RRewind         rewind

// mem operations
#define RMemCpy         memcpy
#define RMemMove        memmove

#define RStringLenght   strlen

#endif /*__RAY_BASE_H__*/