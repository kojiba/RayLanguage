/**
 * RayBase.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
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
 
#include <RayBase.h>
#include <RThread.h>

#undef malloc
#undef realloc
#undef calloc
#undef free

RMutex mallocMutex  = RStackRecursiveMutexInitializer;
RMutex callocMutex  = RStackRecursiveMutexInitializer;
RMutex reallocMutex = RStackRecursiveMutexInitializer;
RMutex freeMutex    = RStackRecursiveMutexInitializer;

const Allocator   RTrueMalloc  = &malloc;
const Reallocator RTrueRealloc = &realloc;
const Callocator  RTrueCalloc  = &calloc;
const Deallocator RTrueFree    = &free;

volatile Allocator   RMallocPtr  = &malloc;
volatile Callocator  RCallocPtr  = &calloc;
volatile Reallocator RReallocPtr = &realloc;
volatile Deallocator RFreePtr    = &free;

#ifdef RAY_GLOBAL_ALLOCATOR_SETTERS

void* RMalloc(size_t size) {
    return RMallocPtr(size);
}

void* RCalloc(size_t size, size_t blockSize) {
    return RCallocPtr(size, blockSize);
}

void* RRealloc(void*  ptr,  size_t size) {
    return RReallocPtr(ptr, size);
}

void RFree(void*  ptr) {
    return RFreePtr(ptr);
}

#pragma mark Setters

void setRMalloc(void * (*new)(size_t)) {
    RMutexLock(&mallocMutex);
    RMallocPtr = new;
    RMutexUnlock(&mallocMutex);
}

void setRCalloc(void * (*new)(size_t, size_t)) {
    RMutexLock(&callocMutex);
    RCallocPtr = new;
    RMutexUnlock(&callocMutex);
}

void setRRealloc(void * (*new)(void*, size_t)) {
    RMutexLock(&reallocMutex);
    RReallocPtr = new;
    RMutexUnlock(&reallocMutex);
}

void setRFree(void (*new)(void*)) {
    RMutexLock(&freeMutex);
    RFreePtr = new;
    RMutexUnlock(&freeMutex);
}

Allocator  getRMalloc (void) {
    Allocator new;
    RMutexLock(&mallocMutex);
    new = RMallocPtr;
    RMutexUnlock(&mallocMutex);
    return new;
}

Callocator getRCalloc (void) {
    Callocator new;
    RMutexLock(&callocMutex);
    new = RCallocPtr;
    RMutexUnlock(&callocMutex);
    return new;
}

Reallocator getRRealloc(void) {
    Reallocator new;
    RMutexLock(&reallocMutex);
    new = RReallocPtr;
    RMutexUnlock(&reallocMutex);
    return new;
}

Deallocator getRFree   (void) {
    Deallocator new;
    RMutexLock(&freeMutex);
    new = RFreePtr;
    RMutexUnlock(&freeMutex);
    return new;
}

    #define malloc  RMalloc
    #define realloc RRealloc
    #define calloc  RCalloc
    #define free    RFree

#else

    #define malloc  RMallocPtr
    #define realloc RReallocPtr
    #define calloc  RCallocPtr
    #define free    RFreePtr

#endif