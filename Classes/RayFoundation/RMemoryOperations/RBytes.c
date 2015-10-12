/**
 * RBytes.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
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

#include "RBytes.h"

#pragma mark RBytes

RData* makeRData(byte *array, size_t size, byte type)  {
    RData *object = nil;
    if(array != nil) {
        object = allocator(RData);
        if(object != nil) {
            object->data = array;
            object->size = size;
            object->type = type;
        }
    }
    return object;
}

RBytes *makeRBytes(byte *array, size_t size) {
    RBytes *object = allocator(RBytes);
    if (object != nil) {
        object->data = array;
        object->size = size;
    }
    return object;
}

inline constructor(RBytes), size_t size) {
    byte *array = RAlloc(size);
    if (array != nil) {
        return makeRBytes(array, size);
    } elseError(
            RError("RBytes. Array allocation failed", object)
    );
    return nil;
}

destructor(RBytes) {
    deallocator(object->data);
}

printer(RBytes) {
    RPrintf("%s object - %p [%lu] {\n", toString(RBytes), object, object->size);
    printByteArrayInHex(object->data, object->size);
    RPrintf("} - %p \n\n", object);
}

method(RBytes *, flushAllToByte, RBytes), byte symbol) {
    object->data = flushAllToByte(object->data, object->size, symbol);
    return object;
}

constMethod(RBytes *, copy, RBytes)) {
    RBytes *copy = $(nil, c(RBytes)), object->size);
    if (copy != nil) {
        RMemCpy(copy->data, object->data, object->size);
    }
    return copy;
}

method(RBytes *, fromRCString, RBytes), RCString *string) {
    if (object->size <= string->size && object->size != 0) {
        RMemCpy(object->data, string->baseString, string->size);
        return object;
    }
    return nil;
}

method(RBytes *, insertInBeginBytes, RBytes), pointer data, size_t size) {
    object->data = RReAlloc(object->data, arraySize(byte, object->size + size));
    if (object->data != nil) {
        RMemCpy(object->data + size, object->data, object->size);
        RMemCpy(object->data, data, size);
    }
    return object;
}

method(RBytes *, insertInBegin, RBytes), RBytes *array) {
    object->data = RReAlloc(object->data, arraySize(byte, object->size + array->size));
    if (object->data != nil) {
        RMemCpy(object->data + array->size, object->data, object->size);
        RMemCpy(object->data, array->data, array->size);
    }
    return object;
}

RBytes *contentOfFile(const char *filename) {
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
                    return makeRBytes(buffer, (size_t) fileSize);
                } else {
                    RError2("contentOfFile. Can't read file \"%s\" of size \"%lu\".\n",
                            nil, filename, fileSize);
                    RFClose(file);
                }

            } elseError(
                    RError2("contentOfFile. Bad allocation buffer for file \"%s\" of size \"%lu\".\n",
                            nil, filename, fileSize
                    ));

        } elseError(
                RError1("contentOfFile. Error read (ftell) file \"%s\".\n", nil, filename)
        );

    } elseError(
            RError1("contentOfFile. Cannot open file \"%s\".\n", nil, filename)
    );

    return nil;
}
