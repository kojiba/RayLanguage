/**
 * RString_Consts.h
 * 
 * Array for storing const RStrings on runtime.
 * Once allocated than reused. Use RS() macro.
 * 
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 04/28/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RString_Consts.h"

#ifndef RAY_EMBEDDED
    RMutex mutex;

    #define tableMutexLock() RMutexLock(&mutex);
    #define tableMutexUnlock() RMutexUnlock(&mutex);
#else
    #define tableMutexLock()
    #define tableMutexUnlock()
#endif

RDictionary* stringConstantsTable() {
    static RDictionary * instance = nil;
    if(instance == nil) {
        instance = makeRDictionary();
        if(instance != nil) {
#ifndef RAY_EMBEDDED
            mutexWithType(&mutex, RMutexNormal);
#endif
            $(instance->keys, m(setPrinterDelegate, RArray)), (PrinterDelegate) RPrintf);

            $(instance->values, m(setDestructorDelegate, RArray)), RFree); // only dealloc
        } elseError(
            RError("Can't allocate array of RStrings constants", nil)
        );
    }
    return instance;
}

const RString * constantRString(const char *string) {
    RString *resultString = nil;
    // comparator for only compile-time constants
    RCompareDelegate delegate;
    tableMutexLock();
    delegate.virtualCompareMethod = defaultComparator;
    delegate.etaloneObject = (char *)string;

    RFindResult result = $(stringConstantsTable()->keys, m(findObjectWithDelegate, RArray)), &delegate);

    // add some if not found
    if(result.object == nil) {
        RString *constant = allocator(RString);
        if(constant != nil) {
            constant->data = (byte *) string;
            constant->size = strlen(string);
            if($(stringConstantsTable()->keys, m(addObject, RArray)), (char *)string) == no_error) {
                if($(stringConstantsTable()->values, m(addObject, RArray)), constant) == no_error) {
                    tableMutexUnlock();
                    resultString = constant;
                } else {
                    $(stringConstantsTable()->keys, m(deleteLast, RArray)));
                    RError1("RString_Consts. Can't add constant \"%s\" to table.", nil, string);
                }
            } else {
                RError1("RString_Consts. Can't add key \"%s\"", stringConstantsTable(), string);
            }
        } else {
            RError1("RString_Consts. Can't allocate constant RString for string \"%s\"", nil, string);
        }
    } else {
        resultString = $(stringConstantsTable()->values, m(objectAtIndex, RArray)), result.index);
    }
    tableMutexUnlock();
    return resultString;
}

