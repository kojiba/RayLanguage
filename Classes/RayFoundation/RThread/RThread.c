/**
 * RThread.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/16/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RThread.h>
#include <unistd.h>

#pragma mark Info

RThreadId currentTreadIdentifier() {
    RThreadId threadId = 0;
#ifndef __WIN32
    pthread_threadid_np(pthread_self(), &threadId);
#else
    #warning fixme currentTreadIdentifier will always 0
#endif
    return threadId;
}

unsigned processorsCount() {
#ifdef __WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;
#else
    return (unsigned) sysconf( _SC_NPROCESSORS_ONLN );
#endif
}

#pragma mark Thread

int RThreadCreate(RThread *thread,
                  RThreadAttributes *attributes,
                  RThreadFunction function,
                  pointer argument ) {

#ifndef __WIN32
    return pthread_create(thread, attributes, function, argument);
#else
    *thread = CreateThread(nil, 0, function, argument, 0, nil);
#endif
}

int RThreadCancel(RThread *thread) {
#ifndef __WIN32
    return pthread_cancel(*thread);
#else
    return TerminateThread(thread, nil);
#endif
}

int RThreadJoin(RThread *thread) {
#ifndef __WIN32
    return pthread_join(*thread, nil);
#else
    return WaitForSingleObject(object->descriptor, INFINITE);
#endif
}

void RThreadExit(pointer data) {
#ifndef _WIN32
    pthread_exit(data);
#else
    ExitThread(data);
#endif
}

#pragma mark Mutex

RMutex mutexWithType(byte mutexType) {
    RMutex mutex;
#ifndef _WIN32
    RMutexAttributes Attr;
    pthread_mutexattr_init(&Attr);
    pthread_mutexattr_settype(&Attr, mutexType);
    if(pthread_mutex_init(&mutex,  &Attr) != 0) {
        RError("RMutexWithType. Error creating mutex with type.", &mutex);
    }
#else
    mutex = RMutexInit(nil, no, nil);
#endif

    return mutex;
}

int RMutexLock(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_lock(mutex);
#else
    return WaitForSingleObject(mutex, INFINITE);
#endif
}

int RMutexUnlock(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_unlock(mutex);
#else
    return ReleaseMutex(mutex);
#endif
}
