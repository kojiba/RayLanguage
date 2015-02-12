#include <RThreadNative.h>

RProcessId getThreadId() {
    RProcessId threadId = 0;
    #ifndef __WIN32
        pthread_threadid_np(pthread_self(), &threadId);
    #else
        // fixme
    #endif
    return threadId;
}