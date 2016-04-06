/**
 * @file RBuffer.h
 * @brief Realization of continuous C dynamic
 *        memory buffer, in Ray additions.
 *        May be used like array of sized elements.
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 11/7/2014
 * @par Ukraine Kharkiv
 *
 *//*
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 */

#ifndef __R_BUFFER_H__
#define __R_BUFFER_H__

#include "RayFoundation/RSyntax.h"
#include "RayFoundation/RMemoryOperations/RData.h"

#ifdef RAY_BUFFER_THREAD_SAFE
    #include "RayFoundation/RThread/RThread.h"
#endif

#define startSizeOfRBufferDefault      1024 ///< start size of memory chunk created with default constructor
#define sizeMultiplierOfRBufferDefault 2    ///< static resize memory multiplier for memory and object count
#define sizeOfObjectsOfRBufferDefault  128  ///< start size of array, that stores sizes, created with default constructor


class(RBuffer) // -------------------
    discipleOf(RData) ///< continuous memory chunk, (size - how many memory was allocated)

    // sizes array info
    size_t  freePlaces; ///< count of free places in sizesArray
    size_t  count;      ///< count of filled places in sizesArray
    RRange *sizesArray; ///< ranges array, defines start and size of each element stored in buffer

    // total placed bytes size
    size_t totalPlaced; ///< count bytes, that totally placed, (size - how many memory currently placed)

#ifdef RAY_BUFFER_THREAD_SAFE
    RMutexDescriptor mutex; ///< single thread lock (mutex), recursive
#endif
endOf(RBuffer) // -------------------

#pragma mark Flexible constructor

/**
 * Main optioned constructor for RBuffer instances.
 *
 * @param      startSize     Start size of continuous memory chunk will be allocated
 * @param      objectCount   Start count of continuous object chunk will be stored (defines sizesArray allocated size)
 *
 * @return Created instance of RBuffer or nil if error occurs.
 */
RBuffer* makeRBufferOptions(size_t startSize, size_t objectCount);

/**
 * Default for RBuffer instances.
 * Use makeRBufferOptions with startSizeOfRBufferDefault and sizeOfObjectsOfRBufferDefault parameters
 *
 * @return Created instance of RBuffer or nil if error occurs.
 */
extern constructor (RBuffer));

/**
 * Default destructor for RBuffer instances.
 */
destructor  (RBuffer);

/**
 * Default printer for RBuffer instances.
 */
printer     (RBuffer);

#pragma mark Reallocation

/**
 * Method used for long-up, shortify-to-fit sizesArray
 * Store data.
 *
 * @param      newSize     New size for sizesArray.
 *                         If newSize > object->count - errors, and no resize.
 *
 * @return self sizesArray after realloc
 */
method(RRange *,          addSizeToSizes, RBuffer),    size_t newSize);    // adds some size to Sizes array, store data, returns self, newsize in sizeof(size_t)

/**
 * Method used for long-up, shortify-to-fit continuous memory array.
 *
 * @param      newSize     New size for memory array.
 *                         Size in bytes.
 *                         If newSize > object->totalPlaced - errors, and no resize.
 *
 * @return self continuous memory array after realloc
 */
method(RData *,           addSizeToMem,   RBuffer),    size_t newSize);    // adds some size to RData, store data, returns self, newsize in bytes

/**
 * Method used for erase data, but store struct.
 * Lazy flush, data not been set to 0, i.e. data can be overwrite.
 */
method(void,              flush,          RBuffer));

/**
 * Method used for delete unused free places and memory places.
 * I.e. will freePlaces == 0, data size == totalPlaced.
 *
 * @return self after memory and sizesArray reallocing
 */
method(RBuffer *,         sizeToFit,      RBuffer));                       // make without free places, store data, returns self

#pragma mark Data operations
#pragma mark setters

/**
 * Method for add some data to buffer. Copies it.
 * Self longs places and mem if needed
 * (i.e. if freePlaces == 0, or sizeInBytes > data size - totalPlaced).
 *
 * @param      data          Pointer to data start
 * @param      sizeInBytes   Data to copy size in bytes
 */
       method(void,       addBytes,           RBuffer),    const pointer data, size_t sizeInBytes);    // copies data

/**
 * Method for add some data to buffer.
 * Full wrapper on addBytes for RData structs.
 *
 * @param      data          RData to copy into buffer
 */
extern method(void,       addData,            RBuffer),    const RData *data);    // copies data

#pragma mark getters

/**
 * Returns data pure reference(pointer) at index.
 *
 * @param      index    Index of data to get reference
 *
 * @return data reference or nil if index < count, error it
 */
constMethod(pointer,      getDataReference,   RBuffer),    size_t index);    // return pointer

