/**
 * RThreadNative.h
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

#ifndef __R_THREAD_NATIVE__
#define __R_THREAD_NATIVE__

#include <RSyntax.h>

#ifndef __WIN32
    #include <pthread.h>

    typedef pthread_t                        RThreadDescriptor;
    typedef pthread_attr_t                   RThreadAttributes;
    typedef pthread_mutex_t                  RMutexDescriptor;
    typedef pthread_mutexattr_t              RMutexAttributes;
    typedef uint64_t                         RProcessId;
    typedef pointer (*RThreadFunction)(pointer);

    #define RMutexInit                       pthread_mutex_init
    #define RMutexLock                       pthread_mutex_lock
    #define RMutexUnlock                     pthread_mutex_unlock
    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype

    #define RMutexRecursive                  PTHREAD_MUTEX_RECURSIVE
    #define RMutexNormal                     PTHREAD_MUTEX_NORMAL
    #define RMutexErrorCheck                 PTHREAD_MUTEX_ERRORCHECK
#else
    #include <windows.h>
    typedef HANDLE                           RThreadDescriptor;
    typedef LPVOID                           RThreadAttributes;
    typedef HANDLE                           RMutexDescriptor;
    typedef LPSECURITY_ATTRIBUTES            RMutexAttributes;
    typedef DWORD (*RThreadFunction)(pointer);

    #define RMutexInit                       CreateMutex
    #define RMutexLock(mutex)                WaitForSingleObject(mutex, INFINITE);
    #define RMutexUnlock                     ReleaseMutex

    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype

    #define RMutexRecursive                  1
    #define RMutexNormal                     2
    #define RMutexErrorCheck                 3// fixme

    // fixme in progress
#endif

RProcessId getThreadId(); // returns caller thread unique identifier

#endif /*__R_THREAD_NATIVE__*/