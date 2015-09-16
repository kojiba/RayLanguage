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

#include "RayConfig.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Hooks for self-code malloc, calloc, realloc, free ----------

typedef void* (*Allocator)  (size_t);
typedef void* (*Callocator) (size_t, size_t);
typedef void* (*Reallocator)(void*, size_t);
typedef void  (*Deallocator)(void*);

// constant pointers to stdlib (OS) functions

// Wrapper to save original libc functions to global var

extern volatile Allocator   RMallocPtr;
extern volatile Callocator  RCallocPtr;
extern volatile Reallocator RReallocPtr;
extern volatile Deallocator RFreePtr;

#define getRMalloc()  RMallocPtr
#define getRCalloc()  RCallocPtr
#define getRRealloc() RReallocPtr
#define getRFree()    RFreePtr

#define setRMalloc(function)  RMallocPtr  = function;
#define setRRealloc(function) RReallocPtr = function;
#define setRCalloc(function)  RCallocPtr  = function;
#define setRFree(function)    RFreePtr    = function;

#ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR
    static Allocator   RTrueMalloc  = 0;
    static Reallocator RTrueRealloc = 0;
    static Callocator  RTrueCalloc  = 0;
    static Deallocator RTrueFree    = 0;

    void* malloc (size_t);
    void* calloc(size_t, size_t);
    void* realloc(void*, size_t);
    void  free(void*);
#else
    static const Allocator   RTrueMalloc  = &malloc;
    static const Reallocator RTrueRealloc = &realloc;
    static const Callocator  RTrueCalloc  = &calloc;
    static const Deallocator RTrueFree    = &free;

    #define malloc  RMallocPtr
    #define realloc RReallocPtr
    #define calloc  RCallocPtr
    #define free    RFreePtr
#endif

// universal

#define initPointers() setRMalloc(RTrueMalloc);\
                       setRFree(RTrueFree);\
                       setRCalloc(RTrueCalloc);\
                       setRRealloc(RTrueRealloc)

#define storePtrs() pointer (*oldMalloc) (size_t size) = getRMalloc();\
                    pointer (*oldRealloc)(pointer ptr, size_t oldSize) = getRRealloc();\
                    pointer (*oldCalloc) (size_t size, size_t blockSize) = getRCalloc();\
                    void    (*oldFree)   (pointer ptr) = getRFree()

#define backPtrs()  setRMalloc(oldMalloc);\
                    setRRealloc(oldRealloc);\
                    setRCalloc(oldCalloc);\
                    setRFree(oldFree)

//---------------------------------------------------------

// Additional DI
// Memory management
#define RAlloc            malloc
#define RReAlloc          realloc
#define RClearAlloc       calloc
#define RFree             free

// I/O
#define RPrintf           printf
#define RScanf            scanf
#define RGetChar          getchar
#define RGetCharUn        getchar_unlocked
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

// Memory operations
#define RMemCpy           memcpy
#define RMemMove          memmove
#define RMemCmp           memcmp

// Basics for strings
#define RVSNPrintf        vsnprintf
#define RSPrintf          sprintf
#define RStringLength     strlen
#define RStringToUnsigned strtoul
#define RStringToInt      atoi
#define RStringToDouble   atof
#define RIntToString      itoa


#ifdef __WIN32 // key delay for win
    #define stopConsole() getchar()
#else
    #define stopConsole()
#endif

#endif /*__RAY_BASE_H__*/