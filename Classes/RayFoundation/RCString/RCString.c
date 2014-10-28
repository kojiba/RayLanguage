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

char randomCharacter(void) {
    register char character = ((char)rand());
    while(character < 34 ||
            character > 126) {
        character = ((char)rand());
    }
    return character;
}

RCString *randomRCString(void) {
    register size_t  iterator;
             RCString *string = makeRCString();
    register size_t  size   = ((size_t)rand()) % 50;
    char     *cstring;

    while(size == 0) {
        size = ((size_t)rand()) % 50;
    }
    cstring = RAlloc(size * sizeof(char));
    forAll(iterator, size - 2){
        cstring[iterator] = randomCharacter();
    }
    cstring[++iterator] = 0;
    $(string, m(setConstantString, RCString)), cstring);
    return string;
}

#pragma mark Constructor - Destructor - Reallocation

constructor(RCString)) {
    object = allocator(RCString);
    if(object != NULL) {
        // 1 - it's for RCString
        object->classId    = 1;
        object->size       = 0;
        object->baseString = NULL;
    }
    return object;
}

destructor(RCString) {
    deallocator(object->baseString);
    object->size = 0;
}

printer(RCString) {
    RPrintf("%s\n", object->baseString);
}

method(void, flush, RCString)) {
    if(object->size != 0 && object->baseString != NULL) {
        deallocator(object->baseString);
        object->size = 0;
    }
}

#pragma mark Setters

method(RCString *, setString, RCString), const char *string) {
    if(string != NULL) {
        register size_t stringSize = RStringLenght(string) + 1;

        // checking, if exist and size like copying
        if(object->baseString == NULL) {
            object->baseString = RAlloc(stringSize * sizeof(char));

        } else if(object->size < stringSize) {
            object->baseString = RReAlloc(object->baseString, stringSize * sizeof(char));
        }

        if(object->baseString == NULL) {
            RError("RCS. SetString alloc or realloc returned NULL.", object);
        }

        // final copying
        object->size = stringSize;
        RMemMove(object->baseString, string, object->size);
        --object->size;
    } else {
        RWarning("RCS. Setted strings is empty, please delete function call, or fix it.", object);
    }
    return object;
}

method(RCString *, setConstantString, RCString), char const *string) {
    if(string != NULL) {
        // copy pointer, and compute length
        object->baseString = string;
        object->size       = RStringLenght(string);
    } else {
        RWarning("RCS. Setted strings is empty, please delete function call, or fix it.", object);
    }
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
    if(toReplace != NULL
            && replacer != NULL
            && toReplace->size != 0
            && replacer->size  != 0
            && toReplace->size <= object->size) {

        register size_t iterator;
        forAll(iterator, object->size) {

            // search first symbol
            if(object->baseString[iterator] == toReplace->baseString[0]) {

                // compare others
                if(RMemCmp(object->baseString + iterator + 1, toReplace->baseString + 1, toReplace->size - 1) == 0) {
                    // insert replacer
                    $(object, m(insertSubstringAt, RCString)), replacer, iterator);

                    // remove toReplace-string from main string
                    $(object, m(deleteInRange, RCString)), makeRRange(iterator + replacer->size, toReplace->size));
                }
            }
        }
    } else {
        RWarning("RCS. Bad strings, or sizes, please delete function call, or fix it.", object);
    }
}

#pragma mark Info

method(size_t, numberOfCharacters, RCString), char character) {
    register size_t reps     = 0;
    register size_t iterator;

    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            ++reps;
        }
    }
    return reps;
}

method(size_t, numberOfSubstrings, RCString), RCString *string) {
    if(string->size == 1) {
        return $(object, m(numberOfCharacters, RCString)), string->baseString[0]);
    } else if(object->size >= string->size) {
        size_t iterator;
        size_t count    = 0;
        forAll(iterator, object->size) {
            // search for first symbol
            if(object->baseString[iterator] == string->baseString[0]) {
                // compare others
                if(RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
                    ++count;
                }
            }
        }
       return count;
    } else {
        return 0;
    }
}

method(static inline rbool, isContains, RCString), char character) {
    size_t iterator = 0;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            return YES;
        }
    }
    return NO;
}

