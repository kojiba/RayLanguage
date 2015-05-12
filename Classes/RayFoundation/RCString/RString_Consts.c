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

#include <RString_Consts.h>

RMutex mutex;

RDictionary* stringConstantsTable() {
    static RDictionary * instance = nil;
    if(instance == nil) {
        instance = makeRDictionary();
        if(instance != nil) {
            mutexWithType(&mutex, RMutexNormal);
            $(instance->keys, m(setPrinterDelegate, RArray)), (PrinterDelegate) RPrintf);

            $(instance->values, m(setDestructorDelegate, RArray)), RFree); // only dealloc
        } elseError(
            RError("Can't allocate array of RStrings constants", nil)
        );
    }
    return instance;
}

RCString * constantRString(char *string) {
    // comparator for only compile-time constants
    RCompareDelegate delegate;
    RMutexLock(&mutex);
    delegate.virtualCompareMethod = defaultComparator;
    delegate.etaloneObject = string;

    RFindResult result = $(stringConstantsTable()->keys, m(findObjectWithDelegate, RArray)), &delegate);

    // add some if not found
    if(result.object == nil) {
        RCString *constant = makeRCString();
        if(constant != nil) {
            $(constant, m(setConstantString, RCString)), string);
            if($(stringConstantsTable()->keys, m(addObject, RArray)), string) == no_error) {
                if($(stringConstantsTable()->values, m(addObject, RArray)), constant) == no_error) {
                    RMutexUnlock(&mutex);
                    return constant;
                } else {
                    $(stringConstantsTable()->keys, m(deleteLast, RArray)));
                    RError1("RString_Consts. Can't add constant \"%s\" to table.", nil, string);
                    RMutexUnlock(&mutex);
                    return nil;
                }
            } else {
                RError1("RString_Consts. Can't add key \"%s\"", stringConstantsTable(), string);
                RMutexUnlock(&mutex);
                return nil;
            }
        } else {
            RError1("RString_Consts. Can't allocate constant RString for string \"%s\"", nil, string);
            RMutexUnlock(&mutex);
            return nil;
        }
    } else {
        RMutexUnlock(&mutex);
        return  $(stringConstantsTable()->values, m(elementAtIndex, RArray)), result.index);
    }
}

