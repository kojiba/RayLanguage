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

#undef malloc
#undef realloc
#undef calloc
#undef free

void* (*const RTrueMalloc) (size_t size) = &malloc;
void* (*const RTrueRealloc)(void*  ptr, size_t size) = &realloc;
void* (*const RTrueCalloc) (size_t size, size_t blockSize) = &calloc;
void  (*const RTrueFree)   (void*  ptr) = &free;

void* volatile (*RMallocPtr) (size_t size) = &malloc;
void* volatile (*RCallocPtr) (size_t size, size_t blockSize) = &calloc;
void* volatile (*RReallocPtr)(void*  ptr,  size_t size) = &realloc;
void  volatile (*RFreePtr)   (void*  ptr) = &free;

#define malloc  RMallocPtr
#define realloc RReallocPtr
#define calloc  RCallocPtr
#define free    RFreePtr