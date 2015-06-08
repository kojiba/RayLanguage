#include <RThreadPool.h>

#ifndef RAY_EMBEDDED

#include <RClassTable.h>

#define threadPoolMutex &object->mutex

struct RThreadPool {
    size_t classId;

    REnumerateDelegate enumerator;
    RThreadFunction    delegateFunction;
    RArray            *threads;
    RMutex             mutex;
};

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

method(void, addWithArg, RThreadPool), pointer argumentForNewWorker) {
    RMutexLock(threadPoolMutex);
    RThread *newOne = allocator(RThread);
    if(newOne != nil) {
        RThreadCreate(newOne, nil, object->delegateFunction, argumentForNewWorker);
        $(object->threads, m(addObject, RArray)), newOne);

    } elseError( RError("RThreadPool. Add with arg bad worker allocation.", object) );
    RMutexUnlock(threadPoolMutex);
}

method(void, addWorker,  RThreadPool), RThread *worker) {
    RMutexLock(threadPoolMutex);
    $(object->threads, m(addObject, RArray)), worker);
    RMutexUnlock(threadPoolMutex);
}

rbool joinThreadCheck(pointer context, pointer thread, size_t iterator) {
    RThreadJoin(thread);
    return yes;
}

method(void, join, RThreadPool)) {
    RMutexLock(threadPoolMutex);
    object->enumerator.virtualEnumerator = joinThreadCheck;
    $(object->threads, m(enumerate, RArray)), &object->enumerator, yes);
    RMutexUnlock(threadPoolMutex);
}

#endif