/**
 * RBuffer.c
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

#include <RBuffer.h>
#include <RClassTable.h>

#ifdef RAY_BUFFER_THREAD_SAFE
    #define bufferMutex &object->mutex
    #define RMutexLockBuffer() RMutexLock(bufferMutex)
    #define RMutexUnlockBuffer() RMutexUnlock(bufferMutex)
#else
    // sets empty
    #define bufferMutex
    #define RMutexLockBuffer(some)
    #define RMutexUnlockBuffer(some)
#endif

RBuffer* makeRBufferOptions(size_t startSize, size_t objectCount) {
    RBuffer *object = allocator(RBuffer);
    if(object != nil) {
        // allocation of buffer
        master(object, RByteArray) = makeRByteArray(startSize);
        if(master(object, RByteArray) != nil) {

            // allocation of sizes array
            object->sizesArray = arrayAllocator(RRange, objectCount);

            if(object->sizesArray  != nil) {
                object->classId     = registerClassOnce(toString(RBuffer));
                object->freePlaces  = objectCount;
                object->count       = 0;
                object->totalPlaced = 0;
#ifdef RAY_BUFFER_THREAD_SAFE
                mutexWithType(&object->mutex, RMutexRecursive);
#endif
            } else {
                RError("RBuffer. Allocation of sizes array failed.", object);
                deleter(master(object, RByteArray), RByteArray);
            }
        } else {
            RError("RBuffer. Allocation of master RByteArray failed.", object);
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

inline constructor(RBuffer)) {
    return makeRBufferOptions(startSizeOfRBufferDefault, sizeOfObjectsOfRBufferDefault);
}

destructor(RBuffer) {
    RMutexLockBuffer();
    // kills buffer
    deleter(master(object, RByteArray), RByteArray);
    // kills sizes
    deallocator(object->sizesArray);
    RMutexUnlockBuffer();
#ifdef RAY_BUFFER_THREAD_SAFE
    RMutexDestroy(bufferMutex);
#endif
}

printer(RBuffer) {
    size_t iterator;
    RMutexLockBuffer();
    RPrintf("%s object - %p {\n", toString(RBuffer), object);
    RPrintf("\t Total   size : %lu (bytes)\n", master(object, RByteArray)->size);
    RPrintf("\t Placed  size : %lu (bytes)\n", object->totalPlaced);
    RPrintf("\t Free  places : %lu\n", object->freePlaces);
    RPrintf("\t Count objcts : %lu\n\n", object->count);
    forAll(iterator, object->count) {
        RPrintf("\t\t %lu : size - %lu\n", iterator, object->sizesArray[iterator].size);
        printByteArrayInHex(master(object, RByteArray)->array + object->sizesArray[iterator].start, object->sizesArray[iterator].size);
        RPrintf("\n");
    }
    RPrintf("} %s object - %p\n", toString(RBuffer), object);
    RMutexUnlockBuffer();
}

#pragma mark Reallocation

method(RRange*, addSizeToSizes, RBuffer), size_t newSize) {
    RMutexLockBuffer();
    if(newSize > object->count) {
        object->sizesArray = RReAlloc(object->sizesArray, newSize * sizeof(RRange));
        if (object->sizesArray != nil) {
            // add free places
            object->freePlaces = newSize - object->count;
        }
    } elseError( RError("RBuffer. Bad new size for Sizes", object) );

    RMutexUnlockBuffer();
    return object->sizesArray;
}

method(RByteArray*, addSizeToMem, RBuffer), size_t newSize) {
    RMutexLockBuffer();
    if(newSize > (object->totalPlaced)) {
        master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, newSize);
        if (master(object, RByteArray)->array != nil) {
            // set newSize
            master(object, RByteArray)->size = newSize;
        }
    } elseError( RError("RBuffer. Bad new size for memory", object) );

    RMutexUnlockBuffer();
    return master(object, RByteArray);
}

method(void, flush, RBuffer)) {
    RMutexLockBuffer();
    object->freePlaces  += object->count;
    object->count        = 0;
    object->totalPlaced  = 0;
    RMutexUnlockBuffer();
}

method(RBuffer*, sizeToFit, RBuffer)) {
    RMutexLockBuffer();
    master(object, RByteArray)->array = RReAlloc(master(object, RByteArray)->array, object->totalPlaced);
    if(master(object, RByteArray)->array != nil) {
        master(object, RByteArray)->size = object->totalPlaced;
        object->sizesArray = RReAlloc(object->sizesArray, arraySize(RRange, object->count));
        if(object->sizesArray != nil) {
            object->freePlaces = 0;
        } elseError(
            RError("RBuffer. Bad sizes reallocation on sizeToFit.", object)
        );
    } else {
        RError("RBuffer. Bad reallocation on sizeToFit.", object);
        deallocator(object->sizesArray);
        deallocator(object);
        object = nil;
    }

    RMutexUnlockBuffer();
    return object;
}

#pragma mark Workers

constMethod(rbool, checkIndexWithError, RBuffer), size_t index) {
    RMutexLockBuffer();
    if(index < object->count) {
        RMutexUnlockBuffer();
        return yes;
    } else {
        RError("RBuffer. Bad index.", object);
        RMutexUnlockBuffer();
        return no;
    }
}

#pragma mark Data operations

method(void, addData, RBuffer), pointer data, size_t sizeInBytes) {
    RMutexLockBuffer();
    while(object->freePlaces == 0) {
        // add free to sizes
        $(object, m(addSizeToSizes, RBuffer)), object->count * sizeMultiplierOfRBufferDefault);
    }
    size_t counter = 1;
    while(sizeInBytes > master(object, RByteArray)->size - object->totalPlaced) {
        // add free to buffer
        $(object, m(addSizeToMem, RBuffer)), object->totalPlaced * sizeMultiplierOfRBufferDefault * counter);
        ++counter;
    }

    if(master(object, RByteArray)->array != nil
            && object->sizesArray != nil) {

        // add object
        RMemCpy(master(object, RByteArray)->array + object->totalPlaced, data, sizeInBytes);
        object->sizesArray[object->count].start = object->totalPlaced;
        object->sizesArray[object->count].size = sizeInBytes;

        object->totalPlaced += sizeInBytes;
        ++object->count;
        --object->freePlaces;
    }
    RMutexUnlockBuffer();
}

constMethod(pointer, getDataReference, RBuffer), size_t index) {
    RMutexLockBuffer();
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {
        RMutexUnlockBuffer();
        return (master(object, RByteArray)->array + object->sizesArray[index].start);
    } else {
        RMutexUnlockBuffer();
        return nil;
    }
}

constMethod(pointer, getDataCopy, RBuffer), size_t index) {
    byte *result = nil;
    RMutexLockBuffer();
    pointer ref = $(object, m(getDataReference, RBuffer)), index);
    if(ref != nil) {
        result = RAlloc(object->sizesArray[index].size);
        if (result != nil) {
            RMemCpy(result, ref, object->sizesArray[index].size);

        } elseError( RError("RBuffer. Bad allocation on getDataCopy.", object) );
    }
    RMutexUnlockBuffer();
    return result;
}

method(void, deleteDataAt, RBuffer), size_t index) {
    RMutexLockBuffer();
    if($(object, m(checkIndexWithError, RBuffer)), index) == yes) {

        RMemMove(master(object, RByteArray)->array + object->sizesArray[index].start,
                 master(object, RByteArray)->array + object->sizesArray[index].start + object->sizesArray[index].size,
                 object->totalPlaced - object->sizesArray[index].size);

        RMemMove(object->sizesArray + index,
                 object->sizesArray + index + 1,
                 object->count - index);

        --object->count;
        ++object->freePlaces;
        object->totalPlaced -= object->sizesArray[index].size;
    }
    RMutexUnlockBuffer();
}

#pragma mark Enumeration

constMethod(RFindResult, enumerate, RBuffer), REnumerateDelegate *delegate, rbool isFromLeft) {
    size_t iterator;
    RFindResult result;
    result.index  = object->count;
    result.object = nil;
    if(delegate->virtualEnumerator != nil) {
        RMutexLockBuffer();
        if(isFromLeft) {
            forAll(iterator, object->count) {
                if(delegate->virtualEnumerator(delegate->context, master(object, RByteArray)->array + object->sizesArray[iterator].start, iterator) == yes) {
                    break;
                }
            }
        } else {
            for(iterator = object->count - 1; iterator != 0; --iterator) {
                if(delegate->virtualEnumerator(delegate->context, master(object, RByteArray)->array + object->sizesArray[iterator].start, iterator) == yes) {
                    break;
                }
            }
        }
        if(iterator != object->count) {
            result.index = iterator;
            result.object = master(object, RByteArray)->array + object->sizesArray[iterator].start;
        }
        RMutexUnlockBuffer();
    } elseWarning(
            RWarning("RBuffer. enumerate. Delegate virtual function is nil.", object)
    );
    return result;
}

#pragma mark Casts

constMethod(RArray*, toReferencesRArray, RBuffer)) {
    size_t iterator;
    RArray *result = makeRArrayOptions(object->count, sizeMultiplierOfRArrayDefault, nil);

    if(result != nil) {
        RMutexLockBuffer();
        forAll(iterator, object->count) {
            $(result, m(addObjectUnsafe, RArray)),
                    $(object, m(getDataReference, RBuffer)), iterator));
        }
        RMutexUnlockBuffer();
    }
    return result;
}

constMethod(RArray *, toRArray, RBuffer)) {
    size_t iterator;
    RArray *result = makeRArrayOptions(object->count, sizeMultiplierOfRArrayDefault, nil);

    if(result != nil) {
        RMutexLockBuffer();
        forAll(iterator, object->count) {
            $(result, m(addObjectUnsafe, RArray)),
                    $(object, m(getDataCopy, RBuffer)), iterator));
        }
        RMutexUnlockBuffer();
        result->destructorDelegate = free;
    }
    return result;
}

#pragma mark File I/O

RBuffer* RBufferFromFile(const char *filename) {
    ssize_t  fileSize;
    byte    *buffer;
    FILE    *file   = RFOpen(filename, "rb");
    RBuffer *result = nil;

    if(file != nil) {
        RFSeek(file, 0, SEEK_END);
        fileSize = RFTell(file);
        if(fileSize > 0) {
            RRewind(file);
            buffer = arrayAllocator(byte, fileSize);
            if(buffer != nil) {
                // create variables
                uint64_t *sizesArray = nil;
                uint64_t  iterator   = 0;
                uint64_t  sumBytes  = 0;
                // read all
                RFRead(buffer, sizeof(byte), (size_t) fileSize, file);
                RFClose(file);

                // begin parse raw bytes
                if(buffer[0] == 4) {
                    //fixme custom 32-to-64
                    uint32_t *tempRef = (uint32_t *) (buffer + 1);
                    sizesArray = (uint64_t *) tempRef;

                } else if((buffer[0] == 8)
                        && (sizeof(size_t) == 8)) {
                    sizesArray = (uint64_t *) (buffer + 1);

                } elseError( RError2("RBufferFromFile. Bad size_t size - %u for current size_t - %lu", nil, buffer[0], sizeof(size_t)) );

                if(sizesArray != nil) {
                    // find terminating '\0' size
                    sumBytes += sizesArray[0];
                    while (sizesArray[iterator] != 0) {
                        ++iterator;
                        sumBytes += sizesArray[iterator];
                    }
                    RByteArray *array = allocator(RByteArray);
                    if (array != nil) {
                        array->size  = sumBytes;
                        array->array = buffer + 1 + (buffer[0] * (iterator + 1));

                        // processing
                        result = $(array, m(serializeToBuffer, RByteArray)), (size_t*)sizesArray);

                        // cleanup
                        deallocator(array);
                        deallocator(buffer);

                    }
                }
            } elseError( RError2("RBufferFromFile. Bad allocation buffer for file \'%s\' of size %lu.\n", nil, filename, fileSize) );

        } elseError( RError1("RBufferFromFile. Error read file \'%s\'.\n", nil, filename) );

    } elseError( RError1("RBufferFromFile. Cannot open file \'%s\'.\n", nil, filename) );

    return result;
}

constMethod(void, saveToFile, RBuffer), const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file != nil) {
        if (remove(filename) != 0) {
            RError("RBuffer. Can't delete existing file with buffer", object);
        }
    }

    file = fopen(filename, "wb+");
    if(file != nil) {
        size_t iterator;

        // dump fist byte sizeof(size_t)
        size_t result = sizeof(size_t);
        result = fwrite(&result, 1, 1, file);

        ifError(result != 1, RError("RBuffer. Failed save init data to file. Breaking processLine.", object) );

        // create sizes array
        size_t *tempSizes = arrayAllocator(size_t, object->count);

        if(tempSizes != nil) {
            // custom copy
            forAll(iterator, object->count) {
                tempSizes[iterator] = object->sizesArray[iterator].size;
            }

            // dump sizes array
            result = RFWrite(tempSizes, sizeof(size_t), object->count, file);

            ifError(result != object->count, RError("RBuffer. Failed save size array to file.", object) );

            // dump last size like 0
            result = 0;
            result = RFWrite(&result, sizeof(size_t), 1, file);

            ifError(result != 1, RError("RBuffer. Failed save last sizes \'\\0\' to file.", object) );

            // dump body
            result = RFWrite(master(object, RByteArray)->array, object->totalPlaced, 1, file);

            ifError(result != 1, RError("RBuffer. Failed save RBuffer body to file.", object) );

            // cleanup
            deallocator(tempSizes);
            fclose(file);

        } elseError(RError("RBuffer. Allocation of temp sizes array failed.", object) );

    } elseError( RError("RBuffer. Failed save string to file, cant open file.", object) );
}

#pragma mark Addition to RByteArray

constMethod(RBuffer *, serializeToBuffer, RByteArray), size_t *sizesArray) {
    size_t iterator = 0;

    // search end 0, compute length
    while(sizesArray[iterator] != 0) {
        ++iterator;
    }

    if(iterator != 0) {
        RBuffer *result = allocator(RBuffer);
        if(result != nil) {
            master(result, RByteArray) = $(object, m(copy, RByteArray)));
            if(master(result, RByteArray) != nil) {
                result->count = iterator;

                RRange *newSizesArray = arrayAllocator(RRange, result->count);
                if(newSizesArray != nil) {
                    size_t sum = 0;

                    // processLine size array into RRange array
                    forAll(iterator, result->count) {
                        newSizesArray[iterator].start = sum;
                        newSizesArray[iterator].size = sizesArray[iterator];
                        sum += newSizesArray[iterator].size;
                    }

                    // final operations
                    result->sizesArray  = newSizesArray;
                    result->totalPlaced = object->size;
                    result->freePlaces  = 0;
                    result->classId = registerClassOnce(toString(RBuffer));
                }
            }
        }
        return result;
    }
    return nil;
}

#pragma mark Additions to RArray

constMethod(RBuffer *, serializeToBuffer, RArray), size_t size) {
    size_t iterator;
    RBuffer *result = $(nil, c(RBuffer)));
    if(result != nil) {
        forAll(iterator, object->count) {
            $(result, m(addData, RBuffer)),
                    $(object, m(elementAtIndex, RArray)), iterator), size);
        }
        return $(result, m(sizeToFit, RBuffer)));
    }
    return nil;
}

constMethod(RBuffer *, serializeToBufferSizes, RArray), size_t *sizesArray) {
    size_t iterator = 0;
    RBuffer *result = $(nil, c(RBuffer)));
    if(result != nil) {
        // while size not 0
        for(;sizesArray[iterator] != 0; ++iterator) {
            $(result, m(addData, RBuffer)),
                    $(object, m(elementAtIndex, RArray)), iterator), sizesArray[iterator]);
        }
        return $(result, m(sizeToFit, RBuffer)));
    }
    return nil;
}

constMethod(RBuffer *, serializeToBufferDelegate, RArray), REnumerateDelegate *delegate) {
    size_t    iterator = 0,
           currentSize;
    pointer prtToSized;

    RBuffer *result = $(nil, c(RBuffer)));
    if(result != nil) {
        forAll(iterator, object->count) {
            if(delegate->virtualEnumerator(delegate, object->array[iterator], iterator) ) {
                currentSize = ((SerializerData *)delegate->context)->size;
                prtToSized = ((SerializerData *)delegate->context)->serializePtrStart;
                if(currentSize != 0 && prtToSized != nil) {
                    $(result, m(addData, RBuffer)), prtToSized, currentSize);
                }
            } else {
                break;
            }
        }
        return $(result, m(sizeToFit, RBuffer)));
    }
    return nil;
}