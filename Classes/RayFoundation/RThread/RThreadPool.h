/**
 * RThreadPool.h
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

#ifndef __R_THREAD_POOL_H__
#define __R_THREAD_POOL_H__

#include <RSyntax.h>
#include <RArray.h>

#ifndef RAY_ARRAY_THREAD_SAFE
    #include <RThread.h>
#endif

typedef struct RThreadPool RThreadPool;

constructor(RThreadPool));
destructor (RThreadPool);
printer    (RThreadPool);

method(void,            setDelegateFunction, RThreadPool),     RThreadFunction delegateFunction);
method(RThreadFunction, delegateFunction,    RThreadPool));

method(void,            addWithArg,          RThreadPool),     pointer argumentForNewWorker);
method(void,            addWorker,           RThreadPool),     RThread *worker);

method(void,            join,                RThreadPool));

#endif /*__R_THREAD_POOL_H__*/
