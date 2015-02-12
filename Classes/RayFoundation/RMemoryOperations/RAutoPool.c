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

#ifdef R_POOL_DETAILED
    #include <RSandBox.h>
#endif

#if defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
    #define poolMutex &object->mutex
    #define RMutexLockPool() RMutexLock(poolMutex)
    #define RMutexUnlockPool() RMutexUnlock(poolMutex)
#else
    #define poolMutex
    #define RMutexLockPool()
    #define RMutexUnlockPool()
#endif


#define toPoolPtrs() RMallocPtr = object->innerMalloc;\
                     RReallocPtr = object->innerRealloc;\
                     RCallocPtr = object->innerCalloc;\
                     RFreePtr = object->innerFree

void poolPrinter(pointer some) {
#ifdef R_POOL_DETAILED
    RControlDescriptor* temp = some;
    RPrintf("%p [s: %lu] (tuid: %qu)\n", (pointer)temp->memRange.start, temp->memRange.size, temp->allocatorThread);
#else
    RPrintf("%p\n", some);
#endif
}

void innerFree(pointer some) {
#ifdef R_POOL_DETAILED
    free((pointer)((RControlDescriptor*)some)->memRange.start);
#endif
    free(some);
}

constructor(RAutoPool)) {
    object = allocator(RAutoPool);
    if(object != nil) {
        object->pointersInWork = makeRArray();
        if(object->pointersInWork != nil) {
            object->classId      = 5;
            object->innerMalloc  = malloc;
            object->innerRealloc = realloc;
            object->innerCalloc  = calloc;
            object->innerFree    = free;
            object->pointersInWork->destructorDelegate = innerFree;
            object->pointersInWork->printerDelegate = poolPrinter;

#if defined(RAY_POOL_THREAD_SAFE) && !defined(RAY_ARRAY_THREAD_SAFE)
            object->mutex = mutexWithType(RMutexRecursive);
#endif
        } else {
            RError("RAutoPool. Bad workers RArray allocation.", object);
        }
    }
    return object;
}

// create implementation for standart singleton
autoPoolNamed(singletonOfRAutoPool);

destructor(RAutoPool) {
    disablePool(object);
    deleter(object->pointersInWork, RArray);
}

printer(RAutoPool) {
    storePtrs();
    toPoolPtrs();
    RMutexLockPool();
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
    RPrintf("Pointers array: ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("---------------------- %p\n", object);
    RMutexUnlockPool();
    backPtrs();
}

method(pointer, malloc, RAutoPool), size_t sizeInBytes) {
    storePtrs();
    toPoolPtrs();
    RMutexLockPool();
    pointer temp = RAlloc(sizeInBytes);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
        RControlDescriptor* descriptor = allocator(RControlDescriptor);
        descriptor->memRange.size = sizeInBytes;
        descriptor->memRange.start = (size_t) temp;
        descriptor->allocatorThread = getThreadId();
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    RMutexUnlockPool();
    backPtrs();
    return temp;
}

method(pointer, realloc, RAutoPool), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RAutoPool)), newSize);
    } else {
        storePtrs();
        toPoolPtrs();
        RMutexLockPool();
        RCompareDelegate *delegate = allocator(RCompareDelegate);
        if(delegate != nil) {
#ifndef R_POOL_DETAILED
            delegate->virtualCompareMethod = nil;
            delegate->etaloneObject = ptr;
#else
            RControlDescriptor *descriptor = allocator(RControlDescriptor);
            descriptor->memRange.start = (size_t) ptr;
            delegate->etaloneObject = descriptor;
            delegate->virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRControlDescriptor;
#endif
            // search if it not first realloc for ptr
            RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
            if(result.object != nil) {
#ifndef R_POOL_DETAILED
                // not destruct it
                object->pointersInWork->destructorDelegate = nil;
#else
                // only free (must dealloc struct)
                object->pointersInWork->destructorDelegate = object->innerFree;
#endif
                if($(object->pointersInWork, m(fastDeleteObjectAtIndexIn, RArray)), result.index) != no_error) {
                    RError("Bad pointers array index.", object);
                }
                object->pointersInWork->destructorDelegate = innerFree;
            }

            pointer temp = RReAlloc(ptr, newSize);
            if(temp != nil) {
                // finally add new pointer
#ifdef R_POOL_DETAILED
                RControlDescriptor* descriptor2 = allocator(RControlDescriptor);
                descriptor2->memRange.size = newSize;
                descriptor2->memRange.start = (size_t) temp;
                descriptor2->allocatorThread = getThreadId();
                $(object->pointersInWork, m(addObject, RArray)), descriptor2);
#else
                $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
            }

#ifdef R_POOL_DETAILED
            deallocator(descriptor);
#endif
            deallocator(delegate);
            RMutexUnlockPool();
            backPtrs();
            return temp;
        } else {
            RError("RAutoPool. Bad RCompareDelegate allocation on realloc.", object);
        }
        RMutexUnlockPool();
        backPtrs();
    }
    return nil;
}

method(pointer, calloc, RAutoPool), size_t blockCount, size_t blockSize) {
    storePtrs();
    toPoolPtrs();
    // high lvl calloc
    RMutexLockPool();
    pointer temp = RClearAlloc(blockCount, blockSize);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
        RControlDescriptor* descriptor = allocator(RControlDescriptor);
        descriptor->memRange.size = blockCount * blockSize;
        descriptor->memRange.start = (size_t) temp;
        descriptor->allocatorThread = getThreadId();
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    RMutexUnlockPool();
    backPtrs();
    return temp;
}

method(void, free, RAutoPool), pointer ptr) {
    storePtrs();
    toPoolPtrs();
    RMutexLockPool();
    RCompareDelegate *delegate = allocator(RCompareDelegate);
    if(delegate != nil) {
#ifndef R_POOL_DETAILED
        delegate->virtualCompareMethod = nil;
        delegate->etaloneObject = ptr;
#else
        RControlDescriptor *descriptor = allocator(RControlDescriptor);
        descriptor->memRange.start = (size_t) ptr;
        delegate->etaloneObject = descriptor;
        delegate->virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRControlDescriptor;
        object->pointersInWork->destructorDelegate = object->innerFree;
#endif
        // search ptr
        RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
        if(result.object != nil) {
            if($(object->pointersInWork, m(fastDeleteObjectAtIndexIn, RArray)), result.index) != no_error) {
                RError("RAutoPool. Bad pointers array index.", object);
            }
        } else {
            RErrStr "ERROR. RAutoPool. Pointer - %p wasn't allocated on RAutoPool - %p\n", ptr, object);
        }
#ifdef R_POOL_DETAILED
        object->pointersInWork->destructorDelegate = innerFree;
        deallocator(descriptor);
#endif
        deallocator(delegate);
    } else {
        RError("RAutoPool. Bad RCompareDelegate allocation on free.", object);
    }
    RMutexUnlockPool();
    backPtrs();
}

method(void, drain, RAutoPool)) {
    RMutexLockPool();
    $(object->pointersInWork, m(flush, RArray)));
    RMutexUnlockPool();
}

void enablePool(RAutoPool *pool) {
    malloc = pool->selfMalloc;
    realloc = pool->selfRealloc;
    calloc = pool->selfCalloc;
    free = pool->selfFree;
}

void disablePool(RAutoPool *pool) {
    malloc = pool->innerMalloc;
    realloc = pool->innerRealloc;
    calloc = pool->innerCalloc;
    free = pool->innerFree;
}
