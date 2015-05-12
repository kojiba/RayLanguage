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

#include <Utils.h>
#include <RString_Consts.h>

rbool processLine(pointer context, pointer string, size_t iterator) {
    $(string, m(trimAfterString, RCString)), RS("//")); // trim comment
    if(((RCString*)string)->size != 0) { // if not fully comment
        RArray *keyValue = $(string, m(substringsSeparatedByString, RCString)), RS(" = "));
        if(keyValue != nil) {
            RCString *key   = $(keyValue, m(elementAtIndex, RArray)), 0);
            RCString *value = $(keyValue, m(lastObject, RArray)));

            // adding
            $(context, m(setObjectForKey, RDictionary)), value, key);

            $(keyValue, m(setDestructorDelegate, RArray)), nil);
            deleter(keyValue, RArray);
        }
    }
    return yes;
}

RDictionary * stringDictionaryFromFile(char *filename) {
    RCString *locales = stringFromFile(filename);

    if(locales != nil) {
        RDictionary *result = makeRDictionary();
        if(result != nil) {
            REnumerateDelegate delegate;

            delegate.virtualEnumerator = processLine;
            delegate.context = result;
            // delegates
            $(result, m(initDelegate, RDictionary)), (ComparatorDelegate) m(compareWith, RCString));
            $(result->keys,   m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RString));
            $(result->keys,   m(setDestructorDelegate, RArray)), (DestructorDelegate) stringDeleter);
            $(result->values, m(setPrinterDelegate,    RArray)), (PrinterDelegate) p(RString));
            $(result->values, m(setDestructorDelegate, RArray)), (DestructorDelegate) stringDeleter);

            $(locales, m(removeRepetitionsOfString, RCString)), RS("\n"));                  // trim some newlines
            RArray *temp = $(locales, m(substringsSeparatedByString, RCString)), RS("\n")); // get lines

            if(temp != nil) {
                $(temp, m(enumerate, RArray)), &delegate, yes);
                deleter(temp, RArray);
            }
            deleter(locales, RCString);
        }
        return result;
    }
    return nil;
}