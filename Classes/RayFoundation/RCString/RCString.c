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
char* copyOfString(const char *string) {
    size_t length = RStringLenght(string);
    if(length > 0) {
        char *result = RAlloc(length * sizeof(char));
        if(result != nil) {
            RMemCpy(result, string, length);
            return result;
        } else {
            RError("CString. Bad allocation", (pointer) result);
            return nil;
        }
    } else {
        return nil;
    }
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

    if(string!= nil) {
        while(size == 0) {
            size = ((size_t)rand()) % 50;
        }
        cstring = RAlloc(size * sizeof(char));
        if(cstring != nil) {
            forAll(iterator, size - 1){
                cstring[iterator] = randomCharacter();
            }
            cstring[++iterator] = 0;
            $(string, m(setConstantString, RCString)), cstring);
        } else {
            RError("RCString. Allocation of temp cstring error.", (pointer) cstring);
        }
    } else {
        RError("RCString. randomRCString. Bad allocation of result.", nil);
    }
    return string;
}

#pragma mark  Make RCS constant start constant ""-string

RCString makeRCSConstant(char *string) {
    RCString result;
    result.baseString = string;
    result.size       = strlen(string);
    result.classId    = 1;
    return result;
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
    RPrintf("%s\n", object->baseString);
}

method(void, flush, RCString)) {
    if(object->size != 0 && object->baseString != nil) {
        deallocator(object->baseString);
        object->size = 0;
    }
}

#pragma mark Setters

method(RCString *, setString, RCString), const char *string) {
    if(string != nil && object != nil) {
        register size_t stringSize = RStringLenght(string) + 1;

        // checking, if exist and size like copying
        if(object->baseString == nil) {
            object->baseString = RAlloc(stringSize * sizeof(char));

        } else if(object->size < stringSize) {
            object->baseString = RReAlloc(object->baseString, stringSize * sizeof(char));
        }

        if(object->baseString == nil) {
            RError("RCS. SetString alloc or realloc returned nil.", object);
        }

        // final copying
        object->size = stringSize;
        RMemMove(object->baseString, string, object->size);
        --object->size;
    } else {
        RWarning("RCS. Setted strings or container is empty, please delete function call, or fix it.", object);
    }
    return object;
}

method(RCString *, setConstantString, RCString), char const *string) {
    if(string != nil) {
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
    if(toReplace != nil
            && replacer != nil
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

                    // remove toReplace-string start main string
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
            return yes;
        }
    }
    return no;
}

method(static inline rbool, isContainsSubsting, RCString), RCString *string) {
    // search for first symbol
    size_t iterator = indexOfFirstCharacterCString(object->baseString, object->size, string->baseString[0]);
    if(iterator != string->size) {
        // compare others
        if(RMemCmp(object->baseString + iterator + 1, string->baseString + 1, string->size - 1) == 0) {
            return yes;
        } else {
            return no;
        }
    } else {
        return no;
    }
}

method(size_t, numberOfLines, RCString)) {
    return $(object, m(numberOfCharacters, RCString)), '\n');
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
            || substring->baseString == nil) {
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
        RWarning("RCS. Substring size is 0, or, substring is nil.", object);
    }
    return nil;
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
    if(range.size != 0
            && ((range.start + range.size) <= object->size)) {
        RMemMove(object->baseString + range.start,
                object->baseString + range.start + range.size,
                object->size - range.size - range.start + 1);
        object->size -= range.size;
        object->baseString[object->size + 1] = 0;
    } else {
        RError("RCS. deleteInRange, bad range, do nothing.", object);
    }
}

method(void, trimTail, RCString), size_t size) {
    $(object, m(deleteInRange, RCString)), makeRRange(object->size - size, size));
}

method(void, trimHead, RCString), size_t size) {
    $(object, m(deleteInRange, RCString)), makeRRange(0, size));
}

#pragma mark Substrings and Copies

method(RCString *, setSubstringInRange, RCString), RRange range, const char *string) {
    if(range.size != 0 && ((range.start + range.size - 1) < object->size)) {
        RMemMove(object->baseString + range.start, string, range.size);
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
    if(range.size != 0
            && ((range.start + range.size) <= object->size)) {
        char *cstring = RAlloc((range.size + 1) * sizeof(char));
        RMemMove(cstring, object->baseString + range.start, range.size);
        cstring[range.size] = 0;

        RCString *rcString   = $(nil, c(RCString)) );
        rcString->size       = range.size;
        rcString->baseString = cstring;

        return rcString;
    } else {
        RError("RCS. BAD RANGE!\n", object);
        return nil;
    }

}

method(RCString *, substringToSymbol, RCString), char symbol) {
    register size_t index = indexOfFirstCharacterCString(object->baseString, object->size, symbol);
    if(index != object->size) {
        return $(object, m(substringInRange, RCString)), makeRRange(0, index));
    } else {
        return nil;
    }
}

method(RArray *, substringsSeparatedBySymbol, RCString), char symbol) {
    // store value of original pointers and size
    RCString tempObject = *object;
    RArray   *result    =  nil;
    RCString *string    = $(object, m(substringToSymbol, RCString)), symbol);

    if(string != nil) {
        result = makeRArray();
        result->destructorDelegate = (void (*)(pointer)) d(RCString);
        result->printerDelegate    = (void (*)(pointer)) p(RCString);
    }

    while(string != nil) {
        $(result, m(addObject, RArray)), string);
        object->baseString += string->size + 1;
        object->size       -= string->size + 1;
        string = $(object, m(substringToSymbol, RCString)), symbol);
        if(string == nil) {
            $(result, m(addObject, RArray)), $(object, m(copy, RCString))) );
        }
    }

    // restore original pointers and size
    *object = tempObject;
    // size to fit RArray
    if(result != nil) {
        $(result, m(sizeToFit, RArray)) );
    }
    return result;
}

