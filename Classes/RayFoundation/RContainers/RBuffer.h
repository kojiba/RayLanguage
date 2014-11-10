/**
 * RArray.h
 * Realization of C dynamic memory buffer, in Ray additions.
 * May use like array of sized elements.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_BUFFER_H__
#define __R_BUFFER_H__

#include "../RSyntax.h"
#include "../RMemoryOperations/RByteOperations.h"

static const size_t startSizeOfRBufferDefault      = 1024; // 1kb of place
static const size_t sizeMultiplierOfRBufferDefault = 2;
static const size_t sizeOfObjectsOfRBufferDefault  = 128;  // 128 sizes stores

class(RBuffer)
    discipleOf(RByteArray)

    // sizes array
    size_t  freePlaces;
    size_t  count;
    size_t *sizesArray;

    // total
    size_t totalPlaced;
endOf(RBuffer)

constructor (RBuffer));
destructor  (RBuffer);
printer     (RBuffer);

// Reallocation
method(size_t*,     addSizeToSizes, RBuffer),    size_t newSize);          // adds some size to Sizes array, store data, returns self, newsize in sizeof(size_t)
method(RByteArray*, addSizeToMem,   RBuffer),    size_t newSize);          // adds some size to RByteArray, store data, returns self, newsize in bytes
method(RBuffer*,    flush,          RBuffer));                             // flushes buffer, returns self
method(RBuffer*,    sizeToFit,      RBuffer));                             // make without free places, store data, returns self

// Workers
method(size_t, shiftForPlace, RBuffer), size_t place);

// Data operations
method(void,    addData,       RBuffer), pointer *data, size_t sizeInBytes);  // copies data
method(pointer, getDataCopy,   RBuffer), size_t index);                       // return copy

#endif /*__R_BUFFER_H__*/