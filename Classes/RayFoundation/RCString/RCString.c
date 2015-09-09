/**
 * RCString.h
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

#include "RCString.h"
#include "RCString_Char.h"
#include <stdarg.h>

#pragma mark Basics

size_t indexOfFirstCharacterCString(const char *string, size_t size, char character) {
    register size_t iterator = 0;
    while(iterator < size) {
        if(string[iterator] == character) {
            break;
        } else {
            ++iterator;
        }
    }
    return iterator;
}

size_t indexOfLastCharacterCString(const char *string, size_t size, char character) {
    register size_t iterator = 0;
    register size_t last     = size;
    while(iterator < size) {
        if(string[iterator] == character) {
            last = iterator;
        }
        ++iterator;
    }
    return last;
}
char * copyOfCString(const char *string) {
    size_t length = RStringLength(string);
    if(length > 0) {
        char *result = arrayAllocator(char, length + 1);
        if(result != nil) {
            RMemCpy(result, string, length);
            return result;
        }
    }
    return nil;
}

char * substringInRange(const char *string, RRange range) {
    char     *result = nil;
    RCString *temp   = RCS(string);
    if(temp != nil) {
        RCString *sub = $(temp, m(substringInRange, RCString)), range);
        if(sub != nil) {
            result = sub->baseString;
            deallocator(sub);
        }
        deallocator(temp);
    }
    return result;
}

char randomCharacter(void) {
    register char character = ((char)rand());
    while(!(character > 34 &&
            character < 126)) {
        character = ((char)rand());
    }
    return character;
}

RCString * randomRCString(void) {
    register size_t  iterator;
             RCString *string = makeRCString();
    register size_t  size   = ((size_t)rand()) % 50;
    char     *cstring;

    if(string!= nil) {
        while(size < 10) {
            size = ((size_t)rand()) % 50;
        }
        cstring = arrayAllocator(char, size + 1);
        if(cstring != nil) {
            forAll(iterator, size){
                cstring[iterator] = randomCharacter();
            }
            cstring[size] = 0;
            $(string, m(setConstantString, RCString)), cstring);

        } elseError(
                RError("RCString. Allocation of temp cstring error.", (pointer) cstring)
        );

    } elseError(
            RError("RCString. randomRCString. Bad allocation of result.", nil)
    );

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

#pragma mark Constructor - Destructor - Reallocation

constructor(RCString)) {
    object = allocator(RCString);
    if(object != nil) {
        // 1 - it's for RCString
        object->classId    = 1;
        object->size       = 0;
        object->baseString = nil;
    }
    return object;
}

destructor(RCString) {
    deallocator(object->baseString);
    object->size = 0;
}

printer(RCString) {
    if(object != nil
       && object->baseString != nil) {
        RPrintf("%s\n", object->baseString);
    } else {
        RPrintf("nil\n");
    }
}

method(void, flush, RCString)) {
    if(object->size != 0 && object->baseString != nil) {
        deallocator(object->baseString);
        object->size = 0;
    }
}

RCString * stringWithFormat(char *string, ...) {
    va_list args;
    RCString *result = nil;
    char *buffer;
    va_start(args, string);
    buffer = vcstringWithFormat(string, args);
    if(buffer != nil) {
        result = makeRCString();
        $(result, m(setConstantString, RCString)), buffer);
    }
    va_end(args);
    return result;
}

RCString * RCStringInit(pointer data, size_t size){
    RCString * object = allocator(RCString);
    if(object != nil) {
        // 1 - it's for RCString
        object->classId    = 1;
        object->size       = size;
        object->baseString = data;
    }
    return object;
}

void stringDeleter(RCString *string) {
    deleter(string, RCString);
}

#pragma mark Setters

method(RCString *, setString, RCString), const char *string) {
    if(string != nil && object != nil) {
        register size_t stringSize = RStringLength(string) + 1;

        // checking, if exist and size like copying
        if(object->baseString == nil) {
            object->baseString = arrayAllocator(char, stringSize);

        } else if(object->size < stringSize) {
            object->baseString = RReAlloc(object->baseString, arraySize(char, stringSize));
        }

        if(object->baseString != nil) {
            // final copying
            object->size = stringSize;
            RMemCpy(object->baseString, string, object->size);
            --object->size;

        } elseError(
                RError("RCString. setString alloc or realloc returned nil.", object)
        );

    } elseWarning(
            RWarning("RCString. Setted strings or container is empty.", object)
    );

    return object;
}

method(RCString *, setConstantString, RCString), char const *string) {
    if(string != nil) {
        // copy pointer, and compute length
        object->baseString = (char *) string;
        object->size       = RStringLength(string);

    } elseWarning(
            RWarning("RCS. Setted strings is empty.", object)
    );

    return object;
}

#pragma mark Replace

method(void, replaceCharacters, RCString), char characterToReplace, char replacer) {
    register size_t iterator = 0;
    while(iterator < object->size) {
        if(object->baseString[iterator] == characterToReplace) {
            object->baseString[iterator] = replacer;
        }
        ++iterator;
    }
}

method(void, replaceSubstrings, RCString), RCString *toReplace, RCString *replacer) {
    if(toReplace != nil
            && replacer != nil
            && toReplace->size != 0
            && replacer->size  != 0
            && toReplace->size <= object->size) {

        register size_t iterator;
        forAll(iterator, object->size - toReplace->size + 1) {

            // search first symbol
            if(object->baseString[iterator] == toReplace->baseString[0]) {

                // compare others
                if(RMemCmp(object->baseString + iterator + 1, toReplace->baseString + 1, toReplace->size - 1) == 0) {
                    // insert replacer
                    $(object, m(insertSubstringAt, RCString)), replacer, iterator);

                    // remove toReplace-string start main string
                    $(object, m(deleteInRange, RCString)), makeRRange(iterator + replacer->size, toReplace->size));

                    iterator += replacer->size;
                }
            }
        }

    } elseWarning(
            RWarning("RCString. replaceSubstrings. Bad strings, or sizes.", object)
    );
}

#pragma mark Info

constMethod(size_t, numberOfCharacters, RCString), char character) {
    register size_t reps     = 0;
    register size_t iterator;

    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            ++reps;
        }
    }
    return reps;
}

constMethod(size_t, numberOfSubstrings, RCString), const RCString * const string) {
    if(string != nil
       && string->size != 0
       && string->baseString != nil) {
        if(string->size == 1) {
            return $(object, m(numberOfCharacters, RCString)), string->baseString[0]);
        } else if(object->size >= string->size) {
            size_t iterator;
            size_t count    = 0;
            forAll(iterator, object->size - string->size + 1) {
                // search for first symbol
                if(object->baseString[iterator] == string->baseString[0]) {
                    // compare others
                    if(RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
                        ++count;
                    }
                }
            }
           return count;
        }
    } elseWarning(
            RWarning("RCString. numberOfSubstrings. Bad string to check.", object)
    )
    return 0;
}

inline constMethod(rbool, isContains, RCString), char character) {
    size_t iterator = 0;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            return yes;
        }
    }
    return no;
}

inline constMethod(rbool, isContainsSubstring, RCString), RCString *string) {
    if(string != nil
       && string->baseString != nil
       && object->size >= string->size) {
        // search for first symbol
        size_t iterator = indexOfFirstCharacterCString(object->baseString, object->size, string->baseString[0]);
        if (iterator != string->size) {
            // compare others
            if (RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
                return yes;
            } else {
                return no;
            }
        } else {
            return no;
        }
    } else {
        RWarning("RCString. isContainsSubstring. Bad string to check", object);
        return no;
    }
}

inline constMethod(size_t, numberOfLines, RCString)) {
    return $(object, m(numberOfCSubstrings, RCString)), "\n");
}

constMethod(size_t, indexOfSubstring, RCString), RCString *string) {
    if(string != nil
            && string->baseString != nil
            && string->size <= object->size) {
        size_t iterator;
        forAll(iterator, object->size - string->size + 1) {
            // search for first symbol
            if(object->baseString[iterator] == string->baseString[0]) {
                // compare others
                if(RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
                    return iterator;
                }
            }
        }
    } elseWarning(
            RWarning("RCString. indexOfSubstring. Bad string to check", object)
    );
    return object->size;
}

#pragma mark Deletions

method(RCString *, deleteAllCharacters, RCString), char character) {
    register size_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            RMemCpy(object->baseString + iterator, object->baseString + iterator + 1, object->size + 1 - iterator);
            --iterator;
            --object->size;
        }
    }
    return object;
}

method(void, removeRepetitionsOf, RCString), char character) {
    register size_t iterator;
    register size_t inner;
    forAll(iterator, object->size - 1) {

        // search first occurrence
        if(object->baseString[iterator] == character) {

            // find length of repetition
            for(inner = 0; inner < object->size - iterator; ++inner) {
                if(object->baseString[inner + iterator + 1] != character){
                    break;
                }
            }

            // delete if length > 0
            if(inner > 0) {
                $(object, m(deleteInRange, RCString)), makeRRange(iterator + 1, inner));
            }
        }
    }
}

method(RCString *, deleteAllSubstrings, RCString), const RCString *substring) {
    register size_t iterator;
    register rbool  needMove = yes;

    if(substring->size != 0
            && substring->baseString != nil
            && object->size >= substring->size) {

        forAll(iterator, object->size - substring->size) {
            // search first symbol
            if(object->baseString[iterator] == substring->baseString[0]) {
                // compare others
                if(RMemCmp(object->baseString + iterator + 1, substring->baseString + 1, substring->size - 1) != 0) {
                    needMove = no;
                }
            } else {
                needMove = no;
            }
            // moving
            if(needMove) {
                RMemCpy(object->baseString + iterator, object->baseString + iterator + substring->size, object->size + 1 - iterator - substring->size);
                --iterator;
                object->size -= substring->size;
            } else {
                needMove = yes;
            }
        }
        return object;

    } elseWarning(
            RWarning("RCS. Substring size is 0, or, substring is nil, or size of substring > size of string.", object)
    );

    return nil;
}

method(void, removeRepetitionsOfString, RCString), const RCString *substring) {
    // if they are not equals, and size is greater than 2 substrings
    if($(object, m(compareWith, RCString)), substring) != equals
            && object->size >= substring->size * 2) {

        register size_t iterator;
        register size_t place;
        register size_t repetitionsCount;

        forAll(iterator, object->size - substring->size) {
            // first symbols compare
            if(object->size - iterator >= substring->size
                    && object->baseString[iterator] == substring->baseString[0]) {

                // compare others
                if(RMemCmp(object->baseString + iterator + 1, substring->baseString + 1, substring->size - 1) == 0) {

                    // add to iterator substring->size
                    iterator += substring->size;
                    place     = iterator;

                    for(repetitionsCount = 0; repetitionsCount < (object->size / substring->size - 1); ) {
                        if (RMemCmp(object->baseString + iterator, substring->baseString, substring->size) == 0) {
                            ++repetitionsCount;
                            iterator += substring->size;
                        } else {
                            break;
                        }
                    }

                    if(repetitionsCount > 0) {
                        // delete in range
                        $(object, m(deleteInRange, RCString)), makeRRange(place, repetitionsCount * substring->size));
                        iterator -= repetitionsCount * substring->size;
                    }
                }
            }
        }
    }
}

method(RCString *, deleteCharacterAt, RCString), size_t index) {
    if(index < object->size) {
        RMemMove(object->baseString + index, object->baseString + index + 1, object->size - index);

    } elseError(
            RError("RCS. Bad index!", object)
    );

    return object;
}

method(void, deleteInRange, RCString), RRange range) {
    if(range.size != 0
            && ((range.start + range.size) <= object->size)) {
        RMemMove(object->baseString + range.start,
                object->baseString + range.start + range.size,
                object->size - range.size - range.start + 1);
        object->size -= range.size;
        object->baseString[object->size + 1] = 0;

    } elseError(
            RError("RCString. deleteInRange, bad range, do nothing.", object)
    );
}

inline
method(void, trimTail, RCString), size_t size) {
    $(object, m(deleteInRange, RCString)), makeRRange(object->size - size, size));
}

inline
method(void, trimHead, RCString), size_t size) {
    $(object, m(deleteInRange, RCString)), makeRRange(0, size));
}
inline
method(void, trimAfterString, RCString), RCString *string) {
    size_t index = $(object, m(indexOfSubstring, RCString)), string);
    if(index != object->size) {
        $(object, m(deleteInRange, RCString)), makeRRangeTo(index, object->size));
    }
}

inline
method(void, trimToString, RCString), RCString *string) {
    size_t index = $(object, m(indexOfSubstring, RCString)), string);
    if(index != object->size
            && index != 0) {
        $(object, m(deleteInRange, RCString)), makeRRangeTo(index, object->size));
    }
}

#pragma mark Subs and Copies

method(RCString *, setSubstringInRange, RCString), RRange range, const char * const string) {
    if(range.size != 0 && ((range.start + range.size - 1) < object->size)) {
        RMemMove(object->baseString + range.start, string, range.size);
    } elseError( RError3(
            "RCString. setSubstringInRange. Bad range [ %lu ; %lu ] for string size %lu.\n",
                object, range.start, range.start + range.size, object->size
    ));
    return object;
}

method(RCString *, insertSubstringAt, RCString), RCString *substring, size_t place) {
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
        $(object, m(concatenate, RCString)), substring);

    } elseWarning(
            RWarning("RCString. Bad place to insert!", object)
    );

    return object;
}

constMethod(RCString *, substringInRange, RCString), RRange range) {
    if(range.size != 0
            && ((range.start + range.size) <= object->size)) {
        char *cstring = arrayAllocator(char, range.size + 1);
        if(cstring != nil) {
            RMemCpy(cstring, object->baseString + range.start, range.size);
            RCString *rcString   = $(nil, c(RCString)));

            if(rcString != nil) {
                rcString->size = range.size;
                rcString->baseString = cstring;
                cstring[range.size] = 0;
                return rcString;
            }
        }
    } elseError(
        RError3(
            "RCString. substringInRange. Bad range [ %lu ; %lu ] for string size %lu.\n",
                object, range.start, range.start + range.size, object->size
    ));
    return nil;
}

constMethod(RCString *, substringToSymbol, RCString), char symbol) {
    register size_t index = indexOfFirstCharacterCString(object->baseString, object->size, symbol);
    if(index != object->size) {
        return $(object, m(substringInRange, RCString)), makeRRange(0, index));
    } else {
        return nil;
    }
}

constMethod(RArray *, substringsSeparatedBySymbol, RCString), char symbol) {
    // store value of original pointers and size
    RCString *tempObject = $(object, m(copy, RCString)));
    RArray   *result    =  nil;
    RCString *string    = $(tempObject, m(substringToSymbol, RCString)), symbol);

    if(string != nil) {
        result = makeRArray();
        result->destructorDelegate = (void (*)(pointer)) stringDeleter;
        result->printerDelegate    = (void (*)(pointer)) p(RCString);
    }

    while(string != nil) {
        $(result, m(addObject, RArray)), string);
        tempObject->baseString += string->size + 1;
        tempObject->size       -= string->size + 1;
        string = $(tempObject, m(substringToSymbol, RCString)), symbol);
        if(string == nil) {
            $(result, m(addObject, RArray)), $(tempObject, m(copy, RCString))) );
        }
    }

    // size to fit RArray
    if(result != nil) {
        $(result, m(sizeToFit, RArray)) );
    }
    return result;
}

constMethod(RArray *, substringsSeparatedBySymbols, RCString), const RCString * const separatorsString) {
    RArray *result = nil;

    if(separatorsString != nil
            && separatorsString->size != 0
            && object != nil
            && object->size != 0) {
        register size_t  iterator;
        register size_t  endOfSubstring   = 0;
        register size_t  startOfSubstring = 0;
        register rbool   isFirst          = yes;
        RCString *substring;

        forAll(iterator, object->size) {
            // check if separator
            if($(separatorsString, m(isContains, RCString)), object->baseString[iterator]) == yes) {
                // if first separator set end of substring
                if(isFirst) {
                    endOfSubstring = iterator;
                    isFirst = no;
                    if(result == nil) {
                        result = makeRArray();

                        if(result != nil) {
                            // set-up delegates
                            result->printerDelegate = (void (*)(pointer)) p(RCString);
                            result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                        // exit if allocation fails
                        } else {
                            RError("RCString. Bad array for substrings allocation.", object);
                            return nil;
                        }
                    }
                }

            // if not separator
            } else {

                // if we found some separators
                if(!isFirst) {
                    isFirst = yes;
                    substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, endOfSubstring));
                    addObjectToRA(result, substring);
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
            substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, endOfSubstring));
            if(substring != nil) {
                addObjectToRA(result, substring);
            }
            // and sizeToFit
            $(result, m(sizeToFit, RArray)) );
        }
    } elseWarning(
            RWarning("RCString. substringsSeparatedBySymbols. Bad separator string size, or string size.", object)
    );
    return result;
}

constMethod(RCString *, substringByBounds, RCString), RBounds bounds) {
    register RRange range;
    range.start = indexOfFirstCharacterCString(object->baseString, object->size, bounds.startSymbol) + 1;
    range.size  = indexOfLastCharacterCString(object->baseString, object->size, bounds.endSymbol) - range.start;
    return $(object, m(substringInRange, RCString)), range);
}

constMethod(RArray *, substringsSeparatedByString, RCString), const RCString * const separatorString) {
    RArray *result = nil;
    if(object->size >= separatorString->size) {
        register size_t iterator;
        register size_t startOfSubstring = 0;
        register size_t inner;
                 RCString *substring = nil;

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
                    substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, iterator));
                    if(result == nil) {
                        result = makeRArray();
                        if(result != nil) {
                            // set-up delegates
                            result->printerDelegate = (void (*)(pointer)) p(RCString);
                            result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                        // exit if allocation fails
                        } else {
                            RError("RCString. Bad array for substrings allocation.", object);
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
                substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, iterator));
                if(substring != nil) {
                    addObjectToRA(result, substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RCString. Bad separator string size, or string size.", object)
    );

    return result;
}

constMethod(RArray *, separatedByStringWithShield, RCString), const RCString * const separatorString, const RCString * const shield) {
    RArray *result = nil;
    if(object->size >= separatorString->size) {
        register size_t iterator;
        register size_t startOfSubstring = 0;
        register size_t inner;
        RCString *substring = nil;

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
                        substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, iterator));
                        if(result == nil) {
                            result = makeRArray();
                            if(result != nil) {
                                // set-up delegates
                                result->printerDelegate = (void (*)(pointer)) p(RCString);
                                result->destructorDelegate = (void (*)(pointer)) stringDeleter;

                            // exit if allocation fails
                            } else {
                                RError("RCString. Bad array for substrings allocation.", object);
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
                substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, iterator));
                if(substring != nil) {
                    addObjectToRA(result, substring);
                }
            }
            $(result, m(sizeToFit, RArray)));
        }
    } elseWarning(
            RWarning("RCString. Bad separator string size, or string size.", object)
    );

    return result;
}

constMethod(RCString *, copy, RCString)) {
    RCString *copy = $(object, m(substringInRange, RCString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

constMethod(RCompareFlags, compareWith, RCString), const RCString *checkString) {
    static size_t iterator;
    if(checkString == nil || object == nil) {
        RWarning("RCString. One of compare strings is empty.", object);
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
constMethod(rbool, isEqualTo, RCString), const RCString *checkString) {
    return (rbool)($(object, m(compareWith, RCString)), checkString) == equals);
}

constMethod(rbool, startsOn, RCString), const RCString *const checkString) {
    return $(object, m(startsOnC, RCString)), checkString->baseString);
}

constMethod(rbool, endsOn, RCString), const RCString *const checkString) {
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

method(void, concatenate, RCString), const RCString *string) {
    if(string->size != 0 && string->baseString != nil) {
        object->baseString = RReAlloc(object->baseString, arraySize(char, string->size + object->size + 1));
        if(object->baseString != nil) {
            RMemMove(object->baseString + object->size, string->baseString, string->size);
            object->baseString[string->size + object->size] = 0;
            object->size += string->size;

        } elseError(
                RError("RCString. concatenate. Realloc error.", object)
        );

    } elseWarning(
            RWarning("RCString. concatenate. Bad concatenate string.", object)
    );
}

method(void, append, RCString), const char character) {
    object->baseString = RReAlloc(object->baseString, arraySize(char, object->size + 2));
    if(object->baseString != nil) {
        object->baseString[object->size] = character;
        object->baseString[object->size + 1] = 0;
        ++object->size;

    } elseError( RError("RCString. append. Realloc error.", object) );
}

#pragma mark Conversions

method(RCString*, toUpperCase, RCString)) {
    register size_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 96 && object->baseString[iterator] < 123 ) {
            object->baseString[iterator] -= 32;
        }
    }
    return object;
}

method(RCString*, toLowerCase, RCString)) {
    register size_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 64 && object->baseString[iterator] < 91 ) {
            object->baseString[iterator] += 32;
        }
    }
    return object;
}
