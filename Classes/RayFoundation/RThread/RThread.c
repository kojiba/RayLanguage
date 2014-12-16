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

#include <RClassTable.h>
#include <RThread.h>

constructor(RThread),
        RThreadAttributes *attributes,
        pointer (*threadFunction)(pointer),
        pointer argument ) {

    object = allocator(RThread);
    if(object != nil) {
            #ifndef __WIN32
                object->descriptor = allocator(RThreadDescriptor);
                if(object->descriptor != nil) {
                    if(pthread_create(object->descriptor, attributes, threadFunction, argument) != 0) {
                        RError("RThread. Bad pthread_create.", object);
                        return nil;
                    }
            #else
                object->descriptor = CreateThread(NULL, 0, threadFunction, argument, 0, NULL);
                if(object->descriptor != nil) {
            #endif
                    object->classId = registerClassOnce(toString(RThread));
        } else {
            RError("RThread. Bad creation/allocation of thread descriptor.", object);
        }
    }
    return object;
}

destructor(RThread) {
    deallocator(object->descriptor);
}

printer(RThread) {
    RPrintf("%s object - %p, RThreadDescriptor - %p\n", toString(RThread), object, object->descriptor);
}

method(void, cancell, RThread)) {
#ifndef __WIN32
    pthread_cancel(*object->descriptor);
#else
    TerminateThread(*object->descriptor, nil);
#endif
}

method(void, join, RThread)) {
#ifndef __WIN32
    pthread_join(*object->descriptor, nil);
#else
    WaitForSingleObject(*object->descriptor, INFINITE);
#endif
}
