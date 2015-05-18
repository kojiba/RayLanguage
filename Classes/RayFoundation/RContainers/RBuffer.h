/**
 * RBuffer.h
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

#include <RSyntax.h>
#include <RByteOperations.h>

#ifdef RAY_BUFFER_THREAD_SAFE
    #include <RThread.h>
#endif

#define startSizeOfRBufferDefault      1024 // 1kb of place
#define sizeMultiplierOfRBufferDefault 2
#define sizeOfObjectsOfRBufferDefault  128  // 128 sizes stores

class(RBuffer) // -------------------
    discipleOf(RByteArray)

    // sizes array info
    size_t  freePlaces;
    size_t  count;
    RRange *sizesArray;

    // total placed bytes size
    size_t totalPlaced;

#ifdef RAY_BUFFER_THREAD_SAFE
    RMutexDescriptor mutex;
#endif
endOf(RBuffer) // -------------------

extern inline
RBuffer* makeRBufferOptions (size_t startSize, size_t objectCount);

constructor (RBuffer));
destructor  (RBuffer);
printer     (RBuffer);

// Reallocation
method(RRange *,          addSizeToSizes, RBuffer),    size_t newSize);          // adds some size to Sizes array, store data, returns self, newsize in sizeof(size_t)
method(RByteArray *,      addSizeToMem,   RBuffer),    size_t newSize);          // adds some size to RByteArray, store data, returns self, newsize in bytes
method(void,              flush,          RBuffer));                             // flushes buffer, returns self
method(RBuffer *,         sizeToFit,      RBuffer));                             // make without free places, store data, returns self

// Data operations
// setters
method(void,              addData,            RBuffer),    pointer data, size_t sizeInBytes);   // copies data

// getters
constMethod(pointer,      getDataReference,   RBuffer),    size_t index);                       // return pointer
constMethod(pointer,      getDataCopy,        RBuffer),    size_t index);                       // return copy

// deleters
method(void,              deleteDataAt,       RBuffer),    size_t index);

// enumeration
constMethod(RFindResult,  enumerate,          RBuffer), REnumerateDelegate *delegate, rbool isFromLeft);

// casts
constMethod(RArray *,     toReferencesRArray, RBuffer)); // using data reference from RBuffer, delegates not set, size to fit
constMethod(RArray *,     toRArray,           RBuffer)); // copy, delete delegateFunction is free, size to fit

// file i/o
RBuffer* RBufferFromFile (const char *filename); // will be size-to-fit
constMethod(void,         saveToFile,         RBuffer),    const char* filename);


// Additions to RByteArray
constMethod(RBuffer *,    serializeToBuffer,  RByteArray),    size_t *sizesArray);        // return created RBuffer, sizesArray must ends on 0

// Additions to RArray
constMethod(RBuffer *,    serializeToBuffer,      RArray),    size_t size);              // if all object one sized
constMethod(RBuffer *,    serializeToBufferSizes, RArray),    size_t *sizesArray);       // return created RBuffer, sizesArray must ends on 0

#endif /*__R_BUFFER_H__*/