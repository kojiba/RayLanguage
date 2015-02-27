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

#include <RArray_Parallel.h>

#ifdef RAY_ARRAY_THREAD_SAFE
    #define arrayMutex &object->mutex
    #define RMutexLockArray() RMutexLock(arrayMutex)
    #define RMutexUnlockArray() RMutexUnlock(arrayMutex)
#else
    // sets empty
    #define arrayMutex
    #define RMutexLockArray(some)
    #define RMutexUnlockArray(some)
#endif

typedef struct enumeratorArgument {
    // pointers
    RCompareDelegate *delegate;
    RArray           *object;

    // directly structures
    RRange           partRange;
    RFindResult     *result;
    unsigned        *signaled;
    unsigned        *selfIterator;
    rbool           *isFound;
} enumeratorArgument;

void privatePartEnumerator(enumeratorArgument *argument) {
    size_t iterator;
    (*argument->result).object = nil;
    inRange(iterator, argument->partRange) {
        if ($(argument->delegate, m(checkObject, RCompareDelegate)), argument->object->array[iterator]) == equals) {
            (*argument->result).index  = iterator;
            (*argument->result).object = argument->object->array[iterator];
            *argument->signaled = *argument->selfIterator;
            *argument->isFound = yes;
            RThreadExit(nil);
        }
        if((*argument->isFound) == yes) {
            RThreadExit(nil);
        }
    }
    *argument->selfIterator = 0;
    RThreadExit(nil);
}

method(RFindResult, findObjectParallel, RArray),    RCompareDelegate *delegate) {
    RFindResult result;
    result.index  = object->count;
    result.object = nil;

    if(delegate != nil) {
        unsigned coreCount = processorsCount();

        unsigned    *signaled  = allocator(unsigned);
        rbool       *isFound   = allocator(rbool);
        unsigned    *iterators = RAlloc(sizeof(unsigned) * coreCount);
        RFindResult *results   = RAlloc(sizeof(RFindResult) * coreCount);

        if(signaled != nil
                && iterators != nil
                && isFound   != nil
                && results   != nil) {
            unsigned iterator;
            size_t partForCore = object->count / coreCount;
            size_t additionalForLastCore = object->count - partForCore * coreCount;
            enumeratorArgument *arguments = RAlloc(sizeof(enumeratorArgument) * coreCount);
            rbool allNotFound = yes;
            *isFound = no;

#ifdef RAY_SHORT_DEBUG
            RPrintf("RArray findObjectParallel of %p\n", object);
#endif
            RMutexLockArray();


            forAll(iterator, coreCount) {
                RThreadDescriptor descriptor;
                iterators[iterator] = iterator;

                arguments[iterator].delegate     = delegate;
                arguments[iterator].object       = object;
                arguments[iterator].selfIterator = &iterators[iterator];
                arguments[iterator].signaled     = signaled;
                arguments[iterator].isFound      = isFound;
                arguments[iterator].result       = &results[iterator];

                if (iterator != coreCount - 1) {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore);
                } else {
                    arguments[iterator].partRange = makeRRange(iterator * partForCore, partForCore + additionalForLastCore);
                }
                RThreadCreate(&descriptor, nil, (pointer (*)(pointer)) privatePartEnumerator, &arguments[iterator]);
            }

            // wait while not found or all threads not found
            while(allNotFound && ((*isFound) == no)) {
                allNotFound = yes;
                forAll(iterator, coreCount) {
                    if (iterators[iterator] != 0) {
                        allNotFound = no;
                    }
                }
            }
            if ((*signaled) != coreCount) {
                *isFound = yes;
                result.index = results[(*signaled)].index;
                result.object = results[(*signaled)].object;
            }

            // white before free isFound
            while(allNotFound) {
                forAll(iterator, coreCount) {
                    if (iterators[iterator] != 0) {
                        allNotFound = no;
                    }
                }
            }
            RMutexUnlockArray();

            // free temp pointers
            RFree(signaled);
            RFree(isFound);
            RFree(iterators);
            RFree(results);
        }
    } else {
        RWarning("RArray. Delegate for searching is nil." , object);
    }
    return result;
}