/**
 * RDictionary.c
 * Realization of C dictionary(map) pair, in Ray additions.
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

#include <RDictionary.h>
#include <RClassTable.h>
#include <stdarg.h>

constructor(RDictionary)) {
    object = allocator(RDictionary);
    if(object != nil) {
        // set up class ID
        object->classId = registerClassOnce(toString(RDictionary));
        object->keys    = makeRArray();
        object->values  = makeRArray();

        if(object->keys != nil
                && object->values != nil) {
            object->values->destructorDelegate = nil;
            object->keys->destructorDelegate   = nil;
            object->values->printerDelegate    = nil;
            object->keys->printerDelegate      = nil;
            object->comparator                 = nil;

        } elseError(
                RError("RDictionary. Allocation keys or values error.", nil)
        );
    }
    return object;
}

destructor(RDictionary) {
    deleter(object->keys, RArray);
    deleter(object->values, RArray);
}

method(void, initDelegate, RDictionary), ComparatorDelegate comparator) {
    object->comparator = comparator;
}

method(void, setObjectForKey, RDictionary), pointer value, pointer key) {
    RCompareDelegate delegate;
    delegate.etaloneObject = key;
    delegate.virtualCompareMethod = object->comparator;

    RFindResult findResult = $(object->keys, m(findObjectWithDelegate, RArray)), &delegate);

    // if object for key not exist
    if(findResult.object == nil){
        $(object->keys,   m(addObject, RArray)), key);    // adding
        $(object->values, m(addObject, RArray)), value);  // adding

    // if key exist
    } else {
        $(object->values, m(setObjectAtIndex, RArray)), value, findResult.index);
    }
}

method(void, sizeToFit, RDictionary)) {
    $(object->keys,   m(sizeToFit, RArray)));
    $(object->values, m(sizeToFit, RArray)));
}

constMethod(pointer, getObjectForKey, RDictionary), pointer key) {
    RCompareDelegate delegate;
    delegate.etaloneObject = key;
    delegate.virtualCompareMethod = object->comparator;

    RFindResult findResult= $(object->keys, m(findObjectWithDelegate, RArray)), &delegate);

    if(findResult.object != nil) {
        return $(object->values, m(elementAtIndex, RArray)), findResult.index);
    } else {
        return nil;
    }
}

printer(RDictionary){
    size_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RDictionary), object);
    RPrintf(" Count : %lu \n", object->keys->count);
    RPrintf(" Free  : %lu \n", object->keys->freePlaces);
    forAll(iterator, object->keys->count) {
        RPrintf("\t %lu - {", iterator);
        RPrintf(" %p : %p } \n", $(object->keys, m(elementAtIndex, RArray)), iterator),
                                 $(object->values, m(elementAtIndex, RArray)), iterator) );
    }
    RPrintf("} end of %s object %p \n\n", toString(RDictionary), object);
}

#pragma mark Init from scratch

RDictionary* dictionaryFromPairs(pointer firstKey, ...) {
    RDictionary *result = makeRDictionary();
#ifdef RAY_ERRORS_ON
    size_t keysCount   = 1,
           valuesCount = 0;
#endif
    pointer temp;
    va_list args;

    if(result != nil) {
        register rbool isKey = no;
        $(result->keys, m(addObject, RArray)), firstKey);

        va_start(args, firstKey);
        temp = va_arg(args, pointer);

        while(temp != nil) {
            if(isKey) {
                $(result->keys, m(addObject, RArray)), temp);
                isKey = no;
#ifdef RAY_ERRORS_ON
                ++keysCount;
#endif
            } else {
                $(result->values, m(addObject, RArray)), temp);
                isKey = yes;
#ifdef RAY_ERRORS_ON
                ++valuesCount;
#endif
            }

            temp = va_arg(args, pointer);
        }
        ifError(keysCount != valuesCount,
                RError2("dictionaryFromPairs. Bad keys and values count (%lu, %lu).", nil, keysCount, valuesCount)
        );

        va_end(args);
        $(result, m(sizeToFit, RDictionary)));
    }

    return result;
}