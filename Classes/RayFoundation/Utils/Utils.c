/**
 * Utils.c
 * Some useful applicable of libRay.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 05/05/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "Utils.h"
#include "RayFoundation/RString/RString_Consts.h"
#include "RayFoundation/RString/RString_File.h"

rbool processLine(pointer context, pointer string, size_t iterator) {
    $(string, m(trimAfterString, RString)), RS("//")); // trim comment
    if(((RString*)string)->size != 0) { // if not fully comment
        RArray *keyValue = $(string, m(separatedByStringWithShield, RString)), RS(" = "), RS("\\"));
        if(keyValue != nil) {
            RString *key   = $(keyValue, m(objectAtIndex, RArray)), 0);
            RString *value = $(keyValue, m(lastObject, RArray)));

            // adding
            $(context, m(setObjectForKey, RDictionary)), value, key);

            $(keyValue, m(setDestructorDelegate, RArray)), nil);
            deleter(keyValue, RArray);
        }
    }
    return yes;
}

RDictionary * stringDictionaryFromFile(char *filename) {
    RString *locales = stringFromFile(filename);

    if(locales != nil) {
        RDictionary *result = makeRDictionary();
        if(result != nil) {
            REnumerateDelegate delegate;

            delegate.virtualEnumerator = processLine;
            delegate.context = result;
            // delegates
            $(result, m(initDelegate, RDictionary)), (ComparatorDelegate) m(compareWith, RString));
            $(result->keys,   m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RData));
            $(result->keys,   m(setDestructorDelegate, RArray)), (DestructorDelegate) RDataDeleter);
            $(result->values, m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RData));
            $(result->values, m(setDestructorDelegate, RArray)), (DestructorDelegate) RDataDeleter);

            $(locales, m(removeRepetitionsOfString, RString)), RS("\n"));                  // trim some newlines
            RArray *temp = $(locales, m(substringsSeparatedByString, RString)), RS("\n")); // get lines

            if(temp != nil) {
                $(temp, m(enumerate, RArray)), &delegate, yes);
                deleter(temp, RArray);
            }
            deleter(locales, RString);
        }
        return result;
    }
    return nil;
}

constMethod(RBuffer *, serializeToBufferDelegate, RArray), REnumerateDelegate *delegate) {
    size_t          iterator = 0,
                    currentSize;
    pointer         prtToSized;
    SerializerData *nextComplexPart;

    RBuffer *result = $(nil, c(RBuffer)));
    if(result != nil) {
        forAll(iterator, object->count) {

            if(delegate->virtualEnumerator(delegate, object->array[iterator], iterator) ) {
                nextComplexPart = delegate->context;

                while(nextComplexPart != nil) {
                    currentSize     = nextComplexPart->size;
                    prtToSized      = nextComplexPart->serializePtrStart;

                    if(currentSize != 0 && prtToSized != nil) {
                        $(result, m(addBytes, RBuffer)), prtToSized, currentSize);
                    }
                    nextComplexPart = ((SerializerData *)delegate->context)->next;
                }
            } else {
                break;
            }
        }
        return $(result, m(sizeToFit, RBuffer)));
    }
    return nil;
}