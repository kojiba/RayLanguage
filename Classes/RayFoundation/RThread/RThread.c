/**
 * RThread.c
 * Simple compile-based wrapper posix and winapi thread dependency.
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

#ifndef RAY_EMBEDDED

#include <unistd.h>

#pragma mark Info

inline RThread currentThread() {
#ifndef __WIN32
    return pthread_self();
#else
    return GetCurrentThread();
#endif
}

inline RThreadId currentThreadIdentifier() {
#ifndef __WIN32
    RThreadId threadId = 0;
    pthread_threadid_np(pthread_self(), &threadId);
    return threadId;
#else
    return GetCurrentThreadId();
#endif
}

inline unsigned processorsCount() {
#ifdef __WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo( &sysinfo );
    return sysinfo.dwNumberOfProcessors;
#else
    return (unsigned) sysconf( _SC_NPROCESSORS_ONLN );
#endif
}

#pragma mark Thread

inline int RThreadCreate(RThread *thread,
                  RThreadAttributes attributes,
                  RThreadFunction function,
                  pointer argument ) {

#ifndef __WIN32
    return pthread_create(thread, attributes, function, argument);
#else
        *thread = CreateThread(attributes, 0, function, argument, 0, nil);
        return *thread != nil;
#endif
}

inline int RThreadCancel(RThread *thread) {
#ifndef __WIN32
    return pthread_cancel(*thread);
#else
    return (int)TerminateThread(thread, 0);
#endif
}

inline int RThreadKill(RThread *thread) {
#ifndef __WIN32
    return pthread_kill(*thread, SIGKILL);
#else
    return (int)TerminateThread(thread, 0);
#endif
}

inline int RThreadJoin(RThread *thread) {
#ifndef __WIN32
    return pthread_join(*thread, nil);
#else
    return (int)WaitForSingleObject(*thread, INFINITE);
#endif
}

inline void RThreadExit(pointer data) {
#ifndef _WIN32
    pthread_exit(data);
#else
    ExitThread((DWORD) data);
#endif
}

extern RThreadId RThreadIdOfThread(RThread *thread) {
#ifndef _WIN32
    RThreadId result;
    pthread_threadid_np(*thread, &result);
    return result;
#else
    return GetThreadId(*thread);
#endif
}

#pragma mark Mutex

inline int mutexWithType(RMutex *mutex, byte mutexType) {
#ifndef _WIN32
    RMutexAttributes Attr;
    pthread_mutexattr_init(&Attr);
    pthread_mutexattr_settype(&Attr, mutexType);
    return pthread_mutex_init(mutex,  &Attr);
#else
    *mutex = CreateMutex(nil, no, nil);
    return 0;
#endif
}

inline int RMutexLock(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_lock(mutex);
#else
    return (int) WaitForSingleObject(mutex, INFINITE);
#endif
}

inline int RMutexTryLock(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_trylock(mutex);
#else
    return (int) WaitForSingleObject(mutex, INFINITE);
#endif
}

inline int RMutexUnlock(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_unlock(mutex);
#else
    return ReleaseMutex(mutex);
#endif
}

inline int RMutexDestroy(RMutex *mutex) {
#ifndef _WIN32
    return pthread_mutex_destroy(mutex);
#else
    return CloseHandle(mutex);
#endif
}

#endif
