/**
 * RClassTable.c
 * ClassTable for manage(get info, etc.) instances in runtime.
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

#include "RClassTable.h"

#if defined(RAY_CLASS_TABLE_THREAD_SAFE)
    #define tableMutex          &object->mutex
    #define RMutexLockTable()   RMutexLock(tableMutex)
    #define RMutexUnlockTable() RMutexUnlock(tableMutex)
#else
    // sets empty
    #define tableMutex
    #define RMutexLockTable()
    #define RMutexUnlockTable()
#endif

constructor(RClassTable)) {
    // alloc RClassTable
    object = allocator(RClassTable);
    printDebugTrace();
    if (object != nil) {

        // alloc RArray
        master(object, RArray) = makeRArray();
        if (master(object, RArray) != nil) {
            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            if(master(object, RCompareDelegate) != nil) {
                // overload delegate function
                master(object, RCompareDelegate)->virtualCompareMethod = (ComparatorDelegate) m(compareWith, RClassNamePair);
                // we store pairs, and set destructor for pair, and printer for pair
                master(object, RArray)->destructorDelegate = RClassNamePairDeleter;
                master(object, RArray)->printerDelegate    = (PrinterDelegate) p(RClassNamePair);

                // 4 it's for self
                object->classId = 3;

#if defined(RAY_CLASS_TABLE_THREAD_SAFE)
                mutexWithType(&object->mutex, RMutexNormal);
#endif
            } elseError(
                RError("RClassTable. Bad allocation on delegate.", object);
            );
        }
    }
    return object;
}

destructor(RClassTable) {
    printDebugTrace();
    deleter(master(object, RArray), RArray);
    deallocator(master(object, RCompareDelegate));
    master(object, RArray) = nil;
    master(object, RCompareDelegate) = nil;
#ifdef RAY_CLASS_TABLE_THREAD_SAFE
    RMutexDestroy(tableMutex);
#endif
}

#pragma mark Private worker

constMethod(size_t, getIdentifierByClassNameWorker, RClassTable), char *name) {
    RClassNamePair *pair = $(nil, c(RClassNamePair)));
    printDebugTrace();
    if(pair != nil) {
        $(master(pair, RCString), m(setConstantString, RCString)), name);
        master(object, RCompareDelegate)->etaloneObject = pair;
        RFindResult foundedObject = $(master(object, RArray), m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));

        // delete temp
        deleter(pair, RClassNamePair);

        if(foundedObject.object == nil){
            return 0;
        } else {
            return ((RClassNamePair*)foundedObject.object)->idForClassName;
        }

    } elseError(
            RError("RClassTable. Bad allocation of temp RClassNamePair.", object)
    );

    return 0;
}

#pragma mark Public

method(size_t, registerClassWithName, RClassTable), char *name) {
    printDebugTrace1("Name %s", name);
    if(name != nil) {
        size_t result;
        RMutexLockTable();
        result = $(object, m(getIdentifierByClassNameWorker, RClassTable)), name);
        if(result == 0) {
            RClassNamePair *pair = $(nil, c(RClassNamePair)));
            if (pair != nil) {
                $(master(pair, RCString), m(setConstantString, RCString)), name);
                pair->idForClassName = master(object, RArray)->count;

                // successfully register new class
                if ($(master(object, RArray), m(addObject, RArray)), pair) == no_error) {
                    printDebugTrace1("SUCCESS %s", name);
                    RMutexUnlockTable();
                    return pair->idForClassName;
                } else {
                    RError("RClassTable. RA add object error.", object);
                    RMutexUnlockTable();
                    return 0;
                }
            } else {
                RError("RClassTable. Allocation of pair error", object);
                RMutexUnlockTable();
                return 0;
            }
        } else {
            RMutexUnlockTable();
            return result;
        }
    } else {
        RWarning("RClassTable. Register classname is nil.", object);
        RMutexUnlockTable();
        return 0;
    }
}

method(size_t, getNumberOfClasses, RClassTable)) {
    return master(object, RArray)->count;
}

printer(RClassTable) {
    RPrintf("\n%s object %p: { \n", toString(RClassTable), object);
    $(master(object, RArray), p(RArray)));
    RPrintf("\t\tTOTAL: %lu classes registered \n", master(object, RArray)->count);
    RPrintf("} end of %s object %p \n\n", toString(RClassTable), object);
}

method(size_t, getIdentifierByClassName, RClassTable), char *name) {
    RMutexLockTable();
    size_t result = $(object, m(getIdentifierByClassNameWorker, RClassTable)), name);
    RMutexUnlockTable();
    return result;
}

method(RCString*, getClassNameByIdentifier, RClassTable), size_t id) {
    RMutexLockTable();
    if(id <= master(object, RArray)->count) {
        RClassNamePair *temp = $(master(object, RArray), m(objectAtIndex, RArray)), id);
        RMutexUnlockTable();
        return master(temp, RCString);
    } else {
        RMutexUnlockTable();
        return nil;
    }
}

singleton(RClassTable) {
    static RClassTable *instance = nil;
    if (instance == nil) {
#ifdef RAY_SHORT_DEBUG
        RPrintf("--------------------- RCTS FIRST_CALL START -------------------\n");
#endif
        instance = $(nil, c(RClassTable)));
        if(instance != nil) {
            // register classes static (only this singleton)
            $(instance, m(registerClassWithName, RClassTable)), toString(RArray));
            $(instance, m(registerClassWithName, RClassTable)), toString(RCString));
            $(instance, m(registerClassWithName, RClassTable)), toString(RClassNamePair));
            $(instance, m(registerClassWithName, RClassTable)), toString(RClassTable));
            $(instance, m(registerClassWithName, RClassTable)), toString(RSandBox));
            $(instance, m(registerClassWithName, RClassTable)), toString(RAutoPool));
            $(instance, m(registerClassWithName, RClassTable)), toString(RThread));
            $(instance, m(registerClassWithName, RClassTable)), toString(RByteArray));
        }

#ifdef RAY_SHORT_DEBUG
        RPrintf("--------------------- RCTS FIRST_CALL END ---------------------\n\n");
#endif
    }
    return instance;
}