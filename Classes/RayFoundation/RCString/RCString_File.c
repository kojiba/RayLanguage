/**
 * RCString_File.c
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 8/10/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RCString_File.h"
#include "RayFoundation/RMemoryOperations/RByteOperations.h"

RCString* stringFromFile(const char *filename) {
    RByteArray *content = contentOfFile(filename);
    RCString   *result = RSC((const char *)content->array);
    deleter(content, RByteArray);
    ifError(result == nil,
            RError1("stringFromFile. Can't create string from \"%s\".", result, filename)
    );
    return result;
}

method(void, appendToFile, RCString), const char *filename) {
    FILE *file = fopen(filename, "ab");
    if (file != nil) {
    ssize_t result = fputs(object->baseString, file);

    ifError(result < 0,
    RError("RCString. Failed save string to file.", object)
    );

    fclose(file);

    } elseError(
            RError("RCString. appendToFile. Failed open file.", object)
    );
}


RCString * getInputString() {
    RCString *result   = makeRCString();
    char *charBuff = arrayAllocator(char, baseInputStringSize);
    size_t  empty    = baseInputStringSize;

    if(result != nil
       && charBuff != nil) {
        int symbol = ' ';

        while(symbol != '\n') {
#ifdef _WIN32
            symbol = RGetChar();
#else
            symbol = RGetCharUn();
#endif
            if (symbol < 256 && symbol != '\n') {
                charBuff[result->size] = (char) symbol;
                ++result->size;
                --empty;
                if(empty == 0) {
                    charBuff = RReAlloc(charBuff, arraySize(char, result->size * 3));
                    if(charBuff != nil) {
                        empty += (2 * result->size);
                    } elseError(
                            RError1("getInputString. Bad result string reallocation on new size %lu.", (pointer)charBuff, (result->size * 3))
                    );
                }
            }
        }

        if(empty > 0) {
            charBuff = RReAlloc(charBuff, arraySize(char, result->size + 1));
        }

        result->baseString = charBuff;
        charBuff[result->size] = 0;

    } elseError(
            RError("getInputString. Bad result string allocation.", nil)
    );

    return result;
}