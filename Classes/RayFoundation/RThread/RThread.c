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
#include <RClassTable.h>

#pragma mark Base

RMutexDescriptor mutexWithType(unsigned short mutexType) {
    RMutexDescriptor mutex;

    #ifndef _WIN32
        RMutexAttributes Attr;
        RMutexAttributeInit(&Attr);
        RMutexAttributeSetType(&Attr, mutexType);
        if(RMutexInit(&mutex,  &Attr) != 0) {
            RError("RMutexWithType. Error creating mutex with type.", &mutex);
        }
    #else
        mutex = RMutexInit(nil, no, nil);
    #endif

    return mutex;
}

#pragma mark Wrapper

constructor(RThread),
        RThreadAttributes *attributes,
        RThreadFunction function,
        pointer argument ) {

    object = allocator(RThread);
    if(object != nil) {
            #ifndef __WIN32
                    if(pthread_create(&object->descriptor, attributes, function, argument) != 0) {
                        RError("RThread. Bad pthread_create.", object);
                        return nil;
                    }
            #else
                object->descriptor = CreateThread(nil, 0, function, argument, 0, nil);
                if(object->descriptor != nil) {
            #endif
                    object->classId = 6; //registerClassOnce(toString(RThread));
    }
    return object;
}

printer(RThread) {
    RPrintf("%s object - %p, RThreadDescriptor - %p\n", toString(RThread), object, object->descriptor);
}

method(void, cancel, RThread)) {
#ifndef __WIN32
    pthread_cancel(object->descriptor);
#else
    TerminateThread(object->descriptor, nil);
#endif
}

method(void, join, RThread)) {
#ifndef __WIN32
    pthread_join(object->descriptor, nil);
#else
    WaitForSingleObject(object->descriptor, INFINITE);
#endif
}
