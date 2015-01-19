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

#include <RayConfig.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Hooks for self-code malloc, calloc, realloc, free ----------

// constant pointers to stdlib (OS) functions
static void* (*const RTrueMalloc) (size_t size) = malloc;
static void* (*const RTrueRealloc)(void*  ptr, size_t size) = realloc;
static void* (*const RTrueCalloc) (size_t size, size_t blockSize) = calloc;
static void  (*const RTrueFree)   (void*  ptr) = free;

// pointers to functions
extern void*   (*RMallocPtr) (size_t size);
extern void*   (*RCallocPtr) (size_t size, size_t blockSize);
extern void*   (*RReallocPtr)(void*  ptr,  size_t size);
extern void    (*RFreePtr)   (void*  ptr);

// all entry points is pointers
#define malloc  RMallocPtr
#define realloc RReallocPtr
#define calloc  RCallocPtr
#define free    RFreePtr

#define initPointers() RMallocPtr  = RTrueMalloc;\
                       RFreePtr    = RTrueFree;\
                       RCallocPtr  = RTrueCalloc;\
                       RReallocPtr = RTrueRealloc

#define storePtrs() pointer (*oldMalloc) (size_t size) = RMallocPtr;\
                    pointer (*oldRealloc)(pointer ptr, size_t oldSize) = RReallocPtr;\
                    pointer (*oldCalloc) (size_t size, size_t blockSize) = RCallocPtr;\
                    void    (*oldFree)   (pointer ptr) = RFreePtr

#define backPtrs()  RMallocPtr = oldMalloc;\
                    RReallocPtr = oldRealloc;\
                    RCallocPtr = oldCalloc;\
                    RFreePtr = oldFree

//---------------------------------------------------------

// Memory management
#define RAlloc            malloc
#define RFree             free
#define RReAlloc          realloc
#define RClearAlloc       calloc

// I/O
#define RPrintf           printf
#define RScanf            scanf
#define RPError           perror

// Files I/O
#define RFOpen            fopen
#define RFPrintf          fprintf
#define RFClose           fclose
#define RFRead            fread
#define RFWrite           fwrite
#define RFSeek            fseek
#define RFTell            ftell
#define RRewind           rewind

#define RSocketOpen       socket

// Memory operations
#define RMemCpy           memcpy
#define RMemMove          memmove
#define RMemCmp           memcmp

// Basics for strings
#define RSPrintf          sprintf
#define RStringLength     strlen
#define RStringToUnsigned strtoul
#define RStringToInt      atoi
#define RStringToDouble   atof
#define RIntToString      itoa

#endif /*__RAY_BASE_H__*/