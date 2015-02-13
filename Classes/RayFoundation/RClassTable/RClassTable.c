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

#include <RClassTable.h>

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

void pairPtrDeleter(pointer ptr) {
    deleter(ptr, RClassNamePair);
}

constructor(RClassTable)) {
    // alloc RClassTable
    object = allocator(RClassTable);
#ifdef RAY_SHORT_DEBUG
    RPrintf("----- RCT constructor START of %p\n", object);
#endif
    if (object != nil) {

        // alloc RArray
        master(object, RArray) = makeRArray();
        if (master(object, RArray) != nil) {
            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            if(master(object, RCompareDelegate) != nil) {
                // overload delegate function
                master(object, RCompareDelegate)->virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) m(compareWith, RClassNamePair);
                // we store pairs, and set destructor for pair, and printer for pair
                master(object, RArray)->destructorDelegate = pairPtrDeleter;
                master(object, RArray)->printerDelegate    = (void (*)(pointer)) p(RClassNamePair);

                // 4 it's for self
                object->classId = 3;

#if defined(RAY_CLASS_TABLE_THREAD_SAFE)
                object->mutex = mutexWithType(RMutexNormal);
#endif
            } else {
                RError("RClassTable. Bad allocation on delegate.", object);
            }
        }
    }
#ifdef RAY_SHORT_DEBUG
    RPrintf("----- RCT constructor END of %p\n", object);
#endif
    return object;
}

destructor(RClassTable) {
#ifdef RAY_SHORT_DEBUG
    RPrintf("RCT destructor of %p\n", object);
#endif
    deleter(master(object, RArray), RArray);
    deallocator(master(object, RCompareDelegate));
}

#pragma mark Private worker

method(size_t, getIdentifierByClassNameWorker, RClassTable), char *name) {
    RClassNamePair *pair = $(nil, c(RClassNamePair)));
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
    } else {
        RError("RClassTable. Bad allocation of temp RClassNamePair.", object);
    }
    return 0;
}

#pragma mark Public

method(size_t, registerClassWithName, RClassTable), char *name) {
#ifdef RAY_SHORT_DEBUG
    RPrintf("--- RCT Register Class with name:\"%s\" of %p\n", name, object);
#endif
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
#ifdef RAY_SHORT_DEBUG
                        RPrintf("--- RCT Register Class SUCCESS on %p\n\n", object);
#endif
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
        RClassNamePair *temp = $(master(object, RArray), m(elementAtIndex, RArray)), id);
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
        RPrintf("--------------------- RCTS FIRST_CALL ---------------------\n", instance);
#endif
        instance = $(nil, c(RClassTable)));
        if(instance != nil) {
            // register classes on that RClassTable was built (only our singleton)
            $(instance, m(registerClassWithName, RClassTable)), toString(RArray));
            $(instance, m(registerClassWithName, RClassTable)), toString(RCString));
            $(instance, m(registerClassWithName, RClassTable)), toString(RClassNamePair));
            $(instance, m(registerClassWithName, RClassTable)), toString(RClassTable));
            $(instance, m(registerClassWithName, RClassTable)), toString(RSandBox));
            $(instance, m(registerClassWithName, RClassTable)), toString(RAutoPool));
        }

#ifdef RAY_SHORT_DEBUG
        RPrintf("--------------------- RCTS FIRST_CALL ---------------------\n\n", instance);
#endif
    }
    return instance;
}