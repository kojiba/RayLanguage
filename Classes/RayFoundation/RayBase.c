/**
 * RayBase.c
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 12/10/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RayBase.h"

#ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR // ---------------------------------------

    #include <dlfcn.h>
    #include "RayFoundation/RMemoryOperations/RAutoPool.h"

    #define saveStdLibFunction(var, f) do { \
            var = dlsym(RTLD_NEXT, f);\
            if (NULL == var) \
                perror("dlsym");\
        } while(0)

    // stdlib functions by default
    volatile Allocator   RMallocPtr  = 0;
    volatile Callocator  RCallocPtr  = 0;
    volatile Reallocator RReallocPtr = 0;
    volatile Deallocator RFreePtr    = 0;

    static unsigned char onceStoreFlag = 1;

    autoPoolNamed(PreloadPool, startSizeOfRPoolDefault, sizeMultiplierOfRPoolDefault);

    void printRPoolAtExitInjection() {
        p(RAutoPool)(PreloadPool());
        deleter(PreloadPool(), RAutoPool);
    }

    void printPool(int signum) {
        p(RAutoPool)(PreloadPool());
    }

    void initLibPtrs() {
        saveStdLibFunction(RTrueMalloc, "malloc");
        if(RMallocPtr == 0) {
            RMallocPtr = RTrueMalloc;
        }

        saveStdLibFunction(RTrueCalloc, "calloc");
        if(RCallocPtr == 0) {
            RCallocPtr = RTrueCalloc;
        }

        saveStdLibFunction(RTrueRealloc, "realloc");
        if(RReallocPtr == 0) {
            RReallocPtr = RTrueRealloc;
        }

        saveStdLibFunction(RTrueFree, "free");
        if(RFreePtr == 0) {
            RFreePtr = RTrueFree;
        }
        atexit(printRPoolAtExitInjection);

        signal(SIGUSR1, printPool);
        signal(SIGTERM, printPool);
        signal(SIGKILL, printPool);
        signal(SIGQUIT, printPool);


        onceStoreFlag = 0;
        enablePool(PreloadPool());
    }

    void* malloc(size_t size) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
        pointer ptr = RMallocPtr(size);
    #ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR_TRACE
        RPrintf("malloc %p [%lu]\n", ptr, size);
    #endif
        return ptr;
    }

    void* calloc(size_t size1, size_t size2) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
        pointer ptr = RCallocPtr(size1, size2);
    #ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR_TRACE
        RPrintf("calloc %p [%lu x %lu]\n", ptr, size1, size2);
    #endif
        return ptr;
    }

    void* realloc(void* ptr, size_t size) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
        pointer ptr2 = RReallocPtr(ptr, size);
    #ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR_TRACE
        RPrintf("realloc %p to %p [%lu]\n", ptr, ptr2, size);
    #endif
        return ptr2;
    }

    void  free(void* ptr) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
    #ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR_TRACE
        RPrintf("free %p\n", ptr);
    #endif
        return RFreePtr(ptr);
    }

#else // ----------------------------------------------------------------------

    #undef malloc
    #undef realloc
    #undef calloc
    #undef free

    // stdlib functions by default
    volatile Allocator   RMallocPtr  = &malloc;
    volatile Callocator  RCallocPtr  = &calloc;
    volatile Reallocator RReallocPtr = &realloc;
    volatile Deallocator RFreePtr    = &free;

    #define malloc  RMallocPtr
    #define realloc RReallocPtr
    #define calloc  RCallocPtr
    #define free    RFreePtr
#endif // ----------------------------------------------------------------------