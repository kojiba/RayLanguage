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

#include "../RBasics/RBasics.h"
#include "RByteOperations.h"

#ifndef __R_SAND_BOX_H__
#define __R_SAND_BOX_H__

typedef struct RControlDescriptor {
    size_t identifier;
    RRange memRange;
} RControlDescriptor;

class(RSandBox)
    RControlDescriptor *descriptorTable;
    RByteArray         *memPart;
endOf(RSandBox)

constructor (RSandBox), size_t sizeOfMemory, size_t descriptorsCount);
destructor  (RSandBox);
printer     (RSandBox);

#endif /*__R_SAND_BOX_H__*/