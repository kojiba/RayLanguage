/**
 * RData.c
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 10/7/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RData.h"

#pragma mark RData

RData* makeRData(const byte *array, size_t size, byte type)  {
    RData *object = nil;
    if(array != nil) {
        object = allocator(RData);
        if(object != nil) {
            object->data = (byte *) array;
            object->size = size;
            object->type = type;
        }
    }
    return object;
}

destructor(RData) {
    deallocator(object->data);
}

constMethod(void, printInFile, RData), FILE *file, rbool appendNewline) {
    if(object != nil && object->data != nil) {
        if (object->type == RDataTypeBytes
            || object->type == RDataTypeOther) {
            RFPrintf(file, "RData object - %p [%lu] {\n", object, object->size);
            printByteArrayInHexWithScreenSizeFile(object->data, object->size, 32, file);
            RFPrintf(file, "} - %p \n\n", object);
        } else {
            // write string full
            fwrite(object->data, object->size, 1, file);
            if(object->type == RDataTypeASCII && appendNewline) {
                fwrite("\n", sizeof("\n") - 1, 1, file);
            }
        }
    } else {
        RFPrintf(file, "(nil)");
    }
}

inline printer(RData) {
    $(object, m(printInFile, RData)), stdout, yes);
}

inline void RDataDeleter(RData* object) {
    deleter(object, RData);
}

inline method(RData *, flushAllToByte, RData), byte symbol) {
    flushAllToByte(object->data, object->size, symbol);
    return object;
}

inline method(void, insert, RData), const RData *data, size_t place) {
    object->data = insertSubArray(object->data, &object->size, data->data, data->size, place);
}

inline constMethod(RData *, copy, RData)) {
    return makeRData(getByteArrayCopy(object->data, object->size), object->size, object->type);
}

inline constMethod(RData*, subArrayInRange, RData), RRange range) {
    return makeRData(subArrayInRange(object->data, object->size, range), object->size, object->type);
}

constMethod(RCompareFlags, compareWith, RData), const RData *checkData) {
    if (checkData == object) {
        return equals;
    } else {
        if(object->type != checkData->type) {
            RWarning1("RData. Compare different data types with %p.", object, checkData);
        }
        if (checkData->size == object->size) {
            if (compareMemory(object->data, checkData->data, object->size) == REquals) {
                return equals;
            } else {
                return not_equals;
            }
        } else {
            if (checkData->size > object->size) {
                return shorter;
            } else {
                return longer;
            }
        }
    }
}

constMethod(rbool, isEqualTo, RData), const RData *checkData) {
    return (rbool)($(object, m(compareWith, RData)), checkData) == equals);
}

constMethod(RArray*, dataSeparatedByBytes, RData), const RData *separatorsArray) {
     size_t iterator;
     size_t endOfSubstring   = 0;
     size_t startOfSubstring = 0;
      rbool isFirst          = yes;
    RArray *result           = nil;
     RData *subArray;

    forAll(iterator, object->size) {

        // check if separator
        if (isContainsByte(separatorsArray->data, separatorsArray->size, object->data[iterator])) {
            // if first separator set end of substring
            if (isFirst) {
                endOfSubstring = iterator;
                isFirst = no;
                if (result == nil) {
                    result = c(RArray)(nil, nil);

                    if (result != nil) {
                        // set-up delegates
                        result->printerDelegate = (PrinterDelegate) p(RData);
                        result->destructorDelegate = (DestructorDelegate) RDataDeleter;

                        // exit if allocation fails
                    } else {
                        RError("RData. dataSeparatedByBytes. Bad array for substrings allocation.", object);
                        return nil;
                    }
                }
            }

            // if not separator
        } else {

            // if we found some separators
            if (!isFirst) {
                isFirst = yes;
                RRange range = makeRRangeTo(startOfSubstring, endOfSubstring);
                subArray = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
                $(result, m(addObject, RArray)), subArray);
                startOfSubstring = iterator;
            }
        }
    }

    // if we found some
    if(result != nil) {
        // if last is not separator - add whole last word
        if(endOfSubstring < startOfSubstring) {
            endOfSubstring = object->size;
        }
        // add last
        RRange range = makeRRangeTo(startOfSubstring, endOfSubstring);
        subArray = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
        if(subArray != nil) {
            $(result, m(addObject, RArray)), subArray);
        }
        // and sizeToFit
        $(result, m(sizeToFit, RArray)));
    }
    return result;
}

constMethod(RArray*, dataSeparatedByArrayWithShield, RData), const RData *separator, const RData *shield) {
    RArray *result = nil;
    if(object->size >= separator->size) {
        size_t iterator;
        size_t startOfSubstring = 0;
        size_t inner;
        RData *substring = nil;

        forAll(iterator, object->size) {
            // check if separator
            if(object->data[iterator] == separator->data[0]) {
                // compare others
                for(inner = 1; inner < separator->size; ++inner) {
                    if(object->data[iterator + inner] != separator->data[inner]) {
                        break;
                    }
                }
                if(inner == separator->size) { // if separator than
                    rbool noShieldBehind = yes;

                    if (shield != nil
                        && iterator >= shield->size) {
                        // check if shield behind
                        for (inner = 0; inner < shield->size; ++inner) {
                            if (object->data[iterator - inner - 1] != shield->data[shield->size - inner - 1]) {
                                break;
                            }
                        }
                        noShieldBehind = (rbool) (inner != shield->size);
                    }

                    if(noShieldBehind) { // if there are no shield
                        RRange range = makeRRangeTo(startOfSubstring, iterator);
                        substring = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
                        if(result == nil) {
                            result = makeRArray();
                            if(result != nil) {
                                // set-up delegates
                                result->printerDelegate = (PrinterDelegate) p(RData);
                                result->destructorDelegate = (DestructorDelegate) RDataDeleter;

                                // exit if allocation fails
                            } else {
                                RError("RData. dataSeparatedByArrayWithShield. Bad array for substrings allocation.", object);
                                return nil;
                            }
                        }
                        $(result, m(addObject, RArray)), substring);
                        startOfSubstring = iterator + separator->size;
                    }
                }
                iterator += inner;
            }
        }

        if(result != nil) {
            // if last is not separator - add whole last word
            if(startOfSubstring < object->size && iterator == object->size) {
                // add last
                RRange range = makeRRangeTo(startOfSubstring, iterator);
                substring = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
                if(substring != nil) {
                    $(result, m(addObject, RArray)), substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RData. dataSeparatedByArrayWithShield. Bad separator size, or array size.", object)
    );

    return result;
}

constMethod(RArray*, dataSeparatedByArray, RData), const RData *separator) {
    return $(object, m(dataSeparatedByArrayWithShield, RData)), separator, nil);
}

rbool privateRDataSeparatorFirstSymbolChecker(byte *currentDataSymbol, RData *currentSeparator, size_t index) {
    if(currentSeparator->size > 0
          && *currentDataSymbol == currentSeparator->data[0]) {
        return no;
    }
    return yes;
}

constMethod(size_t, isRDataSeparatorFirstSymbol, RData), const RArray *separators, size_t index) { // index of separator or RNotFound
    REnumerateDelegate enumerator;
    enumerator.virtualEnumerator = (EnumeratorDelegate) privateRDataSeparatorFirstSymbolChecker;
    enumerator.context = (pointer) object->data + index; // pointer to byte at index

    RFindResult result = $(separators, m(enumerate, RArray)), &enumerator, yes);
    return result.index;
}

rbool privateRDataMinimalSizeEnumerator(size_t *currentMinimalSize, RData *currentData, size_t index) {
    if(currentData->size < *currentMinimalSize
            && currentData->size > 0) {
        *currentMinimalSize = currentData->size;
    }
    return yes;
}

constMethod(size_t, minimalRDataSize, RArray)) { // index of separator or RNotFound
    REnumerateDelegate enumerator;
    size_t minimalSize = RNotFound;
    enumerator.virtualEnumerator = (EnumeratorDelegate) privateRDataMinimalSizeEnumerator;
    enumerator.context = &minimalSize;
    $(object, m(enumerate, RArray)), &enumerator, yes);
    return minimalSize;
}

constMethod(RArray*, dataSeparatedBySeparatorsArray, RData), const RArray *separators) {
    return $(object, m(dataSeparatedBySeparatorsArrayWithShield, RData)), separators, nil);
}

constMethod(RArray*, dataSeparatedBySeparatorsArrayWithShield, RData), const RArray *separators, const RData *shield) {
    RArray *result = nil;
    size_t minimalSeparatorSize = $(separators, m(minimalRDataSize, RArray)));

    if(object->size >= minimalSeparatorSize) {
        size_t iterator;
        size_t startOfSubstring = 0;
        size_t inner;
        RData *substring = nil;

        forAll(iterator, object->size) {
            if(object->size - iterator >= minimalSeparatorSize) {
                // check if separator
                size_t separatorIndex = $(object, m(isRDataSeparatorFirstSymbol, RData)), separators, iterator);
                if(separatorIndex != RNotFound) {
                    RData *currentSeparator = $(separators, m(objectAtIndex, RArray)), separatorIndex);

                    // compare others
                    if(currentSeparator->size <= object->size - iterator) {
                        inner = compareMemory(object->data + iterator + 1, currentSeparator->data + 1, currentSeparator->size - 1);

                        if(inner == REquals) { // if separator than
                            rbool noShieldBehind = yes;

                            if (shield != nil
                                && iterator >= shield->size) {
                                // check if shield behind
                                noShieldBehind = (rbool) (compareMemory(object->data + iterator - shield->size, shield->data, shield->size) != REquals);
                            }

                            if(noShieldBehind) { // if there are no shield
                                RRange range = makeRRangeTo(startOfSubstring, iterator);
                                substring = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
                                if(result == nil) {
                                    result = makeRArray();
                                    if(result != nil) {
                                        // set-up delegates
                                        result->printerDelegate = (PrinterDelegate) p(RData);
                                        result->destructorDelegate = (DestructorDelegate) RDataDeleter;

                                        // exit if allocation fails
                                    } else {
                                        RError("RData. dataSeparatedByArrayWithShield. Bad array for substrings allocation.", object);
                                        return nil;
                                    }
                                }
                                $(result, m(addObject, RArray)), substring);
                                startOfSubstring = iterator + currentSeparator->size;
                            }
                            iterator += currentSeparator->size - 1;
                        } else {
                            iterator += inner;
                        }
                    }
                }
            }
        }

        if(result != nil) {
            // if last is not separator - add whole last word
            if(startOfSubstring < object->size && iterator == object->size) {
                // add last
                RRange range = makeRRangeTo(startOfSubstring, iterator);
                substring = makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
                if(substring != nil) {
                    $(result, m(addObject, RArray)), substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RData. dataSeparatedBySeparatorsArrayWithShield. Bad separator size, or array size.", object)
    );

    return result;
}

RArray* DataSeparatedByBytes(const byte *array, size_t size, const byte *separatorsArray, size_t separatorsSize) {
    RArray *result;
     RData *temp = makeRData(array, size, RDataTypeBytes);
     RData *tempSeparator = makeRData(separatorsArray, separatorsSize, RDataTypeBytes);
    result = $(temp, m(dataSeparatedByBytes, RData)), tempSeparator);
    deallocator(temp);
    deallocator(tempSeparator);
    return result;
}


RArray* DataSeparatedByArrayWithShield(const byte *array, size_t size, const byte *separatorArray, size_t separatorSize, const byte *shield, size_t shieldSize) {
    RArray *result;
     RData *temp = makeRData(array, size, RDataTypeBytes);
     RData *tempSeparator = makeRData(separatorArray, separatorSize, RDataTypeBytes);
     RData *tempShield = nil;
    if(shield != nil) {
        tempShield = makeRData(shield, shieldSize, RDataTypeBytes);
    }
    result = $(temp, m(dataSeparatedByArrayWithShield, RData)), tempSeparator, tempShield);
    deallocator(temp);
    deallocator(tempSeparator);
    deallocator(tempShield);
    return result;
}

inline RArray* DataSeparatedByArray(const byte *array, size_t size, const byte *separatorArray, size_t separatorSize) {
    return DataSeparatedByArrayWithShield(array, size, separatorArray, separatorSize, nil , 0);
}

RData *contentOfFile(const char *filename) {
    FILE *file = RFOpen(filename, "rb");
    byte *buffer;
    ssize_t fileSize;

    if (file != nil) {
        RFSeek(file, 0, SEEK_END);
        fileSize = RFTell(file);
        if (fileSize > 0) {
            RRewind(file);
            buffer = arrayAllocator(byte, (fileSize + 1));
            if (buffer != nil) {
                if (RFRead(buffer, sizeof(byte), (size_t) fileSize, file) > 0) {
                    RFClose(file);
                    return makeRDataBytes(buffer, (size_t) fileSize);
                } else {
                    RError2("RData. contentOfFile. Can't read file \"%s\" of size \"%lu\".\n",
                            nil, filename, fileSize);
                    RFClose(file);
                }

            } elseError(
                    RError2("RData. contentOfFile. Bad allocation buffer for file \"%s\" of size \"%lu\".\n",
                            nil, filename, fileSize
                    ));

        } elseError(
                RError1("RData. contentOfFile. Error read (ftell) file \"%s\".\n", nil, filename)
        );

    } elseError(
            RError1("RData. contentOfFile. Cannot open file \"%s\".\n", nil, filename)
    );

    return nil;
}

constMethod(rbool, saveToFile, RData), const char* filename) {
    rbool result = no;
    FILE *file = fopen(filename, "r");
    if (file != nil) {
        if (remove(filename) != 0) {
            RError("RData. Can't delete existing file with buffer", object);
        }
    }

    file = fopen(filename, "wb+");
    if(file != nil) {
        size_t writed = RFWrite(object->data, 1, object->size, file);
        if(writed != object->size) {
            RError2("RData. Failed save string to file, write only part %lu of %lu.", object, writed, object->size);
        } else {
            result = yes;
        }
    } elseError( RError("RData. Failed save string to file, cant open file.", object) );
    return result;
}
