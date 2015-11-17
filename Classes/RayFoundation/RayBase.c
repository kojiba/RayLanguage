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

    void printRPoolAtExitInjection() {
        p(RAutoPool)(RPool);
        deleter(RPool, RAutoPool);
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
        onceStoreFlag = 0;
        enablePool(RPool);
    }

    void* malloc(size_t size) {
        RPrintf("Malloc %lu\n", size);
        if(onceStoreFlag) {
            initLibPtrs();
        }
        return RMallocPtr(size);
    }

    void* calloc(size_t size1, size_t size2) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
        return RCallocPtr(size1, size2);
    }

    void* realloc(void* ptr, size_t size) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
        return RReallocPtr(ptr, size);
    }

    void  free(void* ptr) {
        if(onceStoreFlag) {
            initLibPtrs();
        }
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