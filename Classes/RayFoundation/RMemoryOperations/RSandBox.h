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

static pointer (*const trueMalloc)(size_t size) = malloc;

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

static pointer (*mallocPtr)(size_t size) = malloc;

// malloc entry point is pointer
#define malloc(sizeInBytes)           mallocPtr(sizeInBytes)

// change to sandbox function
#define enableSandBoxMalloc(sandBoxFunction)  mallocPtr = sandBoxFunction;

// back to constant standart
#define disableSandBoxMalloc()                mallocPtr = trueMalloc;

#endif /*__R_SAND_BOX_H__*/