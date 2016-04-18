/**
 * @file  RAutoPool.h
 * @brief Memory management instrument, that
 *        stores malloc-calloc-realloced pointers.
 *        Free it if drain called.
 *
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 12/1/14
 * @par Ukraine Kharkiv
 *
 *//*
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 */
 
#ifndef __R_AUTO_POOL_H__
#define __R_AUTO_POOL_H__

#include "RayFoundation/RContainers/RArray.h"

#if defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
    #include "RayFoundation/RThread/RThread.h"
#endif

#ifdef R_POOL_DETAILED
    #if !defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
        #include "RayFoundation/RThread/RThread.h"
    #endif

    /**
     * Main structure for handle pointer malloced info.
     */
    typedef struct RPoolDescriptor {
        RThreadId allocatorThread; ///< id of thread called malloc
        pointer   ptr;             ///< pointer was allocated
        size_t    size;            ///< size was allocated
    #ifdef R_POOL_META_ALLOC
        char     *tipString;       ///< pointer to meta-information string from RAutoPool.metaAlloc
                                   ///< (0-terminated C constant string, that not need to be freed)
    #endif
    } RPoolDescriptor;
#endif

#define startSizeOfRPoolDefault      1024    ///< default size of pointer array, created with constructor
#define sizeMultiplierOfRPoolDefault 8       ///< default multiplier value of pointer arrays, created with constructor

/**
 * Main structure for handle pointer malloced info,
 * via storing direct pointers or pointers to RPoolDescriptor.
 * Settings up with R_POOL_DETAILED,      - stores allocatorThread and size
 *                  R_POOL_META_ALLOC,    - additional meta-information store via RAutoPool.metaAlloc
 *                  RAY_POOL_THREAD_SAFE  - threadsafety for call one pool for different threads
 */
class(RAutoPool)
    RArray *pointersInWork; ///< array to store direct pointers or pointers to RPoolDescriptor

    // inner (high-lvl in hierarchy functions)

    Allocator   innerMalloc;     ///< pointer to high-lvl Malloc function
    Reallocator innerRealloc;    ///< pointer to high-lvl Realloc function
    Callocator  innerCalloc;     ///< pointer to high-lvl Calloc function
    Deallocator innerFree;       ///< pointer to high-lvl Free function

    // self functions

    Allocator   selfMalloc;     ///< pointer to self Malloc function
    Reallocator selfRealloc;    ///< pointer to self Realloc function
    Callocator  selfCalloc;     ///< pointer to self Calloc function
    Deallocator selfFree;       ///< pointer to self Free function

#if defined(RAY_POOL_THREAD_SAFE)
    RMutex mutex;               ///< sigle-thread lock, type RMutexErrorCheck
#endif
endOf(RAutoPool)

/**
 * Main optional constructor.
 *
 * @param       startSize     Start size of array, allocates immediately
 * @param       multiplier    Multiplier, used when array longs-up on addObject
 *
 * @return Created instance of RAutoPool or nil if error occurs.
 */
RAutoPool* makeRAutoPool(size_t startSize, size_t multiplier);

/**
 * Default constructor.
 * Calling RAutoPool.makeRAutoPool with
 * startSize = startSizeOfRPoolDefault
 * multiplier = sizeMultiplierOfRPoolDefault
 *
 * @return Created instance of RAutoPool or nil if error occurs.
 */
extern constructor(RAutoPool));

/**
 * Default destructor.
 * Cleanup all pointers to objects
 * (cleanup of RPoolDescriptor structs if used)
 */
destructor(RAutoPool);

/**
 * Default printer.
 * Displays printer thread id, total malloced size,
 * pointer with size allocated, allocater thread,
 * and metainfo if properly configured.
 */
printer(RAutoPool);

/**
 * Main Ray library singleton,
 * called with RPool macros.
 */
singleton(RAutoPool);

/**
 * Current pool malloc function.
 * If upper level allocator success (!= nil),
 * than add pointer to pointersInWork array.
 * Using RArray.addObject
 *
 * @param       sizeInBytes    Size to allocate with upper level malloc
 *
 * @return allocated and registered pointer to memory or nil
 */
method(pointer, malloc,         RAutoPool),    size_t sizeInBytes);

