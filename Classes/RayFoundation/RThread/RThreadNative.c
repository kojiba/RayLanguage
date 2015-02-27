#include <RThreadNative.h>
#include <unistd.h>

RThreadId currentTreadIdentifier() {
    RThreadId threadId = 0;
    #ifndef __WIN32
        pthread_threadid_np(pthread_self(), &threadId);
    #else
        // fixme
    #endif
    return threadId;
}

long processorsCount() {
    #ifdef __WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo( &sysinfo );
        return sysinfo.dwNumberOfProcessors;
    #else
        return sysconf( _SC_NPROCESSORS_ONLN );
    #endif
}