method(static inline rbool, isContainsSubsting, RCString), RCString *string) {
    // search for first symbol
    size_t iterator = indexOfFirstCharacterCString(object->baseString, object->size, string->baseString[0]);
    if(iterator != string->size) {
        // compare others
        if(RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
            return YES;
        } else {
            return NO;
        }
    } else {
        return NO;
    }
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
    register size_t inner;
    register byte flag = 1;

    if(substring->size != 0
            || substring->baseString == NULL) {
        forAll(iterator, object->size - substring->size) {

            // compare to substring
            if(object->baseString[iterator] == substring->baseString[0]) {
                for(inner = 1; inner < substring->size; ++inner) {

                    if(object->baseString[iterator + 1] != substring->baseString[inner]) {
                        flag = 0;
                        break;
                    }

                }
            } else {
                flag = 0;
            }

            // moving
            if(flag == 1) {
                RMemCpy(object->baseString + iterator, object->baseString + iterator + substring->size, object->size + 1 - iterator - substring->size);
                --iterator;
                object->size -= substring->size;
            } else {
                flag = 1;
            }
        }
        return object;
    } else {
        RWarning("RCS. Substring size is 0, or, substring is NULL.", object);
    }
    return NULL;
}

method(void, removeRepetitionsOfString, RCString), const RCString *substring) {
    // if they are not equals, and size is greater than 2 substrings
    if($(object->baseString, m(compareWith, RCString)), substring) != equals
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
    if(index > object->size) {
        RError("RCS. Bad index!", object);
    } else {
        RMemMove(object->baseString + index, object->baseString + index + 1, object->size - index);
    }
    return object;
}

method(void, deleteInRange, RCString), RRange range) {
    if(range.count != 0
            && ((range.from + range.count) < object->size)) {
        RMemMove(object->baseString + range.from,
                object->baseString + range.from + range.count,
                object->size - range.count - range.from + 1);
        object->size -= range.count;
        object->baseString[object->size + 1] = 0;
    } else {
        RPrintf("ERRROR. RCS. deleteInRange, bad range, do nothing.\n");
    }
}

#pragma mark Substrings and Copies

method(RCString *, setSubstringInRange, RCString), RRange range, const char *string) {
    if(range.count != 0 && ((range.from + range.count - 1) < object->size)) {
        RMemMove(object->baseString + range.from, string, range.count);
    } else {
        RError("RCS. BAD RANGE!\n", object);
    }
    return object;
}

method(RCString *, insertSubstringAt, RCString), RCString *substring, size_t place) {
    if(place < object->size) {
        char *result = RAlloc(object->size + substring->size + 1);
        RMemMove(result,                           object->baseString,         place);
        RMemMove(result + place,                   substring->baseString,      substring->size);
        RMemMove(result + place + substring->size, object->baseString + place, object->size - place);
        deallocator(object->baseString);
        result[object->size + substring->size] = 0;

        object->size += substring->size;
        object->baseString = result;
    } else if(place == object->size) {
        $(object, m(concatenate, RCString)), substring);
    } else {
        RWarning("RCS. BAD place to insert!", object);
    }

    return object;
}

method(RCString *, substringInRange, RCString), RRange range) {
    if(range.count != 0
            && ((range.from + range.count) <= object->size)) {
        char *cstring = RAlloc((range.count + 1) * sizeof(char));
        RMemMove(cstring, object->baseString + range.from, range.count);
        cstring[range.count] = 0;

        RCString *rcString   = $(NULL, c(RCString)) );
        rcString->size       = range.count;
        rcString->baseString = cstring;

        return rcString;
    } else {
        RError("RCS. BAD RANGE!\n", object);
        return NULL;
    }

}

method(RCString *, substringToSymbol, RCString), char symbol) {
    register size_t index = indexOfFirstCharacterCString(object->baseString, object->size, symbol);
    if(index != object->size) {
        return $(object, m(substringInRange, RCString)), makeRRange(0, index));
    } else {
        return NULL;
    }
}

