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

// constant pointers to stdlib (OS) functions
static pointer (*const trueMalloc)(size_t size) = malloc;
static void    (*const trueFree)  (pointer ptr) = free;

// free, that is do nothing
void emptyFree(pointer ptr);

typedef struct RControlDescriptor {
    size_t identifier;
    RRange memRange;
} RControlDescriptor;


class(RSandBox)
    RControlDescriptor *descriptorTable;
    RRange              descriptorsInfo; // count - count of free, from - placed
    RByteArray         *memPart;
endOf(RSandBox)

constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount);
destructor  (RSandBox);
printer     (RSandBox);
singleton   (RSandBox);

method(rbool,   isRangeFree,    RSandBox), RRange range);
method(void,    addFilledRange, RSandBox), RRange range);
method(pointer, malloc,         RSandBox), size_t sizeInBytes);

// pointers to our functions
static pointer (*mallocPtr)(size_t size) = malloc;
static void    (*freePtr)  (pointer ptr) = free;

// malloc entry point is pointer
#define malloc(sizeInBytes) mallocPtr(sizeInBytes)
#define free(ptr)           freePtr(ptr)

// change to sandbox function and free to nothing-does function
#define enableSandBoxMalloc(sandBoxFunction)  mallocPtr = sandBoxFunction; freePtr = emptyFree
#define enableSandBoxFree  (sandBoxFree)      freePtr = sandBoxFree;

// back to constant standart
#define disableSandBoxMalloc()                mallocPtr = trueMalloc; freePtr = trueFree

#endif /*__R_SAND_BOX_H__*/