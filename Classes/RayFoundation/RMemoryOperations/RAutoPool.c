/**
 * RAutoPool.c
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

#include <RAutoPool.h>
#include <RClassTable.h>

#if defined(RAY_POOL_THREAD_SAFE)
    #define poolMutex          &object->mutex
    #define RMutexLockPool()   RMutexLock(poolMutex)
    #define RMutexUnlockPool() RMutexUnlock(poolMutex)
#else
    #define poolMutex
    #define RMutexLockPool()
    #define RMutexUnlockPool()
#endif


#define toPoolPtrs() setRMalloc (object->innerMalloc);\
                     setRCalloc (object->innerCalloc);\
                     setRRealloc(object->innerRealloc);\
                     setRFree   (object->innerFree);

#if defined(R_POOL_DETAILED) && !defined(R_POOL_META_ALLOC)
    RPoolDescriptor * descriptorWithInfo(size_t size, pointer ptr, RThreadId threadId) {
        RPoolDescriptor *result = allocator(RPoolDescriptor);
        if(result != nil) {
            result->ptr = ptr;
            result->size = size;
            result->allocatorThread = threadId;
        }
        return result;
    }
#endif

#ifdef R_POOL_META_ALLOC
    RPoolDescriptor * descriptorWithInfoMeta(size_t size, pointer ptr, RThreadId threadId, char *tipString) {
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
    RPrintf("%p [s: %lu] (tuid: %lu)", temp->ptr, temp->size, (unsigned long) temp->allocatorThread);
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

constructor(RAutoPool)) {
    object = allocator(RAutoPool);
    if(object != nil) {
        object->pointersInWork = makeRArray();
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
            mutexWithType(&object->mutex, RMutexNormal);
#endif
        } elseError(
            RError("RAutoPool. Bad workers RArray allocation.", object)
        );

    }
    return object;
}

// create implementation for default singleton
autoPoolNamed(singletonOfRAutoPool)

destructor(RAutoPool) {
    disablePool(object);
    deleter(object->pointersInWork, RArray);
#ifdef RAY_POOL_THREAD_SAFE
    RMutexDestroy(poolMutex);
#endif
}

printer(RAutoPool) {
#ifdef R_POOL_DETAILED
    size_t *memTotal;
    storePtrs();
    toPoolPtrs();
    RMutexLockPool();
    memTotal = allocator(size_t);
    if(memTotal != nil) {
        REnumerateDelegate delegate;
        delegate.context = memTotal;
        *memTotal = 0;
        delegate.virtualEnumerator = totalEnumerator;
        $(object->pointersInWork, m(enumerate, RArray)), &delegate, yes);
    }
    backPtrs();
#else
    RMutexLockPool();
#endif
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
#ifdef R_POOL_DETAILED
    RPrintf("\t Printer tuid : %lu\n", (unsigned long) currentTreadIdentifier());
    if(memTotal != nil) {
        RPrintf("\t Size total %lu bytes\n", *memTotal);
    }
#endif
    RPrintf("Pointers array: ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("-------------------------- %p\n", object);
    RMutexUnlockPool();
}

method(pointer, malloc, RAutoPool), size_t sizeInBytes) {
    RMutexLockPool();
    storePtrs();
    toPoolPtrs();
    pointer temp = RAlloc(sizeInBytes);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(sizeInBytes, temp, currentTreadIdentifier(), nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(sizeInBytes, temp, currentTreadIdentifier());
    #endif
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    backPtrs();
    RMutexUnlockPool();
    return temp;
}

#ifdef R_POOL_META_ALLOC
    method(pointer, metaAlloc, RAutoPool), size_t sizeInBytes, char *tipString) {
        RMutexLockPool();
        storePtrs();
        toPoolPtrs();
        pointer temp = RAlloc(sizeInBytes);
        if(temp != nil) {
            RPoolDescriptor *descriptor = descriptorWithInfoMeta(sizeInBytes, temp, currentTreadIdentifier(), tipString);
            $(object->pointersInWork, m(addObject, RArray)), descriptor);
        }
        backPtrs();
        RMutexUnlockPool();
        return temp;
    }
#endif

method(pointer, realloc, RAutoPool), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RAutoPool)), newSize);
    } else {
        RMutexLockPool();
        storePtrs();
        toPoolPtrs();
        RCompareDelegate delegate;
#ifndef R_POOL_DETAILED
        delegate.virtualCompareMethod = defaultComparator;
        delegate.etaloneObject = ptr;
#else
    #ifdef R_POOL_META_ALLOC
        char *storedTip = nil;
        RPoolDescriptor* descriptor = descriptorWithInfoMeta(0, ptr, 0, nil);
    #else
        RPoolDescriptor* descriptor = descriptorWithInfo(0, ptr, 0);
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
            error = $(object->pointersInWork, m(fastDeleteObjectAtIndexIn, RArray)), result.index);

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
            RPoolDescriptor *descriptor2 = descriptorWithInfoMeta(newSize, temp, currentTreadIdentifier(), storedTip);
    #else
            RPoolDescriptor *descriptor2 = descriptorWithInfo(newSize, temp, currentTreadIdentifier());
    #endif
            $(object->pointersInWork, m(addObject, RArray)), descriptor2);
#else
            $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
        }

#ifdef R_POOL_DETAILED
        deallocator(descriptor);
#endif
        backPtrs();
        RMutexUnlockPool();
        return temp;
    }
    return nil;
}

method(pointer, calloc, RAutoPool), size_t blockCount, size_t blockSize) {
    RMutexLockPool();
    storePtrs();
    toPoolPtrs();
    // high lvl calloc
    pointer temp = RClearAlloc(blockCount, blockSize);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(blockCount * blockSize, temp, currentTreadIdentifier(), nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(blockCount * blockSize, temp, currentTreadIdentifier());
    #endif
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    backPtrs();
    RMutexUnlockPool();
    return temp;
}

method(void, free, RAutoPool), pointer ptr) {
    if(ptr != nil) {
        RMutexLockPool();
        storePtrs();
        toPoolPtrs();
        RCompareDelegate delegate;
#ifndef R_POOL_DETAILED
        delegate.virtualCompareMethod = defaultComparator;
        delegate.etaloneObject = ptr;
#else
    #ifdef R_POOL_META_ALLOC
        RPoolDescriptor *descriptor = descriptorWithInfoMeta(0, ptr, 0, nil);
    #else
        RPoolDescriptor *descriptor = descriptorWithInfo(0, ptr, 0);
    #endif
        delegate.etaloneObject        = descriptor;
        delegate.virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRPoolDescriptor;
#endif
        // search ptr
        RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), &delegate);
        if(result.object != nil) {
            if($(object->pointersInWork, m(fastDeleteObjectAtIndexIn, RArray)), result.index) != no_error) {
                RError1("RAutoPool. Bad pointers array index %lu.", object, result.index);
            }
        } else {
            RError1("RAutoPool. Pointer - %p wasn't allocated on RAutoPool.", object, ptr);
            deallocator(ptr);
        }
#ifdef R_POOL_DETAILED

        ifError(free == object->selfFree,
            RError("RAutoPool. Inner free is self free, bad bad bad!!!", object)
        );

        deallocator(descriptor);
#endif
        backPtrs();
        RMutexUnlockPool();
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
    storePtrs();
    toPoolPtrs();
#ifndef R_POOL_DETAILED
    $(object->pointersInWork, m(flush, RArray)));
#else
    REnumerateDelegate delegate;
    delegate.context = (pointer) currentTreadIdentifier();
    delegate.virtualEnumerator = deleterEnumerator;
    $(object->pointersInWork, m(deleteWithPredicate, RArray)), &delegate);
#endif
    backPtrs();
    RMutexUnlockPool();
}

void enablePool(RAutoPool *object) {
    RMutexLockPool();
    setRMalloc (object->selfMalloc);
    setRCalloc (object->selfCalloc);
    setRRealloc(object->selfRealloc);
    setRFree   (object->selfFree);
    RMutexUnlockPool();
}

void disablePool(RAutoPool *object) {
    RMutexLockPool();
    toPoolPtrs();
    RMutexUnlockPool();
}