method(RArray *, substringsSeparatedBySymbols, RCString), RCString *separatorsString) {
    register size_t  iterator;
    register size_t  endOfSubstring   = 0;
    register size_t  startOfSubstring = 0;
    register byte      isFirst          = 1;
             RArray   *result           =  nil;
             RCString *substring;

    if(separatorsString != nil
            && separatorsString->size != 0
            && object != nil
            && object->size != 0) {

        forAll(iterator, object->size) {
            // check if separator
            if($(separatorsString, m(isContains, RCString)), object->baseString[iterator]) == yes) {
                if(isFirst == 1) {
                    // if first separator set end
                    endOfSubstring = iterator;
                    isFirst = 0;
                    if(result == nil) {
                        result = makeRArray();
                        // set-up delegates
                        result->printerDelegate    = (void (*)(pointer)) p(RCString);
                        result->destructorDelegate = (void (*)(pointer)) d(RCString);
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
        if(result != nil) {
            // if last it is not separator
            if(endOfSubstring < object->size) {
                endOfSubstring = object->size;
            }
            // add last and sizeToFit
            substring = $(object, m(substringInRange, RCString)), makeRRangeTo(startOfSubstring, endOfSubstring));
            if(substring != nil) {
                forAll(iterator, separatorsString->size) {
                    $(substring, m(deleteAllCharacters, RCString)), separatorsString->baseString[iterator]);
                }
                addObjectToRA(result, substring);
            }
            $(result, m(sizeToFit, RArray)) );
        }
    }
    return result;
}

method(RArray *, substringsSeparatedBySymCStr, RCString), char *separatorsString) {
    RCString *temp = RS(separatorsString);
    RArray *result = $(object, m(substringsSeparatedBySymbols, RCString)), temp);
    deallocator(temp);
    return result;
}

method(RCString *, substringByBounds, RCString), RBounds bounds) {
    register RRange range;
    range.start = indexOfFirstCharacterCString(object->baseString, object->size, bounds.startSymbol) + 1;
    range.size = indexOfLastCharacterCString (object->baseString, object->size, bounds.endSymbol) - range.start;
    return $(object, m(substringInRange, RCString)), range);
}

method(RCString *, copy, RCString)) {
    RCString *copy = $(object, m(substringInRange, RCString)), makeRRange(0, object->size));
    return copy;
}

#pragma mark Comparator

method(RCompareFlags, compareWith, RCString), const RCString *checkString) {
    static size_t iterator;
    if(checkString == nil || object == nil) {
        RWarning("RCS. One of compare strings is empty.", object);
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
}

#pragma mark Concatenate

method(void, concatenate, RCString), const RCString *string) {
    if(string->size != 0 && string->baseString != nil) {
        object->baseString = RReAlloc(object->baseString, string->size + object->size + 1);
        if(object->baseString == nil) {
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

method(void, appendString, RCString), const char *string) {
    if(string != nil) {
        size_t stringSize = RStringLenght(string);
        object->baseString = RReAlloc(object->baseString, stringSize + object->size + 1);
        if(object->baseString == nil) {
            RError("RCS. Concatenate realloc error.", object);
        } else {
            RMemMove(object->baseString + object->size, string, stringSize);
            object->baseString[stringSize + object->size] = 0;
            object->size += stringSize;
        }
    } else {
        RWarning("RCS. Bad concatenate string.", object);
    }
}

method(void, append, RCString), const char character) {
    object->baseString = RReAlloc(object->baseString, object->size + 2);
    if(object->baseString == nil) {
        RError("RCS. Concatenate realloc error.", object);
    } else {
        object->baseString[object->size] = character;
        object->baseString[object->size + 1] = 0;
        ++object->size;
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

#pragma mark With file

RCString* RCStringFromFile(const char *filename) {
    FILE *file = RFOpen(filename, "rb");
    char *buffer;
    ssize_t fileSize;

    if(file != nil) {
        RFSeek(file, 0, SEEK_END);
        fileSize = RFTell(file);
        if(fileSize > 0) {
            RRewind(file);
            buffer = RAlloc(fileSize * (sizeof(char)));
            if(buffer != nil) {
                RFRead(buffer, sizeof(char), fileSize, file);
                RFClose(file);
                RCString *result = $(nil, c(RCString)));
                if(result != nil) {
                    result->baseString = buffer;
                    result->size = (size_t) fileSize;
                    return result;
                } else {
                    RError("RCStringFromFile. Bad allocation of RCString.", nil);
                }
            } else {
                RErrStr "RCStringFromFile. Bad allocation buffer for file \"%s\" of size \"%lu\".\n", filename, fileSize);
            }
        } else {
            RErrStr "RCStringFromFile. Error read file \"%s\".\n", filename);
        }
    } else {
        RErrStr "RCStringFromFile. Cannot open file \"%s\".\n", filename);
    }
    return nil;
}

method(void, appendToFile, RCString), const char *filename) {
    FILE *file = fopen(filename, "ab");
    if (file != nil) {
        ssize_t result = fputs(object->baseString, file);
        if(result < 0) {
            RError("RCString. Failed save string to file.", object);
        }
        fclose(file);
    } else {
        RError("RCString. Failed save string to file, cant open file.", object);
    }
}
