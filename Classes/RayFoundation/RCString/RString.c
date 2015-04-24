#include <RString.h>

#define isTrail(c) (c > (byte)0x7F && c < (byte)0xC0)

size_t utf8Length(byte *string, size_t sizeInBytes) {
    rbool  isValid = 0;
    size_t length  = 0;
    size_t nextPosition = 0;
    unsigned codePoint = 0;

    while (utf8GetNextСharacter(string, sizeInBytes, &nextPosition, &isValid, &codePoint)) {
        ++length;
    }
    ifWarning(!isValid,
              RWarning("RString. Utf-8 encoding not valid.", (pointer) string)
    );

    return length;
}

void iterateCharacters(unsigned char *str, size_t str_size) {
    uintptr_t position = 0;
    uintptr_t nextPosition = 0;
    rbool     isValid = 0;
    unsigned  codePoint = 0;

    while (utf8GetNextСharacter(str, str_size, &nextPosition, &isValid, &codePoint) ) {
        if (isValid == yes) {
//            nextPosition - position
        } elseWarning(
            RWarning("iterateCharacters. Character is invalid.", str)
        );
        position = nextPosition;
    }
}

rbool utf8GetNextСharacter(const byte     *string,
                                 size_t    stringSize,
                                 size_t   *cursor,
                                 rbool    *isValid,
                                 unsigned *codePoint) {

    size_t position = *cursor;
    size_t rest_size = stringSize - position;
    byte character;
    byte min;
    byte max;

    *codePoint = 0;
    *isValid = yes;

    if (*cursor >= stringSize) {
        return no;
    }

    character = string[position];

    if (rest_size < 1) {
        *isValid = no;
        position += 1;
    } else if (character < (byte)0x80) {
        *codePoint = string[position];
        *isValid = yes;
        position += 1;
    } else if (character < (byte)0xC2) {
        *isValid = no;
        position += 1;
    } else if (character < (byte)0xE0) {

        if (rest_size < 2 || !isTrail(string[position + 1])) {
            *isValid = no;
            position += 1;
        } else {
            *codePoint = ((string[position] & (byte)0x1F) << 6) | (string[position + 1] & (byte)0x3F);
            *isValid = yes;
            position += 2;
        }

    } else if (character < (byte)0xF0) {

        min = (character == (byte)0xE0) ? (byte)0xA0 : (byte)0x80;
        max = (character == (byte)0xED) ? (byte)0x9F : (byte)0xBF;

        if (rest_size < 2 || string[position + 1] < min || max < string[position + 1]) {
            *isValid = no;
            position += 1;
        } else if (rest_size < 3 || !isTrail(string[position + 2])) {
            *isValid = no;
            position += 2;
        } else {
            *codePoint = ((string[position] & (byte)0x1F) << 12)
                         | ((string[position + 1] & (byte)0x3F) << 6)
                         | (string[position + 2] & (byte)0x3F);
            *isValid = yes;
            position += 3;
        }

    } else if (character < (byte)0xF5) {

        min = (character == (byte)0xF0) ? (byte)0x90 : (byte)0x80;
        max = (character == (byte)0xF4) ? (byte)0x8F : (byte)0xBF;

        if (rest_size < 2 || string[position + 1] < min || max < string[position + 1]) {
            *isValid = no;
            position += 1;
        } else if (rest_size < 3 || !isTrail(string[position + 2])) {
            *isValid = no;
            position += 2;
        } else if (rest_size < 4 || !isTrail(string[position + 3])) {
            *isValid = no;
            position += 3;
        } else {
            *codePoint = ((string[position] & (byte)0x7) << 18)
                         | ((string[position + 1] & (byte)0x3F) << 12)
                         | ((string[position + 2] & (byte)0x3F) << 6)
                         | (string[position + 3] & (byte)0x3F);
            *isValid = yes;
            position += 4;
        }

    } else {
        *isValid = no;
        position += 1;
    }

    *cursor = position;
    return yes;
}