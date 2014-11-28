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

#include "../RBasics/RBasics.h"
#include "RByteOperations.h"

typedef enum RSandBoxAllocationMode {
    RSandBoxAllocationModeStandart = 0,
    RSandBoxAllocationModeRandom,    // use many memory for that mode, algorithm for allocation isn't optimised
    RSandBoxAllocationModeDelegated
} RSandBoxAllocationMode;

typedef struct RControlDescriptor {
//    size_t identifier;  //  reserved
    RRange memRange;
} RControlDescriptor;

class(RSandBoxDelegate)
    size_t (*rangeGenerator)(struct RSandBox *currentSandbox);
    size_t (*memoryPlaced)  (struct RSandBox *currentSandbox);
    rbool  (*isRangeFree)   (struct RSandBox *currentSandbox);
endOf(RSandBoxDelegate)

class(RSandBox)
    RByteArray             *memPart;

    RControlDescriptor     *descriptorTable;
    RRange                  descriptorsInfo; // size - size of places, start - placed

    // inner (high-lvl in hierarchy functions)
    pointer               (*innerMalloc) (size_t size);
    pointer               (*innerRealloc)(pointer ptr, size_t oldSize);
    pointer               (*innerCalloc) (size_t size, size_t blockSize);
    void                  (*innerFree)   (pointer ptr);

    // self functions
    pointer               (*selfMalloc) (size_t size);
    pointer               (*selfRealloc)(pointer ptr, size_t oldSize);
    pointer               (*selfCalloc) (size_t size, size_t blockSize);
    void                  (*selfFree)   (pointer ptr);

    RSandBoxDelegate *delegate;

    RSandBoxAllocationMode  allocationMode; // by default is RSandBoxAllocationModeRandom
endOf(RSandBox)

constructor (RSandBox),    size_t sizeOfMemory,
                           size_t descriptorsCount);
destructor  (RSandBox);
printer     (RSandBox);
singleton   (RSandBox);

// Workings
method(size_t,  memoryPlaced,   RSandBox));

// Main methods
method(pointer, malloc,         RSandBox),    size_t sizeInBytes);
method(pointer, realloc,        RSandBox),    pointer ptr, size_t newSize);
method(pointer, calloc,         RSandBox),    size_t sizeInBytes, size_t blockSize);
method(void,    free,           RSandBox),    pointer ptr);

// Simple crypt
method(void,    XorCrypt,       RSandBox),    RByteArray *key);
method(void,    XorDecrypt,     RSandBox),    RByteArray *key);

// change malloc, realloc, calloc, free pointers to sandBox
void switchToSandBox(RSandBox *sandBox);
void switchFromSandBox(RSandBox *sandBox);

//------------------------------------------------------------------------------------------------

#define createSandBoxSingleton(name) \
RSandBox* name();\
pointer concatenate(SandBoxAllocator, name)(size_t size) {\
    return $(name(), m(malloc, RSandBox)), size);\
}\
pointer concatenate(SandBoxReallocator, name)(pointer ptr, size_t size) {\
    return $(name(), m(realloc, RSandBox)), ptr, size);\
}\
pointer concatenate(SandBoxCallocator, name)(size_t size, size_t blockSize) {\
    return $(name(), m(calloc, RSandBox)), size, blockSize);\
}\
void concatenate(SandBoxFree, name)(pointer ptr) {\
    return $(name(), m(free, RSandBox)), ptr);\
}\
RSandBox* name() { \
static RSandBox *instance = nil; \
if(instance == nil) { \
instance = $(nil, c(RSandBox)), 16348, 128); \
instance->selfMalloc = concatenate(SandBoxAllocator, name); \
instance->selfRealloc = concatenate(SandBoxReallocator, name); \
instance->selfCalloc = concatenate(SandBoxCallocator, name); \
instance->selfFree = concatenate(SandBoxFree, name); \
} \
return instance; \
}
/*createSandBoxSingleton*/


#endif /*__R_SAND_BOX_H__*/