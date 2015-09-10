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

#include "RThreadPool.h"

#ifndef RAY_EMBEDDED

#include "RayFoundation/RClassTable/RClassTable.h"

protocol(PrivateThreadArgument)
    RThreadFunction  delegateFunction;
            pointer  threadArgument;
endOf(PrivateThreadArgument)

#define threadPoolMutex &object->mutex

struct RThreadPool {
    size_t classId;

    REnumerateDelegate enumerator;
    RThreadFunction    delegateFunction;
    RArray            *threads;
    RArray            *arguments;
    RMutex             mutex;
};

pointer privateThreadExecutor(PrivateThreadArgument *arg) {
    pointer result = arg->delegateFunction(arg->threadArgument);
    $(arg->context, m(deleteWorker, RThreadPool)), currentThread());
    $(((RThreadPool*)arg->context)->arguments, m(deleteObjectFast, RArray)), arg);
    return result;
}

constructor(RThreadPool)) {
    object = allocator(RThreadPool);
    if(object != nil) {

        object->threads   = makeRArray();
        object->arguments = makeRArray();

        if(object->threads != nil && object->arguments != nil) {

            $(object->arguments, m(setDestructorDelegate, RArray)), getRFree());

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
    deleter(object->threads,   RArray);
    deleter(object->arguments, RArray);
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
    RMutexLock(threadPoolMutex);
    if(object->delegateFunction != nil) {
        RThread newOne = nil;
        if(selfDeletes) {
            PrivateThreadArgument *arg = allocator(PrivateThreadArgument);
            if(arg != nil) {

                arg->threadArgument   = argumentForNewWorker;
                arg->delegateFunction = object->delegateFunction;
                arg->context          = object;

                RThreadCreate(&newOne, nil, (RThreadFunction) privateThreadExecutor, arg);
                $(object->threads,   m(addObject, RArray)), newOne);
                $(object->arguments, m(addObject, RArray)), arg);

            } elseError(
                    RError("RThreadPool. Add with arg bad arg allocation.", object)
            );
        } else {
            RThreadCreate(&newOne, nil, object->delegateFunction, argumentForNewWorker);
        }
    }
    RMutexUnlock(threadPoolMutex);
}

method(void, addWorker, RThreadPool), RThread worker) {
    RMutexLock(threadPoolMutex);
    $(object->threads, m(addObject, RArray)), worker);
    RMutexUnlock(threadPoolMutex);
}

method(void, deleteWorker, RThreadPool), RThread worker) {
    RMutexLock(threadPoolMutex);
    $(object->threads, m(deleteObjectFast, RArray)), worker);
    RMutexUnlock(threadPoolMutex);
}

rbool joinThreadCheck(pointer context, RThread thread, size_t iterator) {
    RThreadJoin(thread);
    return yes;
}

method(void, joinSelfDeletes, RThreadPool)) {
    while(object->threads->count);
}

method(void, join, RThreadPool)) {
    RMutexLock(threadPoolMutex);
    object->enumerator.virtualEnumerator = (EnumeratorDelegate) joinThreadCheck;
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
    $(object->threads,   m(enumerate, RArray)), &object->enumerator, yes);
    $(object->threads,   m(flush,     RArray)));
    $(object->arguments, m(flush,     RArray)));
    RMutexUnlock(threadPoolMutex);
}

#endif