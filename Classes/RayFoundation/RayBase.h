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

typedef void* (*Allocator)  (size_t);
typedef void* (*Callocator) (size_t, size_t);
typedef void* (*Reallocator)(void*, size_t);
typedef void  (*Deallocator)(void*);

// constant pointers to stdlib (OS) functions

const Allocator   RTrueMalloc;
const Callocator  RTrueCalloc;
const Reallocator RTrueRealloc;
const Deallocator RTrueFree;

volatile Allocator   RMallocPtr;
volatile Callocator  RCallocPtr;
volatile Reallocator RReallocPtr;
volatile Deallocator RFreePtr;

//#define RAY_GLOBAL_ALLOCATOR_SETTERS
#ifdef RAY_GLOBAL_ALLOCATOR_SETTERS // useless functions, not works
    void* RMalloc (size_t size);
    void* RCalloc (size_t size, size_t blockSize);
    void* RRealloc(void*  ptr,  size_t size);
    void  RFree   (void*  ptr);

    void  setRMalloc (Allocator new);
    void  setRCalloc (Callocator new);
    void  setRRealloc(Reallocator new);
    void  setRFree   (Deallocator new);

    Allocator   getRMalloc (void);
    Callocator  getRCalloc (void);
    Reallocator getRRealloc(void);
    Deallocator getRFree   (void);

    #define malloc  RMalloc
    #define realloc RRealloc
    #define calloc  RCalloc
    #define free    RFree

#else // only macro only hardcore

    #define getRMalloc()  RMallocPtr
    #define getRCalloc()  RCallocPtr
    #define getRRealloc() RReallocPtr
    #define getRFree()    RFreePtr

    #define setRMalloc(function)  RMallocPtr  = function;
    #define setRRealloc(function) RReallocPtr = function;
    #define setRCalloc(function)  RCallocPtr  = function;
    #define setRFree(function)    RFreePtr    = function;

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
#ifndef RAY_GLOBAL_ALLOCATOR_SETTERS // RFree - is function name now
    #define RFree             free
#endif

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