/**
 * Returns data copy.
 * Copy size specified in sizesArray.
 *
 * @param      index    Index of data to get copy
 *
 * @return allocated data copy or nil if index < count, erros it
 */
constMethod(pointer,      getDataCopy,        RBuffer),    size_t index);    // return copy

#pragma mark deleters

/**
 * Deletes data at specified index.
 * Shifts sizesArray and memory chunk.
 * Note: increments free places, decrement count and total placed size.
 *
 * @param      index    Index of data to delete
 */
method(void,              deleteDataAt,       RBuffer),    size_t index);

#pragma mark enumeration

/**
 * Default enumeration function.
 * See RArray.enumerate docs.
 */
constMethod(RFindResult,  enumerate,          RBuffer), REnumerateDelegate *delegate, rbool isFromLeft);

#pragma mark casts

/**
 * Creates RArray reference with pointers to data.
 * Note: use RArray.addObjectUnsafe
 *       and RBuffer.getDataReference,
 *       and makeRArrayOptions(object->count, sizeMultiplierOfRArrayDefault, nil)
 *       Result array will be size-to-fited.
 *
 * @return Created RArray (with nil delegates, array object count == buffers object count),
 *         or nil if array allocation fails
 */
constMethod(RArray *,     toReferencesRArray, RBuffer));    // using data reference from RBuffer, delegates not set, size to fit

/**
 * Creates RArray reference with pointers to data copied.
 * Note: use RArray.addObjectUnsafe
 *       and RBuffer.getDataCopy,
 *       and makeRArrayOptions(object->count, sizeMultiplierOfRArrayDefault, nil)
 *       Result array will be size-to-fited.
 *
 * @return Created RArray (destructorDelegate will be set to default deallocator-free)
 *         or nil if array allocation fails
 */
constMethod(RArray *,     toRArray,           RBuffer));    // copy, delete delegateFunction is free, size to fit

#pragma mark casts file i/o

/**
 * Load RBuffer from file,
 * which been saved using RBuffer.saveToFile function
 * File fill be open, and closed.
 * Load type is binary.
 * Different file types for x86 and x64 archs.
 *
 * Note: RError when something goes wrong.
 *       First byte of file defines sizeof(size_t),
 *       load of x64 file in x86 will be incorrect and errored
 *       Used unbufferized RData.contentOfFile function.
 *
 * @param    filename    C-type string ('\0'-terminated), defines filename to load
 *
 * @return   Created size-to-fit RBuffer instance or nil if error occurs.
 */
RBuffer* RBufferFromFile (const char *filename); // will be size-to-fit

/**
 * Store RBuffer to file.
 * File fill be open, and closed.
 * Save type is binary.
 * Different file types for x86 and x64 archs.
 *
 * Warning: Deletes file, if found it using RRemove.
 *
 * Note: RError when something goes wrong.
 *       First byte of file defines sizeof(size_t),
 *       load of x64 file in x86 will be incorrect and errored
 *       Used unbufferized RFWrite.
 *
 * @param    filename    C-type string ('\0'-terminated), defines filename to save
 */
constMethod(void,         saveToFile,         RBuffer),    const char* filename); // rewrite file


#pragma mark casts Additions to RData

/**
 * Serialize RData to RBuffer.
 *
 * @param    sizesArray    0-terminated sizes array
 *                         (i.e. {5, 6, 7, 9, 11, 20, 0}),
 *                         size in bytes
 *
 * @return   Created size-to-fit RBuffer instance or nil if error occurs.
 */
constMethod(RBuffer *,    serializeToBuffer,  RData),    size_t *sizesArray);         // return created RBuffer, sizesArray must ends on 0

#pragma mark casts Additions to RArray

/**
 * Serialize RArray to RBuffer.
 * Note: Using RBuffer.addBytes, RBuffer.sizeToFit, RArray.objectAtIndex
 *
 * @param    size    Constant size of all objects to copy from data
 *
 * @return   Created size-to-fit RBuffer instance or nil if error occurs.
 */
constMethod(RBuffer *,    serializeToBuffer,         RArray),    size_t size);        // if all object one sized

/**
 * Serialize RArray objects to RBuffer.
 * Note: Using RBuffer.addBytes, RBuffer.sizeToFit and RArray.objectAtIndex
 *
 * @param    sizesArray    0-terminated sizes array
 *                         (i.e. {5, 6, 7, 9, 11, 20, 0}),
 *                         size in bytes
 *
 * @return   Created size-to-fit RBuffer instance or nil if error occurs.
 */
constMethod(RBuffer *,    serializeToBufferSizes,    RArray),    size_t *sizesArray); // return created RBuffer, sizesArray must ends on 0

#endif /*__R_BUFFER_H__*/