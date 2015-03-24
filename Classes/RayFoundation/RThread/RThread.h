/**
 * RThread.h
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

#ifndef __R__THREAD_H__
#define __R__THREAD_H__

#include <RSyntax.h>

#ifndef __WIN32
    #include <pthread.h>

    // types
    typedef pthread_t              RThread;
    typedef pthread_attr_t         RThreadAttributes;

    typedef pthread_mutex_t        RMutex;
    typedef pthread_mutexattr_t    RMutexAttributes;

    typedef pthread_cond_t         RCondition;
    typedef uint64_t               RThreadId;
    typedef pointer             (* RThreadFunction)(pointer);

    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype
    #define RConditionSignal                 pthread_cond_signal
    #define RConditionWait                   pthread_cond_wait

    // initializers
    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RStackConditionInitializer       PTHREAD_COND_INITIALIZER

    // mutex types
    #define RMutexRecursive                  PTHREAD_MUTEX_RECURSIVE
    #define RMutexNormal                     PTHREAD_MUTEX_NORMAL
    #define RMutexErrorCheck                 PTHREAD_MUTEX_ERRORCHECK
#else
    #include <windows.h>

    // types
    typedef HANDLE                    RThread;
    typedef LPVOID                    RThreadAttributes;
    typedef HANDLE                    RMutex;
    typedef LPSECURITY_ATTRIBUTES     RMutexAttributes;
    typedef DWORD                  (* RThreadFunction)(pointer);
    typedef uint64_t                  RThreadId;

    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype

    #define RMutexRecursive                  1
    #define RMutexNormal                     2
    #define RMutexErrorCheck                 3
    // fixme in progress
#endif

#pragma mark Info

RThreadId currentTreadIdentifier(); // returns caller thread unique identifier
unsigned  processorsCount();        // returns cores count

#pragma mark Thread

int RThreadCreate(RThread *thread,
                  RThreadAttributes *attributes,
                  RThreadFunction function,
                  pointer argument);

int  RThreadCancel(RThread *thread);
int  RThreadJoin  (RThread *thread);
void RThreadExit  (pointer data);

#pragma mark Mutex

RMutex mutexWithType(byte mutexType);

int RMutexLock  (RMutex *mutex);
int RMutexUnlock(RMutex *mutex);

#endif /*__R__THREAD_H__*/
