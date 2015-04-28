/**
 * RAutoPool.h
 * Memory management instrument.
 * Self-longs array, stores malloc-calloc-realloced pointers.
 * Free it if needs.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/1/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/
 
#ifndef __R_AUTO_POOL_H__
#define __R_AUTO_POOL_H__

#include <RSyntax.h>
#include <RArray.h>

#if defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
    #include <RThread.h>
#endif

#ifdef R_POOL_DETAILED
    #if !defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
        #include <RThread.h>
    #endif

typedef struct RPoolDescriptor {
    RThreadId allocatorThread;
    pointer   ptr;
    size_t    size;
#ifdef R_POOL_META_ALLOC
    char     *tipString;
#endif
} RPoolDescriptor;
#endif

class(RAutoPool)
    RArray *pointersInWork;

    // inner (high-lvl in hierarchy functions)
    Allocator   innerMalloc;
    Reallocator innerRealloc;
    Callocator  innerCalloc;
    Deallocator innerFree;

    // self functions
    Allocator   selfMalloc;
    Reallocator selfRealloc;
    Callocator  selfCalloc;
    Deallocator selfFree;

#if defined(RAY_POOL_THREAD_SAFE)
    RMutex mutex;
#endif
endOf(RAutoPool)

constructor (RAutoPool));
destructor  (RAutoPool);
printer     (RAutoPool);
singleton   (RAutoPool);

method(pointer, malloc,         RAutoPool),    size_t sizeInBytes);

#ifdef R_POOL_META_ALLOC
    method(pointer, metaAlloc,      RAutoPool),    size_t sizeInBytes, char *tipString);
#endif

method(pointer, realloc,        RAutoPool),    pointer ptr, size_t newSize);
method(pointer, calloc,         RAutoPool),    size_t blockCount, size_t blockSize);
method(void,    free,           RAutoPool),    pointer ptr);

method(void,    drain,          RAutoPool)); // cleanup pointers only for that thread, not all if R_AUTO_POOL_DETAILED macro defined

void enablePool(RAutoPool *pool);
void disablePool(RAutoPool *pool);

//-------------------------------------------------------------------------------

#define autoPoolNamed(name) \
RAutoPool* name();\
pointer concatenate(PoolAllocator_, name)(size_t size) {\
    return $(name(), m(malloc, RAutoPool)), size);\
}\
pointer concatenate(PoolReallocator_, name)(pointer ptr, size_t size) {\
    return $(name(), m(realloc, RAutoPool)), ptr, size);\
}\
pointer concatenate(PoolCallocator_, name)(size_t size, size_t blockSize) {\
    return $(name(), m(calloc, RAutoPool)), size, blockSize);\
}\
void concatenate(PoolDeallocator_, name)(pointer ptr) {\
    return $(name(), m(free, RAutoPool)), ptr);\
}\
RAutoPool* name(void) { \
    static RAutoPool *instance = nil; \
    if(instance == nil) { \
        instance = $(nil, c(RAutoPool))); \
        if(instance != nil) { \
            instance->selfMalloc  = concatenate(PoolAllocator_, name); \
            instance->selfRealloc = concatenate(PoolReallocator_, name); \
            instance->selfCalloc  = concatenate(PoolCallocator_, name); \
            instance->selfFree    = concatenate(PoolDeallocator_, name); \
            enablePool(instance); \
        } \
    } \
    return instance; \
}

// autoPoolNamed ----------------------------------------------------------------

#define RPool singletonCall(RAutoPool)

#ifdef R_POOL_META_ALLOC
    #define metaAlloc(sizeInBytes, tipString) $(RPool, m(metaAlloc, RAutoPool)), sizeInBytes, tipString)
#else
    #define metaAlloc(sizeInBytes, tipString) RAlloc(sizeInBytes)
#endif

#endif /*__R_AUTO_POOL_H__*/
