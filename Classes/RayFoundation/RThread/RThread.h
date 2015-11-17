/**
 * RThread.h
 * Simple compile-based wrapper posix and winapi thread dependency.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#ifndef __R_THREAD_H__
#define __R_THREAD_H__

#include "RayFoundation/RSyntax.h"

#ifndef RAY_EMBEDDED
#ifndef __WIN32
    #include <pthread.h>

    // types
    typedef pthread_t              RThread;
    typedef pthread_attr_t*        RThreadAttributes;

    typedef pthread_mutex_t        RMutex;
    typedef pthread_mutexattr_t    RMutexAttributes;

    typedef uint64_t               RThreadId;
    typedef pointer             (* RThreadFunction)(pointer);
    typedef pthread_cond_t         RCondition;

    #ifdef RAY_BLOCKS_ON
        typedef pointer (^RThreadBlock)(pointer);
    #endif

    // initializers
    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RStackConditionInitializer       PTHREAD_COND_INITIALIZER

    // mutex types
    #define RMutexRecursive                  PTHREAD_MUTEX_RECURSIVE
    #define RMutexNormal                     PTHREAD_MUTEX_NORMAL
    #define RMutexErrorCheck                 PTHREAD_MUTEX_ERRORCHECK
#else
    #include <windows.h>
    #include <winbase.h>

    struct RMutexImpl {
        byte   type;
        HANDLE handle;
    };

    // types
    typedef HANDLE                    RThread;
    typedef LPSECURITY_ATTRIBUTES     RThreadAttributes;
    typedef struct RMutexImpl         RMutex;
    typedef LPSECURITY_ATTRIBUTES     RMutexAttributes;
    typedef DWORD            (WINAPI* RThreadFunction)(pointer);
    typedef DWORD                     RThreadId;
    typedef CONDITION_VARIABLE        RCondition;

    #define RStackRecursiveMutexInitializer  PTHREAD_RECURSIVE_MUTEX_INITIALIZER
    #define RMutexAttributeInit              pthread_mutexattr_init
    #define RMutexAttributeSetType           pthread_mutexattr_settype

    #define RMutexRecursive                  1
    #define RMutexNormal                     2
    #define RMutexErrorCheck                 3
    // fixme in progress
#endif

#pragma mark Info

extern RThread   currentThread();
extern RThreadId currentThreadIdentifier(); // returns caller thread unique identifier
extern unsigned  processorsCount();         // returns cores count

#pragma mark Thread

extern int  RThreadCreate (RThread *thread,
                           RThreadAttributes attributes,
                           RThreadFunction function,
                           pointer argument);

extern int  RThreadCancel (RThread thread);
extern int  RThreadKill   (RThread thread);
extern int  RThreadJoin   (RThread thread);
extern void RThreadExit   (pointer data);
extern
RThreadId RThreadIdOfThread(RThread thread);
extern
RThread makeRThread(RThreadFunction function);

#ifdef RAY_BLOCKS_ON
    extern RThread makeRThreadWithBlock(pointer argument, RThreadBlock block);
    extern RThread RThreadWithBlock(void(^block)(void));
#endif

#pragma mark Mutex

extern int mutexWithType (RMutex *mutex, byte mutexType);
extern int RMutexLock    (RMutex *mutex);
extern int RMutexUnlock  (RMutex *mutex);
extern int RMutexTryLock (RMutex *mutex);
extern int RMutexDestroy (RMutex *mutex);

extern rbool lockOrDeadlocked(RMutex *mutex); // no if deadlocked

#pragma mark Conditions

extern int RConditionInit      (RCondition *condition);
extern int RConditionSignal    (RCondition *condition);
extern int RConditionBroadcast (RCondition *condition);
extern int RConditionWait      (RCondition *condition, RMutex *mutex);
extern int RConditionWaitTimed (RCondition *condition, RMutex *mutex, long milliseconds);
extern int RConditionDestroy   (RCondition *condition);



#endif /* RAY_EMBEDDED */

#endif /*__R_THREAD_H__*/

