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

#define storePtrs() pointer (*oldMalloc) (size_t size) = RMallocPtr;\
                    pointer (*oldRealloc)(pointer ptr, size_t oldSize) = RReallocPtr;\
                    pointer (*oldCalloc) (size_t size, size_t blockSize) = RCallocPtr;\
                    void    (*oldFree)   (pointer ptr) = RFreePtr

#define backPtrs()  RMallocPtr = oldMalloc;\
                    RReallocPtr = oldRealloc;\
                    RCallocPtr = oldCalloc;\
                    RFreePtr = oldFree

#define toPoolPtrs() RMallocPtr = object->innerMalloc;\
                     RReallocPtr = object->innerRealloc;\
                     RCallocPtr = object->innerCalloc;\
                     RFreePtr = object->innerFree


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
            object->pointersInWork->destructorDelegate = object->innerFree;
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
    RPrintf("%s object - %p -------\n", toString(RAutoPool), object);
    RPrintf("Pointers array: ");
    $(object->pointersInWork, p(RArray)));
    RPrintf("---------------------- %p\n", object);
    backPtrs();
}

method(pointer, malloc, RAutoPool), size_t sizeInBytes) {
    storePtrs();
    toPoolPtrs();
    pointer temp = RAlloc(sizeInBytes);
    if(temp != nil) {
        $(object->pointersInWork, m(addObject, RArray)), temp);
    }
    backPtrs();
    return temp;
}

method(pointer, realloc, RAutoPool), pointer ptr, size_t newSize) {
    if(ptr == nil) {
        return $(object, m(malloc, RAutoPool)), newSize);
    } else {
        storePtrs();
        toPoolPtrs();
        RCompareDelegate *delegate = allocator(RCompareDelegate);
        if(delegate != nil) {
            delegate->etaloneObject = ptr;
            delegate->virtualCompareMethod = nil;
            // search if it not first realloc for ptr, not destruct
            RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
            if(result.object != nil) {
                object->pointersInWork->destructorDelegate = nil;
                $(object->pointersInWork, m(deleteObjectAtIndex, RArray)), result.index);
                object->pointersInWork->destructorDelegate = object->innerFree;

            }

            pointer temp = RReAlloc(ptr, newSize);

            // finally add new pointer
            $(object->pointersInWork, m(addObject, RArray)), temp);

            deallocator(delegate);
            backPtrs();
            return temp;
        } else {
            RError("RAutoPool. Bad RCompareDelegate allocation on realloc.", object);
        }
        backPtrs();
    }
    return nil;
}

method(pointer, calloc, RAutoPool), size_t blockCount, size_t blockSize) {
    storePtrs();
    toPoolPtrs();
    // high lvl calloc
    pointer temp = RClearAlloc(blockCount, blockSize);
    $(object->pointersInWork, m(addObject, RArray)), temp);
    backPtrs();
    return temp;
}

method(void, free, RAutoPool), pointer ptr) {
    storePtrs();
    toPoolPtrs();
    RCompareDelegate *delegate = allocator(RCompareDelegate);
    if(delegate != nil) {
        delegate->etaloneObject = ptr;
        delegate->virtualCompareMethod = nil;
        // search ptr
        RFindResult result = $(object->pointersInWork, m(findObjectWithDelegate, RArray)), delegate);
        if(result.object != nil) {
            $(object->pointersInWork, m(deleteObjectAtIndex, RArray)), result.index);
        } else {
            RErrStr "RAutoPool. Pointer - %p wasn't allocated on RAutoPool - %p\n", ptr, object);
        }
    } else {
        RError("RAutoPool. Bad RCompareDelegate allocation on free.", object);
    }
    backPtrs();
}

method(void, drain, RAutoPool)) {
    $(object->pointersInWork, m(flush, RArray)));
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
