/**
 * RSandBox.h
 * Defines continue heap block,
 * with random pointer allocation, deallocation,
 * encryption and management table, etc.
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
    RSandBoxAllocationModeStandart,
    RSandBoxAllocationModeRandom,    // use many memory for that mode, algo for alloc isn't optimised
    RSandBoxAllocationModeDelegated
} RSandBoxAllocationMode;

// free, that is do nothing
void emptyFree(pointer ptr);

typedef struct RControlDescriptor {
    size_t identifier;
    RRange memRange;
} RControlDescriptor;

class(RSandBox)
    RControlDescriptor     *descriptorTable;
    RRange                  descriptorsInfo; // count - count of places, from - placed
    RByteArray             *memPart;
    pointer               (*innerMallocPtr)(size_t size);
    void                  (*innerFreePtr)  (pointer ptr);
    size_t                (*rangeGenerator)(struct RSandBox *currentSandBox);
    RSandBoxAllocationMode  allocationMode; // by default is RSandBoxAllocationModeRandom
endOf(RSandBox)

constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount, pointer (*innerMallocPtr)(size_t size), void (*innerFreePtr)(pointer ptr));
destructor  (RSandBox);
printer     (RSandBox);
singleton   (RSandBox);

// Workings
method(rbool,   isRangeFree,    RSandBox), RRange range);
method(size_t,  sizeForPointer, RSandBox), pointer ptr);
method(size_t,  memoryPlaced,   RSandBox));

// Main methods
method(void,    addFilledRange, RSandBox), RRange range);
method(pointer, malloc,         RSandBox), size_t sizeInBytes);

// Simple crypt
method(void,    XorCrypt,         RSandBox),    RByteArray *key);
method(void,    XorDecrypt,       RSandBox),    RByteArray *key);

// change to sandbox function and free to nothing-does function
#define enableSandBoxMalloc(sandBoxFunction)  RMallocPtr = sandBoxFunction; RFreePtr = emptyFree
#define enableSandBoxFree  (sandBoxFree)      RFreePtr   = sandBoxFree;

// back to constant standart
#define disableSandBoxMalloc()                RMallocPtr = RTrueMalloc; RFreePtr = RTrueFree; RReallocPtr = RTrueRealloc

#endif /*__R_SAND_BOX_H__*/