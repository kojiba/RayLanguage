/**
 * RThread.h
 * Simple compile-based stdlib(winapi) thread dependency.
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
    typedef pthread_t       RThreadDescriptor;
    typedef pthread_attr_t  RThreadAttributes;
    typedef pthread_mutex_t RThreadMutex;
#else
    #include <windows.h>

    typedef HANDLE RThreadDescriptor;
    // fixme windows
    typedef LPVOID  RThreadAttributes;
    typedef HANDLE RThreadMutex;
#endif

class(RThread)
    RThreadDescriptor *descriptor;
endOf(RThread)

constructor (RThread),
        RThreadAttributes *attributes,
        pointer (*threadFunction)(pointer),
        pointer argument);

destructor  (RThread);
printer     (RThread);

method(void, cancel, RThread));
method(void, join,    RThread));

#endif /*__R__THREAD_H__*/
