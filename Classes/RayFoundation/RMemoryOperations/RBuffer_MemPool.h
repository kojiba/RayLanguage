/**
 * RBuffer_MemPool.h
 * Auto-release pool analog for C.
 * (Mem pool additions to RBuffer)
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 11/28/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/
 
#ifndef __R_MEM_POOL_H__
#define __R_MEM_POOL_H__

#include "../RSyntax.h"
#include "../RBasics/RBasics.h"
#include "../RContainers/RBuffer.h"

method(pointer, malloc, RBuffer), size_t size);
method(pointer, calloc, RBuffer), size_t size, size_t blockSize);
method(pointer, realloc, RBuffer), pointer ptr, size_t newSize);
method(void,    free, RBuffer), pointer ptr);


#endif /*__R_MEM_POOL_H__*/
