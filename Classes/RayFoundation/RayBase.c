#include "RayBase.h"

void* mallocFunc(size_t size) {
#if RAY_SHORT_DEBUG == 1
    printf("Ray malloc hook\n");
#endif
    return RMallocPtr(size);
}

void freeFunc(void *pointer) {
#if RAY_SHORT_DEBUG == 1
    printf("Ray free hook\n");
#endif
    return RFreePtr(pointer);
}

void initPointers() {
#if RAY_SHORT_DEBUG == 1
    printf("Init Ray Pointers\n");
#endif
    RMallocPtr = RTrueMalloc;
    RFreePtr   = RTrueFree;
}
