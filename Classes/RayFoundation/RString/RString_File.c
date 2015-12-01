/**
 * RString_File.c
 * File i/o for strings.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RString_File.h"

RString* stringFromFile(const char *filename) {
    RData *content = contentOfFile(filename);
    $(content, m(validateToASCII, RString)));
    return content;
}

method(void, appendToFile, RString), const char *filename) {
    FILE *file = fopen(filename, "ab");
    if (file != nil) {
    ssize_t result = fputs((const char *) object->data, file);

    ifError(result < 0,
    RError("RString. Failed save string to file.", object)
    );

    fclose(file);

    } elseError(
            RError("RString. appendToFile. Failed open file.", object)
    );
}


RString * getInputString() {
    RString *result   = allocator(RString);
    char    *charBuff = arrayAllocator(char, baseInputStringSize);
    size_t   empty    = baseInputStringSize;

    if(result != nil
       && charBuff != nil) {
        result->type = RDataTypeASCII;
        result->size = 0;
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

        if(empty > 0) { // size to fit
            charBuff = RReAlloc(charBuff, arraySize(char, result->size + 1));
            charBuff[result->size] = 0;
        }

        result->data = (byte *) charBuff;

    } elseError(
            RError("getInputString. Bad result string allocation.", nil)
    );

    return result;
}