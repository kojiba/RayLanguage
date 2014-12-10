

/**
 * RayBase.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/10/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/
 
#include "RayBase.h"

void*   (*RMallocPtr) (size_t size);
void*   (*RCallocPtr) (size_t size, size_t blockSize);
void*   (*RReallocPtr)(void*  ptr,  size_t size);
void    (*RFreePtr)   (void*  ptr);