method(RArray *, substringsSeparatedBySymbol, RCString), char symbol) {
    // store value of original pointers and size
    RCString tempObject = *object;
    RArray   *result    =  NULL;
    RCString *string    = $(object, m(substringToSymbol, RCString)), symbol);

    if(string != NULL) {
        result = makeRArray();
        result->destructorDelegate = d(RCString);
        result->printerDelegate    = p(RCString);
    }

    while(string != NULL) {
        $(result, m(addObject, RArray)), string);
        object->baseString += string->size + 1;
        object->size       -= string->size + 1;
        string = $(object, m(substringToSymbol, RCString)), symbol);
        if(string == NULL) {
            $(result, m(addObject, RArray)), $(object, m(copy, RCString))) );
        }
    }

    // restore original pointers and size
    *object = tempObject;
    // size to fit RArray
    if(result != NULL) {
        $(result, m(sizeToFit, RArray)) );
    }
    return result;
}

method(RArray *, substringsSeparatedBySymbols, RCString), RCString *separatorsString) {
    register size_t  iterator;
    register size_t  endOfSubstring   = 0;
    register size_t  startOfSubstring = 0;
    register byte      isFirst          = 1;
             RArray   *result           =  NULL;
             RCString *substring;

    if(separatorsString != NULL
            && separatorsString->size != 0
            && object != NULL
            && object->size != 0) {

        forAll(iterator, object->size) {
            // check if separator
            if($(separatorsString, m(isContains, RCString)), object->baseString[iterator]) == YES) {
                if(isFirst == 1) {
                    // if first separator set end
                    endOfSubstring = iterator;
                    isFirst = 0;
                    if(result == NULL) {
                        result = makeRArray();
                        // set-up delegates
                        result->printerDelegate    = p(RCString);
                        result->destructorDelegate = d(RCString);
                    }
                }

            // if not separator
            } else {

                // if we found some separators
                if(isFirst == 0) {
                    isFirst = 1;
                    substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, endOfSubstring));
                    addObjectToRA(result, substring);
                    startOfSubstring = iterator;
                }
            }
        }

        // if we found some
        if(result != NULL) {
            // if last it is not separator
            if(endOfSubstring < object->size) {
                endOfSubstring = object->size;
            }
            // add last and sizeToFit
            substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, endOfSubstring));
            if(substring != NULL) {
                addObjectToRA(result, substring);
            }
            $(result, m(sizeToFit, RArray)) );
        }
    }
    return result;
}

method(RCString *, substringByBounds, RCString), RBounds bounds) {
    register RRange range;
    range.from  = indexOfFirstCharacterCString(object->baseString, object->size, bounds.startSymbol) + 1;
    range.count = indexOfLastCharacterCString (object->baseString, object->size, bounds.endSymbol) - range.from;
    return $(object, m(substringInRange, RCString)), range);
}

method(RCString *, copy, RCString)) {
    RCString *copy = $(object, m(substringInRange, RCString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

method(RCompareFlags, compareWith, RCString), const RCString *checkString) {
    static size_t iterator;
    if(checkString == NULL || object == NULL) {
        RWarning("RCS. One of compare strings is empty, please delete function call, or fix it.", object);
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
                if(checkString->size == object->size) {
                    return shorter;
                } else {
                    return longer;
                }
            }
        }
    }
    return not_equals;
}

#pragma mark With file

method(void, fromFile, RCString), const RCString *filename) {
    FILE *file = RFOpen(filename->baseString, "rb");
    char *buffer;
    long fileSize;

    if(file != NULL) {
        RFSeek(file, 0, SEEK_END);
        fileSize = RFTell(file);
        RRewind(file);
        buffer = RAlloc(fileSize * (sizeof(char)));
        RFRead(buffer, sizeof(char), fileSize, file);
        RFClose(file);
        $(object, m(setConstantString, RCString)), buffer);
    } else {
        RWarning("RCS. Cannot open file.\n", object);
    }
}

#pragma mark Concatenate

method(void, concatenate, RCString), const RCString *string) {
    if(string->size != 0 && string->baseString != NULL) {
        object->baseString = RReAlloc(object->baseString, string->size + object->size + 1);
        if(object->baseString == NULL) {
            RError("RCS. Concatenate realloc error.", object);
        } else {
            RMemMove(object->baseString + object->size, string->baseString, string->size);
            object->baseString[string->size + object->size] = 0;
            object->size += string->size;
        }
    } else {
        RWarning("RCS. Bad concatenate string.", object);
    }
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
