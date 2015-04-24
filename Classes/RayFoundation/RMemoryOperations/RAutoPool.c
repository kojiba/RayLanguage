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

void poolPrinter(pointer some) {
#ifdef R_POOL_DETAILED
    RPoolDescriptor* temp = some;
    RPrintf("%p [s: %lu] (tuid: %lu)\n", temp->ptr, temp->size, temp->allocatorThread);
#else
    RPrintf("%p\n", some);
#endif
}

#ifdef R_POOL_DETAILED
    RCompareFlags compareRPoolDescriptor(RPoolDescriptor *first, RPoolDescriptor *second) {
        return first->ptr == second->ptr ? equals : not_equals;
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
}

printer(RAutoPool) {
    RMutexLockPool();
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
#ifdef R_POOL_DETAILED
    RPrintf("\t Printer tuid : %lu\n", currentTreadIdentifier());
#endif
    RPrintf("Pointers array: ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("---------------------- %p\n", object);
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
            delegate->virtualCompareMethod = (ComparatorDelegate) compareRPoolDescriptor;
#endif
            // search if it not first realloc for ptr
            RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
            if(result.object != nil) {
                RArrayFlags error;
#ifndef R_POOL_DETAILED
                // not destruct it
                object->pointersInWork->destructorDelegate = nil;
#else
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

        } elseError(
            RError("RAutoPool. Bad RCompareDelegate allocation (realloc).", object)
        );

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
    if(ptr != nil) {
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
                RError1("RAutoPool. Pointer - %p wasn't allocated on RAutoPool.", object, ptr);
                deallocator(ptr);
            }
#ifdef R_POOL_DETAILED

            ifError(free == object->selfFree,
                RError("RAutoPool. Inner free is self free, bad bad bad!!!", object)
            );

            deallocator(descriptor);
#endif
            deallocator(delegate);

        } elseError(
            RError("RAutoPool. Bad RCompareDelegate allocation (free).", object)
        );

        backPtrs();
        RMutexUnlockPool();
    } else {
        RWarning("RAutoPool. Free nil.", object);
    }
}

#ifdef R_POOL_DETAILED

static RThreadId storedId = 0;

rbool deleterEnumerator(pointer object, size_t iterator) {
    RPoolDescriptor *temp = object;
    if(temp->allocatorThread == storedId) {
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
    storedId = currentTreadIdentifier();
    delegate.virtualCheckObject = deleterEnumerator;
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