#ifdef R_POOL_META_ALLOC

/**
 * Current pool meta information malloc function.
 * If upper level allocator success (!= nil),
 * than add pointer to pointersInWork array.
 *
 * Warning: if currect pool is locked - that use high lvl malloc instead.
 *
 * Using RArray.addObject
 *
 * @param       sizeInBytes    Size to allocate with upper level malloc
 * @param       tipString      Pointer to 0-terminated C constant string
 *
 * @return allocated and registered pointer to memory or nil
 */
method(pointer, metaAlloc,      RAutoPool),    size_t sizeInBytes, char *tipString);
#endif

/**
 * Current pool realloc function to change size of ptr.
 * Using RArray.findObjectWithDelegate,
 *       RArray.deleteObjectAtIndexFast
 *
 * @param       ptr          Pointer, that must be realloced
 *                           if nil, uses RAutoPool.malloc
 *
 * @param       newSize      New long of memory range, can be smaller or bigger than old
 *
 * @return relloced pointer with new size or nil
 */
method(pointer, realloc,        RAutoPool),    pointer ptr, size_t newSize);

/**
 * Current pool calloc function.
 * If upper level allocator success (!= nil),
 * than add pointer to pointersInWork array.
 * Using RArray.addObject
 *
 * @param       blockCount   Count of blocks allocated
 * @param       blockSize    Size of memory block in bytes
 *
 * @return allocated and registered pointer to memory or nil, memory in range blockCount * blockSize will be set to 0
 */
method(pointer, calloc,         RAutoPool),    size_t blockCount, size_t blockSize);

/**
 * Current pool free function.
 * Remove pointer from pointer in work,
 * than cleanup it with upper level free.
 * Using RArray.findObjectWithDelegate,
 *       RArray.deleteObjectAtIndexFast
 *
 * @param       ptr        Pointer to delete
 */
method(void,    free,           RAutoPool),    pointer ptr);

/**
 * Pool drain function.
 * If R_POOL_DETAILED is ON
 * than deletes pointers only allocated by caller thread, using RArray.deleteWithPredicate
 * Else deletes all, using RArray.flush
 */
method(void,    drain,          RAutoPool)); // cleanup pointers only for that thread, not all if R_AUTO_POOL_DETAILED macro defined

/**
 * Pool enable function.
 * Sets all memory management functions to pool self functions.
 *
 * @param       pool    Pool to enable
 */
void enablePool (RAutoPool *pool);

/**
 * Pool disable function.
 * Sets all memory management functions to pool inner functions.
 *
 * @param       pool    Pool to disable
 */
void disablePool(RAutoPool *pool);

#ifdef RAY_BLOCKS_ON
    /**
     * Pool to cleanup after block execution.
     * Using autoPoolNamed(singleBlockPool, 64, 3)
     *
     * @return Small pool to cleanup after block
     */
    RAutoPool *singleBlockPool(); // much smaller than RPool

    /**
     * Function to cleanup after block execution.
     * Using enablePool
             RAutoPool.drain
             disablePool

     * @param    block    Block to execute and cleanup after
     */
    extern void autoReleaseBlock(void(^block)(void));
#endif

//-------------------------------------------------------------------------------

/**
 * Macro to define function of memory management with some name.
 * I.e. creates static function with pool reference and creates
 * memory management functions Allocator_name
                               Reallocator_name
                               Callocator_name
                               Deallocator_name

   to store compatibility with C stdlib one parameter malloc, realloc, free, calloc,
   without passing pool parameter to it, but only static call of name function that returns pool.
 */
#define autoPoolNamed(name, startSize, multiplier) \
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
        instance = makeRAutoPool((startSize), (multiplier)); \
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

/**
 * Main metaAlloc function call.
 * When R_POOL_META_ALLOC is ON - stores metainfo, else - ignore it.
 */
#ifdef R_POOL_META_ALLOC
    #define metaAlloc(sizeInBytes, tipString) $(RPool, m(metaAlloc, RAutoPool)), sizeInBytes, tipString)
#else
    #define metaAlloc(sizeInBytes, tipString) RAlloc(sizeInBytes)
#endif

#endif /*__R_AUTO_POOL_H__*/
