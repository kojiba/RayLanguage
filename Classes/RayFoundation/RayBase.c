#include "RayBase.h"

void* mallocFunc(size_t size) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("Ray malloc hook, size - %lu\n", size);
#endif
    return RMallocPtr(size);
}

void* reallocFunc(void *ptr, size_t size) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("Ray realloc hook, ptr - %p, size - %lu\n", ptr, size);
#endif
    return RReallocPtr(ptr, size);
}

void freeFunc(void *pointer) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("Ray free hook, ptr - %p\n", pointer);
#endif
    return RFreePtr(pointer);
}

void initPointers() {
#if RAY_SHORT_DEBUG == 1
    RPrintf("Init Ray Pointers\n");
#endif
    RMallocPtr  = RTrueMalloc;
    RFreePtr    = RTrueFree;
    RReallocPtr = RTrueRealloc;
}
