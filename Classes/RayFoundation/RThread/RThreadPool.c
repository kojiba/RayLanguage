/**
 * RThreadPool.c
 * Thread pool realization.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 1/29/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RThreadPool.h>

#ifndef RAY_EMBEDDED

#include <RClassTable.h>

protocol(PrivateThreadArgument)
    RThreadFunction delegate;
    pointer threadArgument;
    RThread *pointerToSelf;
endOf(PrivateThreadArgument)

#define threadPoolMutex &object->mutex

struct RThreadPool {
    size_t classId;

    REnumerateDelegate enumerator;
    RThreadFunction    delegateFunction;
    RArray            *threads;
    RMutex             mutex;
};

pointer privateThreadExecutor(pointer arg) {
    PrivateThreadArgument *temp = arg;
    pointer result = temp->delegate(temp->threadArgument);

    $(temp->context, m(deleteWorker, RThreadPool)), temp->pointerToSelf);
    deallocator(arg);
    return result;
}

constructor(RThreadPool)) {
    object = allocator(RThreadPool);
    if(object != nil) {
        object->threads = makeRArray();
        if(object->threads != nil) {
            object->threads->destructorDelegate = getRFree();
            object->classId = registerClassOnce(toString(RThreadPool));
            mutexWithType(threadPoolMutex, RMutexNormal);
        } else {
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(RThreadPool) {
    RMutexLock(threadPoolMutex);
    deleter(object->threads, RArray);
    RMutexUnlock(threadPoolMutex);
    RMutexDestroy(threadPoolMutex);
}

printer(RThreadPool) {
    RMutexLock(threadPoolMutex);
    RPrintf("%s object %p {", toString(RThreadPool), object);
    $(object->threads, p(RArray)));
    RPrintf("} end of %s\n", toString(RThreadPool));
    RMutexUnlock(threadPoolMutex);
}

method(void, setDelegateFunction, RThreadPool), RThreadFunction delegateFunction) {
    RMutexLock(threadPoolMutex);
    object->delegateFunction = delegateFunction;
    RMutexUnlock(threadPoolMutex);
}

method(RThreadFunction, delegateFunction, RThreadPool)) {
    return object->delegateFunction;
}

method(void, addWithArg, RThreadPool), pointer argumentForNewWorker, rbool selfDeletes) {
    if(object->delegateFunction != nil) {
        RMutexLock(threadPoolMutex);
        RThread *newOne = allocator(RThread);
        if(newOne != nil) {
            if(selfDeletes) {
                PrivateThreadArgument *arg = allocator(PrivateThreadArgument);
                if(arg != nil) {

                    arg->threadArgument = argumentForNewWorker;
                    arg->delegate       = object->delegateFunction;
                    arg->context        = object;
                    arg->pointerToSelf  = newOne;

                    RThreadCreate(newOne, nil, privateThreadExecutor, arg);
                    $(object->threads, m(addObject, RArray)), newOne);

                } elseError( RError("RThreadPool. Add with arg bad arg allocation.", object) );
            } else {
                RThreadCreate(newOne, nil, object->delegateFunction, argumentForNewWorker);
            }
        } elseError( RError("RThreadPool. Add with arg bad worker allocation.", object) );
        RMutexUnlock(threadPoolMutex);
    }
}

method(void, addWorker,  RThreadPool), RThread *worker) {
    RMutexLock(threadPoolMutex);
    $(object->threads, m(addObject, RArray)), worker);
    RMutexUnlock(threadPoolMutex);
}

method(void, deleteWorker, RThreadPool), RThread *worker) {
    RCompareDelegate compareDelegate;
    RFindResult findResult;
    compareDelegate.virtualCompareMethod = defaultComparator;
    compareDelegate.etaloneObject = worker;

    RMutexLock(threadPoolMutex);

    findResult = $(object->threads, m(findObjectWithDelegate, RArray)), &compareDelegate);
    if(findResult.index != object->threads->count
        && findResult.object != nil) {
        $(object->threads, m(deleteObjectAtIndex, RArray)), findResult.index);
    } elseError(
            RError1("RThreadPool. Can't find worker %p in pool.", object, worker)
    );

    RMutexUnlock(threadPoolMutex);
}

rbool joinThreadCheck(pointer context, pointer thread, size_t iterator) {
    RThreadJoin(thread);
    return yes;
}

method(void, joinSelfDeletes, RThreadPool)) {
    while(object->threads->count);
}

method(void, join, RThreadPool)) {
    RMutexLock(threadPoolMutex);
    object->enumerator.virtualEnumerator = joinThreadCheck;
    $(object->threads, m(enumerate, RArray)), &object->enumerator, yes);
    RMutexUnlock(threadPoolMutex);
}

rbool cancelThreadCheck(pointer context, pointer thread, size_t iterator) {
    RThreadCancel(thread);
    return yes;
}

method(void, cancel, RThreadPool)) {
    RMutexLock(threadPoolMutex);
    object->enumerator.virtualEnumerator = cancelThreadCheck;
    $(object->threads, m(enumerate, RArray)), &object->enumerator, yes);
    RMutexUnlock(threadPoolMutex);
}

#endif