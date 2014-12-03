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

#define RAY_WARNINGS_ON      1 // 451
#define RAY_ERRORS_ON        1 // 451
#define RAY_ASSERT_ON_ERRORS 1 //451, errors must be 1
#define RAY_SHORT_DEBUG      451 //

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#if RAY_ASSERT_ON_ERRORS == 1
    #include <assert.h>
#endif

// Hooks for malloc, free ---------------------------------

// constant pointers to stdlib (OS) functions
static void* (*const RTrueMalloc) (size_t size) = malloc;
static void* (*const RTrueRealloc)(void*  ptr, size_t size) = realloc;
static void* (*const RTrueCalloc) (size_t size, size_t blockSize) = calloc;
static void  (*const RTrueFree)   (void*  ptr) = free;

// pointers to functions
void*   (*RMallocPtr) (size_t size);
void*   (*RCallocPtr) (size_t size, size_t blockSize);
void*   (*RReallocPtr)(void*  ptr,  size_t size);
void    (*RFreePtr)   (void*  ptr);

// malloc entry point is pointer
#define malloc                RMallocPtr
#define realloc               RReallocPtr
#define calloc                RCallocPtr
#define free                  RFreePtr

#define initPointers() RMallocPtr  = RTrueMalloc;\
                       RFreePtr    = RTrueFree;\
                       RCallocPtr  = RTrueCalloc;\
                       RReallocPtr = RTrueRealloc
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
#define RStringLenght     strlen
#define RStringToUnsigned strtoul
#define RStringToInt      atoi
#define RStringToDouble   atof
#define RIntToString      itoa

#endif /*__RAY_BASE_H__*/