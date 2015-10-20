/**
 * RString.h
 * Realization of wrapper on '\0' - terminated C-strings, in Ray additions.
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

#include "RString.h"
#include "RString_Char.h"
#include <stdarg.h>

#pragma mark Basics

char randomCharacter(void) {
    return (char) (rand() % 94 + 32);
}

RString * randomASCIIString(void) {
    size_t   iterator;
    size_t   size   = ((size_t)rand()) % 40 + 10;
    RString *string = c(RBytes)(nil, size);

    if(string!= nil) {
        string->type = RDataTypeASCII;
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

#pragma mark Replace

method(void, replaceCharacters, RString), char characterToReplace, char replacer) {
    if(object->type == RDataTypeASCII) {
        replaceByteWithByte(object->data, object->size, (byte) characterToReplace, (byte) replacer);
    } elseWarning(
        RWarning("RString. Replace ascii characters in not ascii string", object)
    );
}

method(void, replaceSubstrings, RString), RString *toReplace, RString *replacer) {
    replaceBytesWithBytes(object->data, &object->size, toReplace->data, toReplace->size, replacer->data, replacer->size);
}

#pragma mark Info

constMethod(size_t, numberOfCharacters, RString), char character) {
    return numberOfBytes(object->data, object->size, (byte) character);
}

constMethod(size_t, numberOfSubstrings, RString), const RString * const string) {
    return numberOfSubArrays(object->data, object->size, string->data, string->size);
}

inline constMethod(rbool, isContains, RString), char character) {
   return indexOfFirstByte(object->data, object->size, (byte) character) == RNotFound ? no : yes;
}

inline constMethod(rbool, isContainsSubstring, RString), RString *string) {
    return indexOfFirstSubArray(object->data, object->size, string->data, string->size) == RNotFound ? no : yes;
}

inline constMethod(size_t, numberOfLines, RString)) {
    return numberOfBytes(object->data, object->size, '\n');
}

constMethod(size_t, indexOfSubstring, RString), RString *string) {
    return indexOfFirstSubArray(object->data, object->size, string->data, string->size);
}

#pragma mark Deletions

method(RString *, deleteAllCharacters, RString), char character) {
    object->data = deleteAllBytes(object->data, &object->size, (byte) character);
    return object;
}

method(void, removeRepetitionsOf, RString), char character) {
    object->data = deleteRepetitionsOfByte(object->data, &object->size, (byte) character);
}

method(RString *, deleteAllSubstrings, RString), const RString *substring) {
    object->data = deleteAllSubArrays(object->data, &object->size, substring->data, substring->size);
    return object;
}

method(void, removeRepetitionsOfString, RString), const RString *substring) {
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

method(void, deleteInRange, RString), RRange range) {
    object->data = deleteInRange(object->data, &object->size, range);
}

inline
method(void, trimTail, RString), size_t size) {
    object->data = trimTail(object->data, &object->size, size);
}

inline
method(void, trimHead, RString), size_t size) {
    object->data = trimHead(object->data, &object->size, size);
}

inline
method(void, trimAfterString, RString), RString *string) {
    object->data = trimAfterSubArray(object->data, &object->size, string->data, string->size);
}

inline
method(void, trimToString, RString), RString *string) {
    object->data = trimBeforeSubArray(object->data, &object->size, string->data, string->size);
}

#pragma mark Subs and Copies

method(RString *, setSubstringInRange, RString), RRange range, const char * const string) {
    if(range.size != 0 && ((range.start + range.size - 1) < object->size)) {
        RMemMove(object->baseString + range.start, string, range.size);
    } elseError( RError3(
            "RString. setSubstringInRange. Bad range [ %lu ; %lu ] for string size %lu.\n",
                object, range.start, range.start + range.size, object->size
    ));
    return object;
}

method(RString *, insertSubstringAt, RString), RString *substring, size_t place) {
    if(place < object->size) {
        char *result = arrayAllocator(char, object->size + substring->size + 1);
        RMemMove(result,                           object->baseString,         place);
        RMemMove(result + place,                   substring->baseString,      substring->size);
        RMemMove(result + place + substring->size, object->baseString + place, object->size - place);
        deallocator(object->baseString);
        result[object->size + substring->size] = 0;

        object->size += substring->size;
        object->baseString = result;
    } else if(place == object->size) {
        $(object, m(concatenate, RString)), substring);

    } elseWarning(
            RWarning("RString. Bad place to insert!", object)
    );

    return object;
}

constMethod(RString *, substringInRange, RString), RRange range) {
    if(range.size != 0
            && ((range.start + range.size) <= object->size)) {
        char *cstring = arrayAllocator(char, range.size + 1);
        if(cstring != nil) {
            RMemCpy(cstring, object->baseString + range.start, range.size);
            RString *RString   = $(nil, c(RString)));

            if(RString != nil) {
                RString->size = range.size;
                RString->baseString = cstring;
                cstring[range.size] = 0;
                return RString;
            }
        }
    } elseError(
        RError3(
            "RString. substringInRange. Bad range [ %lu ; %lu ] for string size %lu.\n",
                object, range.start, range.start + range.size, object->size
    ));
    return nil;
}

constMethod(RString *, substringToSymbol, RString), char symbol) {
    register size_t index = indexOfFirstCharacteRString(object->baseString, object->size, symbol);
    if(index != object->size) {
        return $(object, m(substringInRange, RString)), makeRRange(0, index));
    } else {
        return nil;
    }
}

constMethod(RArray *, substringsSeparatedBySymbol, RString), char symbol) {
    // store value of original pointers and size
    RString *tempObject = $(object, m(copy, RString)));
    RArray   *result    =  nil;
    RString *string    = $(tempObject, m(substringToSymbol, RString)), symbol);

    if(string != nil) {
        result = makeRArray();
        result->destructorDelegate = (void (*)(pointer)) stringDeleter;
        result->printerDelegate    = (void (*)(pointer)) p(RString);
    }

    while(string != nil) {
        $(result, m(addObject, RArray)), string);
        tempObject->baseString += string->size + 1;
        tempObject->size       -= string->size + 1;
        string = $(tempObject, m(substringToSymbol, RString)), symbol);
        if(string == nil) {
            $(result, m(addObject, RArray)), $(tempObject, m(copy, RString))) );
        }
    }

    // size to fit RArray
    if(result != nil) {
        $(result, m(sizeToFit, RArray)) );
    }
    return result;
}

constMethod(RArray *, substringsSeparatedBySymbols, RString), const RString * const separatorsString) {
    RArray *result = nil;

    if(separatorsString != nil
            && separatorsString->size != 0
            && object != nil
            && object->size != 0) {
        register size_t  iterator;
        register size_t  endOfSubstring   = 0;
        register size_t  startOfSubstring = 0;
        register rbool   isFirst          = yes;
        RString *substring;

        forAll(iterator, object->size) {
            // check if separator
            if($(separatorsString, m(isContains, RString)), object->baseString[iterator]) == yes) {
                // if first separator set end of substring
                if(isFirst) {
                    endOfSubstring = iterator;
                    isFirst = no;
                    if(result == nil) {
                        result = makeRArray();

                        if(result != nil) {
                            // set-up delegates
                            result->printerDelegate = (void (*)(pointer)) p(RString);
                            result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                        // exit if allocation fails
                        } else {
                            RError("RString. Bad array for substrings allocation.", object);
                            return nil;
                        }
                    }
                }

            // if not separator
            } else {

                // if we found some separators
                if(!isFirst) {
                    isFirst = yes;
                    substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, endOfSubstring));
                     $(result, m(addObject, RArray)), substring);
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
            substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, endOfSubstring));
            if(substring != nil) {
                 $(result, m(addObject, RArray)), substring);
            }
            // and sizeToFit
            $(result, m(sizeToFit, RArray)) );
        }
    } elseWarning(
            RWarning("RString. substringsSeparatedBySymbols. Bad separator string size, or string size.", object)
    );
    return result;
}

constMethod(RString *, substringByBounds, RString), RBounds bounds) {
    register RRange range;
    range.start = indexOfFirstCharacteRString(object->baseString, object->size, bounds.startSymbol) + 1;
    range.size  = indexOfLastCharacteRString(object->baseString, object->size, bounds.endSymbol) - range.start;
    return $(object, m(substringInRange, RString)), range);
}

constMethod(RArray *, substringsSeparatedByString, RString), const RString * const separatorString) {
    RArray *result = nil;
    if(object->size >= separatorString->size) {
        register size_t iterator;
        register size_t startOfSubstring = 0;
        register size_t inner;
                 RString *substring = nil;

        forAll(iterator, object->size) {
            // check if separator
            if(object->baseString[iterator] == separatorString->baseString[0]) {
                // compare others
                for(inner = 1; inner < separatorString->size; ++inner) {
                    if(object->baseString[iterator + inner] != separatorString->baseString[inner]) {
                        break;
                    }
                }
                if(inner == separatorString->size) {
                    substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, iterator));
                    if(result == nil) {
                        result = makeRArray();
                        if(result != nil) {
                            // set-up delegates
                            result->printerDelegate = (void (*)(pointer)) p(RString);
                            result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                        // exit if allocation fails
                        } else {
                            RError("RString. Bad array for substrings allocation.", object);
                            return nil;
                        }
                    }
                    $(result, m(addObject, RArray)), substring);
                    startOfSubstring = iterator + inner;
                }
                iterator += inner;
            }
        }

        if(result != nil) {
            // if last is not separator - add whole last word
            if(startOfSubstring < object->size && iterator == object->size) {
                // add last
                substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, iterator));
                if(substring != nil) {
                     $(result, m(addObject, RArray)), substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RString. Bad separator string size, or string size.", object)
    );

    return result;
}

constMethod(RArray *, separatedByStringWithShield, RString), const RString * const separatorString, const RString * const shield) {
    RArray *result = nil;
    if(object->size >= separatorString->size) {
        register size_t iterator;
        register size_t startOfSubstring = 0;
        register size_t inner;
        RString *substring = nil;

        forAll(iterator, object->size) {
            // check if separator
            if(object->baseString[iterator] == separatorString->baseString[0]) {
                // compare others
                for(inner = 1; inner < separatorString->size; ++inner) {
                    if(object->baseString[iterator + inner] != separatorString->baseString[inner]) {
                        break;
                    }
                }
                if(inner == separatorString->size
                        && iterator >= shield->size) { // if separator than

                    // check if shield behind
                    for(inner = 0; inner < shield->size; ++inner) {
                        if(object->baseString[iterator - inner - 1] != shield->baseString[shield->size - inner - 1]) {
                            break;
                        }
                    }

                    if(inner != shield->size) { // if there are no shield
                        substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, iterator));
                        if(result == nil) {
                            result = makeRArray();
                            if(result != nil) {
                                // set-up delegates
                                result->printerDelegate = (void (*)(pointer)) p(RString);
                                result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                            // exit if allocation fails
                            } else {
                                RError("RString. Bad array for substrings allocation.", object);
                                return nil;
                            }
                        }
                        $(result, m(addObject, RArray)), substring);
                        startOfSubstring = iterator + separatorString->size;
                    }
                }
                iterator += inner;
            }
        }

        if(result != nil) {
            // if last is not separator - add whole last word
            if(startOfSubstring < object->size && iterator == object->size) {
                // add last
                substring = $(object, m(substringInRange, RString)), makeRRangeTo(startOfSubstring, iterator));
                if(substring != nil) {
                     $(result, m(addObject, RArray)), substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RString. Bad separator string size, or string size.", object)
    );

    return result;
}

constMethod(RString *, copy, RString)) {
    RString *copy = $(object, m(substringInRange, RString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

constMethod(RCompareFlags, compareWith, RString), const RString *checkString) {
    static size_t iterator;
    if(checkString == nil || object == nil) {
        RWarning("RString. One of compare strings is empty.", object);
        return not_equals;
    } else {
        if (checkString == object) {
            return equals;
        } else {
            if (checkString->size == object->size) {
                forAll(iterator, object->size) {
                    if(object->baseString[iterator] != checkString->baseString[iterator]){
                        return not_equals;
                    }
                }
                return equals;
            } else {
                if(checkString->size > object->size) {
                    return shorter;
                } else {
                    return longer;
                }
            }
        }
    }
}

extern
constMethod(rbool, isEqualTo, RString), const RString *checkString) {
    return (rbool)($(object, m(compareWith, RString)), checkString) == equals);
}

constMethod(rbool, startsOn, RString), const RString *const checkString) {
    return $(object, m(startsOnC, RString)), checkString->baseString);
}

constMethod(rbool, endsOn, RString), const RString *const checkString) {
    ssize_t iterator = 0;
    while(iterator < checkString->size) {
        if(object->baseString[object->size - iterator] != checkString->baseString[checkString->size - iterator]) {
            return no;
        }
        ++iterator;
    }
    return yes;
}

#pragma mark Concatenate

method(void, concatenate, RString), const RString *string) {
    if(string->size != 0 && string->baseString != nil) {
        object->baseString = RReAlloc(object->baseString, arraySize(char, string->size + object->size + 1));
        if(object->baseString != nil) {
            RMemMove(object->baseString + object->size, string->baseString, string->size);
            object->baseString[string->size + object->size] = 0;
            object->size += string->size;

        } elseError(
                RError("RString. concatenate. Realloc error.", object)
        );

    } elseWarning(
            RWarning("RString. concatenate. Bad concatenate string.", object)
    );
}

method(void, append, RString), const char character) {
    object->baseString = RReAlloc(object->baseString, arraySize(char, object->size + 2));
    if(object->baseString != nil) {
        object->baseString[object->size] = character;
        object->baseString[object->size + 1] = 0;
        ++object->size;

    } elseError( RError("RString. append. Realloc error.", object) );
}

#pragma mark Conversions

method(RString*, toUpperCase, RString)) {
    register size_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 96 && object->baseString[iterator] < 123 ) {
            object->baseString[iterator] -= 32;
        }
    }
    return object;
}

method(RString*, toLowerCase, RString)) {
    register size_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 64 && object->baseString[iterator] < 91 ) {
            object->baseString[iterator] += 32;
        }
    }
    return object;
}
