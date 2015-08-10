/**
 * RArray_Parallel.c
 * Threaded optimisation for RArray functions.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2/27/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RArray_Parallel.h"

#ifndef RAY_EMBEDDED

#ifdef RAY_ARRAY_THREAD_SAFE
    #define arrayMutex &object->mutex
    #define RMutexLockArray() RMutexLock(arrayMutex)
    #define RMutexUnlockArray() RMutexUnlock(arrayMutex)
#else
    #include "RayFoundation/RThread/RThread.h"
    // sets empty
    #define arrayMutex
    #define RMutexLockArray(some)
    #define RMutexUnlockArray(some)
#endif

typedef struct finderArgument {
    // data pointers
    RCompareDelegate *delegate;
    RArray const *object;

    // directly structures
    RRange           partRange;
    RFindResult      result;
    unsigned         selfIterator;

    // global flags
    unsigned        *signaled;
    rbool           *isFound;
} finderArgument;

void privatePartFinder(finderArgument *argument) {
    size_t iterator;
    argument->result.object = nil;
    inRange(iterator, argument->partRange) {
        if(*(argument->isFound) == yes) {
            argument->selfIterator = 0;
            return;
        }
        if (argument->delegate->virtualCompareMethod(argument->delegate->etaloneObject, argument->object->array[iterator]) == equals) {

            argument->result.index  = iterator;
            argument->result.object = argument->object->array[iterator];

            *(argument->signaled)  = argument->selfIterator - 1;
            *(argument->isFound)   = yes;
            argument->selfIterator = 0;
            return;
        }
    }
    argument->selfIterator = 0;
    return;
}

method(RFindResult, findObjectParallel, RArray),    RCompareDelegate *delegate) {
    RFindResult result;
    result.index  = object->count;
    result.object = nil;

    if(delegate != nil) {
        unsigned coreCount = processorsCount();

        unsigned *signaled = allocator(unsigned);
        rbool    *isFound  = allocator(rbool);
        finderArgument    *arguments = arrayAllocator(finderArgument, coreCount);

        if(signaled != nil
                && isFound   != nil
                && arguments != nil) {
            unsigned iterator;
            size_t partForCore = object->count / coreCount;
            size_t additionalForLastCore = object->count - partForCore * coreCount;

            *signaled = coreCount;
            rbool allNotFound = no;
            *isFound = no;

#ifdef RAY_SHORT_DEBUG
            RPrintf("RArray findObjectParallel of %p\n", object);
#endif
            RMutexLockArray();

            forAll(iterator, coreCount) {
                RThread descriptor;

                arguments[iterator].delegate      = delegate;
                arguments[iterator].object        = object;
                arguments[iterator].selfIterator  = iterator + 1;
                arguments[iterator].signaled      = signaled;
                arguments[iterator].isFound       = isFound;
                arguments[iterator].result.object = nil;

                if (iterator != coreCount - 1) {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore);
                } else {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore + additionalForLastCore);
                }
                RThreadCreate(&descriptor, nil, (RThreadFunction) privatePartFinder, &arguments[iterator]);
            }

            // wait while not found or all threads not found
            while(!allNotFound && ((*isFound) == no)) {
                allNotFound = yes;
                forAll(iterator, coreCount) {
                    if (arguments[iterator].selfIterator != 0) {
                        allNotFound = no;
                    }
                }
            }
            if ((*signaled) != coreCount && (*signaled) < coreCount) {
                *isFound = yes;
                result.index = arguments[(*signaled)].result.index;
                result.object = arguments[(*signaled)].result.object;
            }

            // white free before all not found
            while(!allNotFound) {
                allNotFound = yes;
                forAll(iterator, coreCount) {
                    if (arguments[iterator].selfIterator != 0) {
                        allNotFound = no;
                    }
                }
            }
            RMutexUnlockArray();

            // free temp pointers
            deallocator(signaled);
            deallocator(isFound);
            deallocator(arguments);
        }
    } else {
        RWarning("RArray_Parallel. Delegate for searching is nil.", object);
    }
    return result;
}

typedef struct executerArgument {
    // data pointers
    REnumerateDelegate *delegate;
    RArray const *object;

    // range
    RRange           partRange;
} executerArgument;

void privatePartExecuter(executerArgument *argument) {
    size_t iterator;
    inRange(iterator, argument->partRange) {
        argument->delegate->virtualEnumerator(argument->delegate->context, argument->object->array[iterator], iterator);
    }
}

method(void, executeParallel, RArray), REnumerateDelegate *delegate) {
    if(delegate != nil) {
        unsigned coreCount = processorsCount();

        executerArgument  *arguments   = arrayAllocator(finderArgument, coreCount);
        RThread *descriptors = arrayAllocator(RThread, coreCount);

        if(arguments != nil
                && descriptors != nil) {
            unsigned iterator;
            size_t partForCore = object->count / coreCount;
            size_t additionalForLastCore = object->count - partForCore * coreCount;

#ifdef RAY_SHORT_DEBUG
            RPrintf("RArray executeParallel of %p\n", object);
#endif
            RMutexLockArray();

            forAll(iterator, coreCount) {
                arguments[iterator].delegate = delegate;
                arguments[iterator].object   = object;

                if (iterator != coreCount - 1) {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore);
                } else {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore + additionalForLastCore);
                }
                RThreadCreate(&descriptors[iterator], nil, (RThreadFunction) privatePartExecuter, &arguments[iterator]);
            }

            // join all threads
            forAll(iterator, coreCount) {
                RThreadJoin(&descriptors[iterator]);
            }

            RMutexUnlockArray();

            // free temp pointers
            deallocator(arguments);
            deallocator(descriptors);
        }
    } else {
        RWarning("RArray_Parallel. Delegate for execute operation is nil.", object);
    }
}

#endif