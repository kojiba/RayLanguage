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

uint64_t indexOfFirstCharacterCString(const char *string, uint64_t size, char character) {
    register uint64_t iterator = 0;
    while(iterator < size) {
        if(string[iterator] == character) {
            break;
        } else {
            ++iterator;
        }
    }
    return iterator;
}

uint64_t indexOfLastCharacterCString(const char *string, uint64_t size, char character) {
    register uint64_t iterator = 0;
    register uint64_t last = size;
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
    register uint64_t  iterator;
    RCString *string    = makeRCString();
    register uint64_t  size      = ((uint64_t)rand()) % 50;
    char     *cstring;

    while(size == 0) {
        size = ((uint64_t)rand()) % 50;
    }
    cstring = RAlloc(size * sizeof(char));
    forAll(iterator, size){
        cstring[iterator] = randomCharacter();
    }
    $(string, m(setConstantString, RCString)), cstring);
    return string;
}

#pragma mark constructor - destructor - reallocation

constructor(RCString)) {
    object = allocator(RCString);
    if(object) {
        // 1 - it's for RCString
        object->classId = 1;
        object->size = 0;
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
        register uint64_t stringSize = RStringLenght(string) + 1;

        // checking, if exist and size like copying
        if(object->size != 0 && object->baseString != NULL
                || object->size < stringSize) {
            deallocator(object->baseString);
            object->baseString = RAlloc(stringSize * sizeof(char));
        }

        // final copying
        object->size = stringSize;
        RMemCpy(object->baseString, string, object->size);
        --object->size;
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

method(RCString *, setConstantString, RCString), char const *string) {
    if(string != NULL) {
        // copy pointer, and compute length
        object->baseString = string;
        object->size       = RStringLenght(string);
    } else {
        RPrintf("Warning. RCS. Setted strings is empty, please delete function call, or fix it.\n");
    }
    return object;
}

#pragma mark Replace

method(void, replaceCharacters, RCString), char characterToReplace, char replacer) {
    register uint64_t iterator = 0;
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

        register uint64_t iterator;
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
        RPrintf("Warning. RCS. Bad strings, or sizes, please delete function call, or fix it.\n");
    }
}

#pragma mark Info

method(uint64_t, numberOfRepetitions, RCString), char character) {
    register uint64_t reps     = 0;
    register uint64_t iterator;

    forAll(iterator, object->size) {
        if(object->baseString[iterator] == character) {
            ++reps;
        }
    }
    return reps;
}

#pragma mark Deletions

method(RCString *, deleteAllCharacters, RCString), char character) {
    register uint64_t iterator;
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
    register uint64_t iterator;
    register uint64_t inner;
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
    register uint64_t iterator;
    register uint64_t inner;
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
        RPrintf("Warning. RCS. Substring size is 0, or, substring is NULL.\n");
    }
    return NULL;
}

method(void, removeRepetitionsOfString, RCString), const RCString *substring) {
    // if they are not equals, and size is greater than 2 substrings
    if($(object->baseString, m(compareWith, RCString)), substring) != equals
            && object->size >= substring->size * 2) {

        register uint64_t iterator;
        register uint64_t place;
        register uint64_t repetitionsCount;

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

method(RCString *, deleteCharacterAt, RCString), uint64_t index) {
    if(index > object->size) {
        RPrintf("Error. RCS. Bad index!");
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
        RPrintf("ERROR. RCS. BAD RANGE!\n");
    }
    return object;
}

method(RCString *, insertSubstringAt, RCString), RCString *substring, uint64_t place) {
    if(place < object->size) {
        char *result = RAlloc(object->size + substring->size + 1);
        RMemMove(result,                           object->baseString,         place);
        RMemMove(result + place,                   substring->baseString,      substring->size);
        RMemMove(result + place + substring->size, object->baseString + place, object->size - place);
        deallocator(object->baseString);

        object->size += substring->size;
        object->baseString = result;
    } else if(place == object->size) {
        $(object, m(concatenate, RCString)), substring);
    } else {
        RPrintf("Warning. RCS. BAD place to insert!\n");
    }

    return object;
}

method(RCString *, substringInRange, RCString), RRange range) {
    if(range.count != 0 && range.from < object->size) {
        char *cstring = RAlloc(range.count + 1 * sizeof(char));
        RMemMove(cstring, object->baseString + range.from, range.count);
        cstring[range.count + 1] = 0;

        RCString *rcString   = $(NULL, c(RCString)) );
        rcString->size       = range.count;
        rcString->baseString = cstring;

        return rcString;
    } else {
        RPrintf("ERROR. RCS. BAD RANGE!\n");
        return NULL;
    }

}

method(RCString *, substringToSymbol, RCString), char symbol) {
    register uint64_t index = indexOfFirstCharacterCString(object->baseString, object->size, symbol);
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
        object->size -= string->size + 1;
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
    register uint64_t iterator = 0;
//             RCString tempObject = *object;
             RArray   *result    =  NULL;

    if(separatorsString != NULL
            && separatorsString->size != 0
            && object != NULL
            && object->size != 0) {
        // replace all separators to ' ' - space
        while(iterator < separatorsString->size) {
            if(separatorsString->baseString[iterator] != ' ') {
                $(object, m(replaceCharacters, RCString)), separatorsString->baseString[iterator], ' ');
            }

            ++iterator;
        }
//        fixme
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
    static uint64_t iterator;
    if(checkString == NULL || object == NULL) {
        RPrintf("Warning. RCS. One of compare strings is empty, please delete function call, or fix it.\n");
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
        RPrintf("Warning. RCS. Cannot open file.\n");
    }
}

#pragma mark Concatenate

method(void, concatenate, RCString), const RCString *string) {
    if(string->size != 0 && string->baseString != NULL) {
        char *concatenationResult = RAlloc(string->size + object->size + 1);
        RMemMove(concatenationResult,                object->baseString, object->size);
        RMemMove(concatenationResult + object->size, string->baseString, string->size);
        concatenationResult[string->size + object->size + 1] = 0;

        deallocator(object->baseString);
        object->baseString = concatenationResult;
        object->size += string->size;
    } else {
        RPrintf("Warning. RCS. Bad concatenate string.\n");
    }
}

#pragma mark Conversions

method(RCString*, toUpperCase, RCString)) {
    register uint64_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 96 && object->baseString[iterator] < 123 ) {
            object->baseString[iterator] -= 32;
        }
    }
    return object;
}

method(RCString*, toLowerCase, RCString)) {
    register uint64_t iterator;
    forAll(iterator, object->size) {
        if(object->baseString[iterator] > 64 && object->baseString[iterator] < 91 ) {
            object->baseString[iterator] += 32;
        }
    }
    return object;
}
