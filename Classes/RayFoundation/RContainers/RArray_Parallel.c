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
    // data pointers
    RCompareDelegate *delegate;
    RArray           *object;

    // directly structures
    RRange           partRange;
    RFindResult      result;
    unsigned         selfIterator;

    // global flags
    unsigned        *signaled;
    rbool           *isFound;
} enumeratorArgument;

void privatePartEnumerator(enumeratorArgument *argument) {
    size_t iterator;
    argument->result.object = nil;
    inRange(iterator, argument->partRange) {
        if(*(argument->isFound) == yes) {
            argument->selfIterator = 0;
            RThreadExit(nil);
        }
        if ($(argument->delegate, m(checkObject, RCompareDelegate)), argument->object->array[iterator]) == equals) {

            argument->result.index  = iterator;
            argument->result.object = argument->object->array[iterator];

            *(argument->signaled)  = argument->selfIterator - 1;
            *(argument->isFound)   = yes;
            argument->selfIterator = 0;
            RThreadExit(nil);
        }
    }
    argument->selfIterator = 0;
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
        enumeratorArgument *arguments = RAlloc(sizeof(enumeratorArgument) * coreCount);

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
                RThreadDescriptor descriptor;

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
                #ifndef __WIN32
                RThreadCreate(&descriptor, nil, (pointer (*)(pointer)) privatePartEnumerator, &arguments[iterator]);
                #else
                    #warning FIXME threads
                #endif
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
        RWarning("RArray. Delegate for searching is nil.", object);
    }
    return result;
}