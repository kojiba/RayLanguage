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

#include "RayFoundation/RSyntax.h"
#ifndef RAY_EMBEDDED
#include "RayFoundation/RContainers/RArray.h"

#ifndef RAY_ARRAY_THREAD_SAFE
    #include "RThread.h"
#endif

typedef struct RThreadPool RThreadPool;

constructor(RThreadPool));
destructor (RThreadPool);
printer    (RThreadPool);

method(void,            setDelegateFunction, RThreadPool),     RThreadFunction delegateFunction);
method(RThreadFunction, delegateFunction,    RThreadPool));

// selfDeletes - remove itself record from pool, when finished
method(void,            addWithArg,          RThreadPool),     pointer argumentForNewWorker, rbool selfDeletes);
method(void,            addWorker,           RThreadPool),     RThread worker);
method(void,            deleteWorker,        RThreadPool),     RThread worker);

method(void,            join,                RThreadPool));
method(void,            joinSelfDeletes,     RThreadPool));

method(void,            cancel,              RThreadPool));

#endif /* RAY_EMBEDDED */

#endif /*__R_THREAD_POOL_H__*/
