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

#if defined(RAY_POOL_THREAD_SAFE)
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
                     RFreePtr = object->innerFree;

void poolPrinter(pointer some) {
#ifdef R_POOL_DETAILED
    RPoolDescriptor* temp = some;
    RPrintf("%p [s: %lu] (tuid: %qu)\n", temp->ptr, temp->size, temp->allocatorThread);
#else
    RPrintf("%p\n", some);
#endif
}

#ifdef R_POOL_DETAILED
RCompareFlags compareRPoolDescriptor(RPoolDescriptor *first, RPoolDescriptor *second) {
    return /*first != nil && */first->ptr == second->ptr ? equals : not_equals;
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
            object->innerMalloc  = malloc;
            object->innerRealloc = realloc;
            object->innerCalloc  = calloc;
            object->innerFree    = free;
            object->pointersInWork->destructorDelegate = innerFree;
            object->pointersInWork->printerDelegate = poolPrinter;

#if defined(RAY_POOL_THREAD_SAFE)
            mutexWithType(&object->mutex, RMutexNormal);
#endif
        } else {
            RError("RAutoPool. Bad workers RArray allocation.", object);
        }
    }
    return object;
}

// create implementation for default singleton
autoPoolNamed(singletonOfRAutoPool);

destructor(RAutoPool) {
    disablePool(object);
    deleter(object->pointersInWork, RArray);
}

printer(RAutoPool) {
    RMutexLockPool();
    storePtrs();
    toPoolPtrs();
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
    RPrintf("Pointers array: ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("---------------------- %p\n", object);
    backPtrs();
    RMutexUnlockPool();
}

method(pointer, malloc, RAutoPool), size_t sizeInBytes) {
    RMutexLockPool();
    storePtrs();
    toPoolPtrs();
    pointer temp = RAlloc(sizeInBytes);
    if(temp != nil) {
#ifdef R_POOL_DETAILED
        RPoolDescriptor* descriptor = allocator(RPoolDescriptor);
        descriptor->size = sizeInBytes;
        descriptor->ptr = temp;
        descriptor->allocatorThread = currentTreadIdentifier();
        $(object->pointersInWork, m(addObject, RArray)), descriptor);
#else
        $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
    }
    backPtrs();
    RMutexUnlockPool();
    return temp;
}

method(pointer, realloc, RAutoPool), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RAutoPool)), newSize);
    } else {
        RMutexLockPool();
        storePtrs();
        toPoolPtrs();
        RCompareDelegate *delegate = allocator(RCompareDelegate);
        if(delegate != nil) {
#ifndef R_POOL_DETAILED
            delegate->virtualCompareMethod = nil;
            delegate->etaloneObject = ptr;
#else
            RPoolDescriptor *descriptor = allocator(RPoolDescriptor);
            descriptor->ptr = ptr;
            delegate->etaloneObject = descriptor;
            delegate->virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRPoolDescriptor;
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
                    RError("Bad pointers array index on delete (realloc).", object);
                }
                object->pointersInWork->destructorDelegate = innerFree;
            }

            pointer temp = RReAlloc(ptr, newSize);
            if(temp != nil) {
                // finally add new pointer
#ifdef R_POOL_DETAILED
                RPoolDescriptor* descriptor2 = allocator(RPoolDescriptor);
                descriptor2->size = newSize;
                descriptor2->ptr  = temp;
                descriptor2->allocatorThread = currentTreadIdentifier();
                $(object->pointersInWork, m(addObject, RArray)), descriptor2);
#else
                $(object->pointersInWork, m(addObject, RArray)), temp);
#endif
            }

#ifdef R_POOL_DETAILED
            deallocator(descriptor);
#endif
            deallocator(delegate);
            backPtrs();
            RMutexUnlockPool();
            return temp;
        } else {
            RError("RAutoPool. Bad RCompareDelegate allocation (realloc).", object);
        }
        backPtrs();
        RMutexUnlockPool();
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
        RPoolDescriptor* descriptor = allocator(RPoolDescriptor);
        descriptor->size = blockCount * blockSize;
        descriptor->ptr  = temp;
        descriptor->allocatorThread = currentTreadIdentifier();
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
    RMutexLockPool();
    storePtrs();
    toPoolPtrs();
    RCompareDelegate *delegate = allocator(RCompareDelegate);
    if(delegate != nil) {
#ifndef R_POOL_DETAILED
        delegate->virtualCompareMethod = nil;
        delegate->etaloneObject = ptr;
#else
        RPoolDescriptor *descriptor = allocator(RPoolDescriptor);
        descriptor->ptr = ptr;
        delegate->etaloneObject = descriptor;
        delegate->virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareRPoolDescriptor;
#endif
        // search ptr
        RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
        if(result.object != nil) {
            if($(object->pointersInWork, m(fastDeleteObjectAtIndexIn, RArray)), result.index) != no_error) {
                RError("RAutoPool. Bad pointers array index.", object);
            }
        } else {
            RErrStr "%p ERROR. RAutoPool. Pointer - %p wasn't allocated on RAutoPool.\n", object, ptr);
        }
#ifdef R_POOL_DETAILED
        if(RFreePtr == object->selfFree) {
            RError("RAutoPool. Inner free is self free, bad bad bad!!!", object);
        }
        deallocator(descriptor);
#endif
        deallocator(delegate);
    } else {
        RError("RAutoPool. Bad RCompareDelegate allocation (free).", object);
    }
    backPtrs();
    RMutexUnlockPool();
}

method(void, drain, RAutoPool)) {
    RMutexLockPool();
#ifndef R_POOL_DETAILED
    $(object->pointersInWork, m(flush, RArray)));
#else
//    fixme partiral delete
//    storedId = currentTreadIdentifier();
//    $(object->pointersInWork, m(enumerate, RArray)), &delegate, yes);
#endif
    RMutexUnlockPool();
}

void enablePool(RAutoPool *object) {
    RMutexLockPool();
    malloc  = object->selfMalloc;
    realloc = object->selfRealloc;
    calloc  = object->selfCalloc;
    free    = object->selfFree;
    RMutexUnlockPool();
}

void disablePool(RAutoPool *object) {
    RMutexLockPool();
    malloc  = object->innerMalloc;
    realloc = object->innerRealloc;
    calloc  = object->innerCalloc;
    free    = object->innerFree;
    RMutexUnlockPool();
}
