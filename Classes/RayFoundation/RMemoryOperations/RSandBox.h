/**
 * RSandBox.h
 * Defines continue heap block,
 * with random pointer allocation, deallocation,
 * encryption and management table, etc.
 * May be used like auto-release pool in obj-c.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 30.10.14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/


#ifndef __R_SAND_BOX_H__
#define __R_SAND_BOX_H__

#include "RBytes.h"

#ifdef RAY_SAND_BOX_THREAD_SAFE
    #include "RayFoundation/RThread/RThread.h"
#endif

typedef enum RSandBoxAllocationMode {
    RSandBoxAllocationModeStandart = 0,
    RSandBoxAllocationModeRandom,    // ASLR, use many memory for that mode, algorithm for allocation isn't optimised (use 1,7-2x mem from real mem)
    RSandBoxAllocationModeDelegated
} RSandBoxAllocationMode;

typedef struct RControlDescriptor {
    RRange memRange;
} RControlDescriptor;

//---------------------------------------------------------------------------------

struct RSandBox;

class(RSandBoxDelegate)
    size_t (*rangeGenerator)(struct RSandBox *currentSandbox);
    size_t (*memoryPlaced)  (struct RSandBox *currentSandbox);
    rbool  (*isRangeFree)   (struct RSandBox *currentSandbox);
endOf(RSandBoxDelegate)

class(RSandBox)
    RBytes *memPart;

    RControlDescriptor *descriptorTable;
                size_t  descriptorsCount;
                size_t  descriptorsTotal;

    // inner (high-lvl in hierarchy functions)
    Allocator   innerMalloc;
    Reallocator innerRealloc;
    Callocator  innerCalloc;
    Deallocator innerFree;

    // self functions
    Allocator   selfMalloc;
    Reallocator selfRealloc;
    Callocator  selfCalloc;
    Deallocator selfFree;

    RSandBoxDelegate *delegate;

    RSandBoxAllocationMode  allocationMode; // by default is RSandBoxAllocationModeRandom

#ifdef RAY_SAND_BOX_THREAD_SAFE
    RMutex mutex;
#endif
endOf(RSandBox)

constructor (RSandBox),    size_t sizeOfMemory,
                           size_t descriptorsCount);
destructor  (RSandBox);
printer     (RSandBox);

// Main methods
method(pointer, malloc,         RSandBox),    size_t sizeInBytes);
method(pointer, realloc,        RSandBox),    pointer ptr, size_t newSize);
method(pointer, calloc,         RSandBox),    size_t blockCount, size_t blockSize);
method(void,    free,           RSandBox),    pointer ptr);

// Simple crypt
method(void,    XorCrypt,       RSandBox),    RBytes *key);
method(void,    XorDecrypt,     RSandBox),    RBytes *key);

// change malloc, realloc, calloc, free pointers to sandBox
void enableSandBox(RSandBox *sandBox);
void disableSandBox(RSandBox *sandBox);

//---------------------------------------------------------------------------------

#define sandBoxNamed(name, memSize) \
RSandBox* name();\
pointer concatenate(SandBoxAllocator_, name)(size_t size) {\
    return $(name(), m(malloc, RSandBox)), size);\
}\
pointer concatenate(SandBoxReallocator_, name)(pointer ptr, size_t size) {\
    return $(name(), m(realloc, RSandBox)), ptr, size);\
}\
pointer concatenate(SandBoxCallocator_, name)(size_t size, size_t blockSize) {\
    return $(name(), m(calloc, RSandBox)), size, blockSize);\
}\
void concatenate(SandBoxDeallocator_, name)(pointer ptr) {\
    return $(name(), m(free, RSandBox)), ptr);\
}\
RSandBox* name() { \
    static RSandBox *instance = nil; \
    if(instance == nil) { \
        instance = $(nil, c(RSandBox)), memSize, 32); \
        if(instance != nil) { \
            instance->selfMalloc  = concatenate(SandBoxAllocator_, name); \
            instance->selfRealloc = concatenate(SandBoxReallocator_, name); \
            instance->selfCalloc  = concatenate(SandBoxCallocator_, name); \
            instance->selfFree    = concatenate(SandBoxDeallocator_, name); \
            enableSandBox(instance); \
        } \
    } \
    return instance; \
}

// sandBoxNamed ---------------------------------------------------------


#endif /*__R_SAND_BOX_H__*/