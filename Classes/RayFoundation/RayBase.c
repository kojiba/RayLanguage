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
 
#include <RayBase.h>
#include <RThread.h>

#undef malloc
#undef realloc
#undef calloc
#undef free

// stdlib functions by default
volatile Allocator   RMallocPtr  = &malloc;
volatile Callocator  RCallocPtr  = &calloc;
volatile Reallocator RReallocPtr = &realloc;
volatile Deallocator RFreePtr    = &free;

#define malloc  RMallocPtr
#define realloc RReallocPtr
#define calloc  RCallocPtr
#define free    RFreePtr