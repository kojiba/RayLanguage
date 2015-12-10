/**
 * RString.h
 * Realization of wrapper on '\0' - terminated C-strings, in Ray additions.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RString_UTF8.h"
#include <stdarg.h>

#pragma mark Basics

char randomCharacter(void) {
    return (char) (rand() % 94 + 32);
}

RString * randomASCIIString(void) {
    size_t   iterator;
    size_t   size   = ((size_t)rand()) % 40 + 10;
    RString *string = makeRData(arrayAllocator(byte, size), size, RDataTypeASCII);

    if(string!= nil) {
        forAll(iterator, size) {
            string->data[iterator] = (byte) randomCharacter();
        }
    }

    return string;
}

char * cstringWithFormat(const char *format, ...) {
    va_list args;
    char *buffer;
    va_start(args, format);
    buffer = vcstringWithFormat(format, args);
    va_end(args);
    return buffer;
}

char * vcstringWithFormat(const char *format, va_list list) {
    int size;
    char *buffer = arrayAllocator(char, 100);
    va_list listCopy;
    va_copy(listCopy, list);
    if(buffer != nil) {
        size = RVSNPrintf(buffer, 100, format, list);
        if(size > 100) {
            ++size;
            buffer = RReAlloc(buffer, arraySize(char, (size_t) size));
            RVSNPrintf(buffer, (size_t) size, format, listCopy);
        } else if(size > 0) {
            buffer = RReAlloc(buffer, arraySize(char, (size_t) size));

        } elseError(
                RError("vcstringWithFormat. Bad size", nil)
        );
    }
    return buffer;
}

RString * stringWithFormat(char *string, ...) {
    va_list args;
    RString *result = nil;
    char *buffer;
    va_start(args, string);
    buffer = vcstringWithFormat(string, args);
    if(buffer != nil) {
        result = RCS(buffer);
    }
    va_end(args);
    return result;
}

inline destructor(RString) {
    d(RData)(object);
}

inline printer(RString) {
    p(RData)(object);
}

method(RString*, setConstantString, RString), const char *nullTerminatedString) {
    if(object != nil) {
        object->data = (byte *) nullTerminatedString;
        object->size = RStringLength(nullTerminatedString);
        object->type = RDataTypeASCII;
    }
    return object;
}

method(void, validateToASCII, RString)) {
    size_t iterator;
    forAll(iterator, object->size) {
        if((object->data[iterator] < 32 || object->data[iterator] > 126)
                && object->data[iterator] != 10) {
            deleteAtIndex(object->data, &object->size, iterator);
            --iterator;
        }
    }
    object->type = RDataTypeASCII;
}

#pragma mark Replace

method(void, replaceCharacters, RString), char characterToReplace, char replacer) {
    if(object->type == RDataTypeASCII) {
        replaceByteWithByte(object->data, object->size, (byte) characterToReplace, (byte) replacer);
    } elseWarning(
        RWarning("RString. Replace ascii characters in not ascii string", object)
    );
}

inline method(void, replaceSubstrings, RString), const RString *toReplace, const RString *replacer) {
    replaceBytesWithBytes(object->data, &object->size, toReplace->data, toReplace->size, replacer->data, replacer->size);
}

#pragma mark Info

inline constMethod(size_t, numberOfCharacters, RString), char character) {
    return numberOfBytes(object->data, object->size, (byte) character);
}

inline constMethod(size_t, numberOfSubstrings, RString), const RString * const string) {
    return numberOfSubArrays(object->data, object->size, string->data, string->size);
}

inline constMethod(rbool, isContains, RString), char character) {
   return indexOfFirstByte(object->data, object->size, (byte) character) == RNotFound ? no : yes;
}

inline constMethod(rbool, isContainsSubstring, RString), const RString *string) {
    return indexOfFirstSubArray(object->data, object->size, string->data, string->size) == RNotFound ? no : yes;
}

inline constMethod(size_t, numberOfLines, RString)) {
    return numberOfBytes(object->data, object->size, '\n');
}

inline constMethod(size_t, indexOfSubstring, RString), const RString *string) {
    return indexOfFirstSubArray(object->data, object->size, string->data, string->size);
}

#pragma mark Deletions

inline method(RString *, deleteAllCharacters, RString), char character) {
    object->data = deleteAllBytes(object->data, &object->size, (byte) character);
    return object;
}

inline method(void, removeRepetitionsOf, RString), char character) {
    object->data = deleteRepetitionsOfByte(object->data, &object->size, (byte) character);
}

inline method(RString *, deleteAllSubstrings, RString), const RString *substring) {
    object->data = deleteAllSubArrays(object->data, &object->size, substring->data, substring->size);
    return object;
}

inline method(void, removeRepetitionsOfString, RString), const RString *substring) {
    object->data = deleteRepetitionsOfSubArray(object->data, &object->size, substring->data, substring->size);
}

method(RString *, deleteCharacterAt, RString), size_t index) {
    if(index < object->size) {
        RMemMove(object->data + index, object->data + index + 1, object->size - index);
        --object->size;
    } elseError(
            RError("RString. Bad index!", object)
    );

    return object;
}

inline method(void, deleteInRange, RString), RRange range) {
    object->data = deleteInRange(object->data, &object->size, range);
}

inline method(void, trimTail, RString), size_t size) {
    object->data = trimTail(object->data, &object->size, size);
}

inline method(void, trimHead, RString), size_t size) {
    object->data = trimHead(object->data, &object->size, size);
}

inline method(void, trimAfterString, RString), const RString *string) {
    object->data = trimAfterSubArray(object->data, &object->size, string->data, string->size);
}

inline method(void, trimToString, RString), const RString *string) {
    object->data = trimBeforeSubArray(object->data, &object->size, string->data, string->size);
}

#pragma mark Subs and Copies

inline method(RString *, setSubstringInRange, RString), RRange range, const RString *substring) {
    object->data = setSubArrayInRange(object->data, object->size, substring->data, substring->size, range);
    return object;
}

inline method(RString *, insertSubstringAt, RString), const RString *substring, size_t place) {
    object->data = insertSubArray(object->data, &object->size, substring->data, substring->size, place);
    return object;
}

inline constMethod(RString *, substringInRange, RString), RRange range) {
    return makeRData(subArrayInRange(object->data, object->size, range), range.size, object->type);
}

constMethod(RString *, substringToSymbol, RString), char symbol) {
    size_t index = indexOfFirstByte(object->data, object->size, (byte) symbol);
    if(index != RNotFound) {
        return $(object, m(substringInRange, RString)), makeRRange(0, index));
    } else {
        return nil;
    }
}

inline constMethod(RArray *, substringsSeparatedBySymbols, RString), const RString * const separatorsString) {
    return $(object, m(dataSeparatedByBytes, RData)), separatorsString);
}

inline constMethod(RArray *, substringsSeparatedByString, RString), const RString * const separatorString) {
    return $(object, m(dataSeparatedByArray ,RData)), separatorString);
}

inline constMethod(RArray *, separatedByStringWithShield, RString), const RString * const separatorString, const RString * const shield) {
    return $(object, m(dataSeparatedByArrayWithShield, RData)), separatorString, shield);
}

inline constMethod(RString *, copy, RString)) {
    RString *copy = $(object, m(substringInRange, RString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

inline constMethod(RCompareFlags, compareWith, RString), const RString *checkString) {
    return $(object, m(compareWith, RData)), checkString);
}

inline constMethod(rbool, isEqualTo, RString), const RString *checkString) {
    return (rbool)($(object, m(compareWith, RString)), checkString) == equals);
}

inline constMethod(rbool, startsOn, RString), const RString *const checkString) {
    return isStartsOnArray(object->data, object->size, checkString->data, checkString->size);
}

inline constMethod(rbool, endsOn, RString), const RString *const checkString) {
    return isEndsOnArray(object->data, object->size, checkString->data, checkString->size);
}

#pragma mark Concatenate

inline method(void, concatenate, RString), const RString *string) {
    appendArray(&object->data, &object->size, string->data, string->size);
}

inline method(void, append, RString), const char character) {
    appendArray(&object->data, &object->size, (const byte *) &character, 1);
}

#pragma mark Conversions

method(RString*, toUpperCase, RString)) {
    size_t iterator;
    forAll(iterator, object->size) {
        if(object->data[iterator] > 96 && object->data[iterator] < 123 ) {
            object->data[iterator] -= 32;
        }
    }
    return object;
}

method(RString*, toLowerCase, RString)) {
    size_t iterator;
    forAll(iterator, object->size) {
        if(object->data[iterator] > 64 && object->data[iterator] < 91 ) {
            object->data[iterator] += 32;
        }
    }
    return object;
}
