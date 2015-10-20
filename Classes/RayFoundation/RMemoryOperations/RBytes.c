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

destructor(RData) {
    deallocator(object->data);
}

printer(RData) {
    if(object != nil && object->data != nil) {
        if (object->type == RDataTypeBytes
            || object->type == RDataTypeOther) {
            RPrintf("RData object - %p [%lu] {\n", object, object->size);
            printByteArrayInHex(object->data, object->size);
            RPrintf("} - %p \n\n", object);
        } else {
            // write string full
            fwrite(object->data, object->size, 1, stdout);
        }
    } else {
        RPrintf("nil");
    }
}

void RDataDeleter(RData* object) {
    deleter(object, RData);
}

method(RData *, flushAllToByte, RData), byte symbol) {
    object->data = flushAllToByte(object->data, object->size, symbol);
    return object;
}

constMethod(RData *, copy, RData)) {
    return makeRData(getByteArrayCopy(object->data, object->size), object->size, object->type);
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
