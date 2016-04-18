/**
 * @file  RAutoPool.c
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

#include "RAutoPool.h"


#define toPoolPtrs() setRMalloc (object->innerMalloc);\
                     setRCalloc (object->innerCalloc);\
                     setRRealloc(object->innerRealloc);\
                     setRFree   (object->innerFree)


#if defined(RAY_POOL_THREAD_SAFE)

    #define storePoolPtrs()
    #define backPoolPtrs()
    #define toInnerPoolPtrs()

    #define poolMutex          &object->mutex
    #define RMutexLockPool()   RMutexLock(poolMutex)
    #define RMutexUnlockPool() RMutexUnlock(poolMutex)

    #define isMutexDeadLocked !lockOrDeadlocked(poolMutex)
#else

    #define storePoolPtrs()   storePtrs()
    #define backPoolPtrs()    backPtrs()
    #define toInnerPoolPtrs() toPoolPtrs()

    #define poolMutex
    #define RMutexLockPool()
    #define RMutexUnlockPool()
#endif

#if defined(R_POOL_DETAILED) && !defined(R_POOL_META_ALLOC)
    RPoolDescriptor * descriptorWithInfo(RAutoPool *object, size_t size, pointer ptr, RThreadId threadId) {
        RPoolDescriptor *result = allocator(RPoolDescriptor);
        if(result != nil) {
            result->ptr = ptr;
            result->size = size;
            result->allocatorThread = threadId;
        }
        return result;
    }
#endif

#if defined(R_POOL_DETAILED) && defined(R_POOL_META_ALLOC)
    RPoolDescriptor * descriptorWithInfoMeta(RAutoPool *object, size_t size, pointer ptr, RThreadId threadId, char *tipString) {
        RPoolDescriptor *result = allocator(RPoolDescriptor);
        if(result != nil) {
            result->ptr = ptr;
            result->size = size;
            result->allocatorThread = threadId;
            result->tipString = tipString;
        }
        return result;
    }
#endif

void poolPrinter(pointer some) {
#ifdef R_POOL_DETAILED
    RPoolDescriptor* temp = some;
    RPrintf("%p  |  [s: %lu] (tuid: %lu)", temp->ptr, temp->size, (unsigned long) temp->allocatorThread);
    #ifdef R_POOL_META_ALLOC
        if(temp->tipString != nil) {
            RPrintf(" - %s", temp->tipString);
        }
    #endif
    RPrintf("\n");
#else
    RPrintf("%p\n", some);
#endif
}

#ifdef R_POOL_DETAILED
    RCompareFlags compareRPoolDescriptor(RPoolDescriptor *first, RPoolDescriptor *second) {
        return first->ptr == second->ptr ? equals : not_equals;
    }

    rbool totalEnumerator(pointer context, pointer object, size_t iterator) {
        *(size_t*)(context) += ((RPoolDescriptor*)object)->size;
        return yes;
    }
#endif

void innerFree(pointer some) {
#ifdef R_POOL_DETAILED
    deallocator(((RPoolDescriptor*)some)->ptr);
#endif
    deallocator(some);

}

RAutoPool* makeRAutoPool(size_t startSize, size_t multiplier) {
    RAutoPool* object = allocator(RAutoPool);
    if(object != nil) {
        object->pointersInWork = makeRArrayOptions(startSize, multiplier, nil);
        if(object->pointersInWork != nil) {
            object->classId      = 5;
            // store current hierarchy lvl memory functions
            object->innerMalloc  = getRMalloc();
            object->innerRealloc = getRRealloc();
            object->innerCalloc  = getRCalloc();
            object->innerFree    = getRFree();
            object->pointersInWork->destructorDelegate = innerFree;
            object->pointersInWork->printerDelegate = poolPrinter;

#if defined(RAY_POOL_THREAD_SAFE)
            mutexWithType(&object->mutex, RMutexErrorCheck);
#endif
        } elseError(
                RError("RAutoPool. Bad workers RArray allocation.", object)
        );

    }
    return object;
}

inline constructor(RAutoPool)) {
    return makeRAutoPool(startSizeOfRPoolDefault, sizeMultiplierOfRPoolDefault);
}

// create implementation for default singleton
autoPoolNamed(singletonOfRAutoPool, startSizeOfRPoolDefault, sizeMultiplierOfRPoolDefault)

destructor(RAutoPool) {
    disablePool(object);
    RMutexLockPool();
    deleter(object->pointersInWork, RArray);
    RMutexUnlockPool();
#ifdef RAY_POOL_THREAD_SAFE
    RMutexDestroy(poolMutex);
#endif
}

printer(RAutoPool) {
#ifdef R_POOL_DETAILED
    size_t *memTotal;
    RMutexLockPool();
    storePoolPtrs();
    toInnerPoolPtrs();
    memTotal = allocator(size_t);
    if(memTotal != nil) {
        REnumerateDelegate delegate;
        delegate.context = memTotal;
        *memTotal = 0;
        delegate.virtualEnumerator = totalEnumerator;
        $(object->pointersInWork, m(enumerate, RArray)), &delegate, yes);
    }
#else
    RMutexLockPool();
    storePoolPtrs();
    toInnerPoolPtrs();
#endif
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
#ifdef R_POOL_DETAILED
    RPrintf("\t Printer tuid : %lu\n", (unsigned long) currentThreadIdentifier());
    if(memTotal != nil) {
        RPrintf("\t Size total %lu bytes\n", *memTotal);
    }
#endif
    RPrintf("Pointers ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("-------------------------- %p\n", object);
#ifdef R_POOL_DETAILED
    deallocator(memTotal);
#endif
    RMutexUnlockPool();
    backPoolPtrs();
}

method(pointer, malloc, RAutoPool), size_t sizeInBytes) {
#ifdef RAY_POOL_THREAD_SAFE
    if(isMutexDeadLocked) {
        return object->innerMalloc(sizeInBytes);
    }
#endif
    storePoolPtrs();
    toInnerPoolPtrs();
    pointer temp = RAlloc(sizeInBytes);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(object, sizeInBytes, temp, currentThreadIdentifier(), nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(object, sizeInBytes, temp, currentThreadIdentifier());
    #endif
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    RMutexUnlockPool();
    backPoolPtrs();
    return temp;
}

#ifdef R_POOL_META_ALLOC
    method(pointer, metaAlloc, RAutoPool), size_t sizeInBytes, char *tipString) {
#ifdef RAY_POOL_THREAD_SAFE
        if(isMutexDeadLocked) {
            RWarning("RAutoPool. DeadLock on metaAlloc, using high lvl malloc instead.", object);
            return object->innerMalloc(sizeInBytes);
        }
#endif
        pointer temp;

        temp = RAlloc(sizeInBytes);
        if(temp != nil) {
            RPoolDescriptor *descriptor = descriptorWithInfoMeta(object, sizeInBytes, temp, currentThreadIdentifier(), tipString);
            $(object->pointersInWork, m(addObject, RArray)), descriptor);
        }
        
        RMutexUnlockPool();
        return temp;
    }
#endif

method(pointer, realloc, RAutoPool), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RAutoPool)), newSize);
    } else {
#ifdef RAY_POOL_THREAD_SAFE
        if(isMutexDeadLocked) {
            return object->innerRealloc(ptr, newSize);
        }
#endif
        RCompareDelegate delegate;

        storePoolPtrs();
        toInnerPoolPtrs();

    #ifndef R_POOL_DETAILED
            delegate.virtualCompareMethod = defaultComparator;
            delegate.etaloneObject = ptr;
    #else
        #ifdef R_POOL_META_ALLOC
            char *storedTip = nil;
            RPoolDescriptor* descriptor = descriptorWithInfoMeta(object, 0, ptr, 0, nil);
        #else
            RPoolDescriptor* descriptor = descriptorWithInfo(object, 0, ptr, 0);
        #endif
            delegate.etaloneObject = descriptor;
            delegate.virtualCompareMethod = (ComparatorDelegate) compareRPoolDescriptor;
    #endif
            // search if it not first realloc for ptr
            RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), &delegate);
            if(result.object != nil) {
                RArrayFlags error;
    #ifndef R_POOL_DETAILED
                // not destruct it
                object->pointersInWork->destructorDelegate = nil;
    #else
        #ifdef R_POOL_META_ALLOC
                storedTip = ((RPoolDescriptor*)result.object)->tipString;
        #endif
                // only free (must dealloc struct)
                object->pointersInWork->destructorDelegate = object->innerFree;
    #endif
                error = $(object->pointersInWork, m(deleteObjectAtIndexFast, RArray)), result.index);

                ifError(error != no_error,
                        RError("Bad pointers array index on delete (realloc).", object)
                );

                object->pointersInWork->destructorDelegate = innerFree;
            }

            pointer temp = RReAlloc(ptr, newSize);
            if(temp != nil) {
                    // finally add new pointer
    #ifdef R_POOL_DETAILED
        #ifdef R_POOL_META_ALLOC
                RPoolDescriptor *descriptor2 = descriptorWithInfoMeta(object, newSize, temp, currentThreadIdentifier(), storedTip);
        #else
                RPoolDescriptor *descriptor2 = descriptorWithInfo(object, newSize, temp, currentThreadIdentifier());
        #endif
                $(object->pointersInWork, m(addObject, RArray)), descriptor2);
    #else
                $(object->pointersInWork, m(addObject, RArray)), temp);
    #endif
            }

    #ifdef R_POOL_DETAILED
            deallocator(descriptor);
    #endif
            RMutexUnlockPool();

        backPoolPtrs();
            return temp;
    }
    return nil;
}

method(pointer, calloc, RAutoPool), size_t blockCount, size_t blockSize) {
#ifdef RAY_POOL_THREAD_SAFE
    if(isMutexDeadLocked) {
        return object->innerCalloc(blockCount, blockSize);
    }
#endif

    storePoolPtrs();
    toInnerPoolPtrs();

    // high lvl calloc
    pointer temp = RClearAlloc(blockCount, blockSize);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(object, blockCount * blockSize, temp,
                                                             currentThreadIdentifier(), nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(object, blockCount * blockSize, temp, currentThreadIdentifier());
    #endif
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    RMutexUnlockPool();
    backPoolPtrs();
    return temp;
}

method(void, free, RAutoPool), pointer ptr) {
    if(ptr != nil) {
#ifdef RAY_POOL_THREAD_SAFE
        if(isMutexDeadLocked) {
            return object->innerFree(ptr);
        }
#endif
        RCompareDelegate delegate;
        storePoolPtrs();
        toInnerPoolPtrs();
#ifndef R_POOL_DETAILED
        delegate.virtualCompareMethod = defaultComparator;
        delegate.etaloneObject = ptr;
#else
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(object, 0, ptr, 0, nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(object, 0, ptr, 0);
    #endif
        delegate.etaloneObject        = descriptor;
        delegate.virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRPoolDescriptor;
#endif
        // search ptr
        RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), &delegate);
        if(result.object != nil) {
            if($(object->pointersInWork, m(deleteObjectAtIndexFast, RArray)), result.index) != no_error) {
                RError1("RAutoPool. Bad pointers array index %lu.", object, result.index);
            }
        } else {
            RError1("RAutoPool. Pointer - %p wasn't allocated on RAutoPool.", object, ptr);
            deallocator(ptr);
        }
#ifdef R_POOL_DETAILED
        deallocator(descriptor);
#endif
        RMutexUnlockPool();
        backPoolPtrs();
    } else {
        RWarning("RAutoPool. Free nil.", object);
    }
}

#ifdef R_POOL_DETAILED

rbool deleterEnumerator(pointer context, pointer object, size_t iterator) {
    RPoolDescriptor *temp = object;
    if(temp->allocatorThread == (RThreadId)context) {
        return yes;
    } else {
        return no;
    }
}

#endif

method(void, drain, RAutoPool)) {
    RMutexLockPool();
    storePoolPtrs();
    toInnerPoolPtrs();
#ifndef R_POOL_DETAILED
    $(object->pointersInWork, m(flush, RArray)));
#else
    REnumerateDelegate delegate;
    delegate.context = (pointer) currentThreadIdentifier();
    delegate.virtualEnumerator = deleterEnumerator;
    $(object->pointersInWork, m(deleteWithPredicate, RArray)), &delegate);
#endif
    RMutexUnlockPool();
    backPoolPtrs();
}

void enablePool(RAutoPool *object) {
    RMutexLockPool();
    if(getRMalloc() != object->selfMalloc) {
        setRMalloc (object->selfMalloc);
    }
    if(getRCalloc () != object->selfCalloc) {
        setRCalloc (object->selfCalloc);
    }
    if(getRRealloc() != object->selfRealloc) {
        setRRealloc(object->selfRealloc);
    }
    if(getRFree   () != object->selfFree) {
        setRFree   (object->selfFree);
    }
    RMutexUnlockPool();
}

void disablePool(RAutoPool *object) {
    RMutexLockPool();
    toPoolPtrs();
    RMutexUnlockPool();
}

#ifdef RAY_BLOCKS_ON

autoPoolNamed(singleBlockPool, 64, 3);

inline
void autoReleaseBlock(void(^block)(void)) {
    enablePool(singleBlockPool());
    block();
    $(singleBlockPool(), m(drain, RAutoPool)));
    disablePool(singleBlockPool());
}

#endif
