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

constructor(RClassTable)) {
    // alloc RClassTable
    object = allocator(RClassTable);
#if RAY_SHORT_DEBUG == 1
    RPrintf("----- RCT constructor START of %p\n", object);
#endif
    if (object != nil) {

        // alloc RArray
        master(object, RArray) = makeRArray();
        if (master(object, RArray) != nil) {
            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            if(master(object, RCompareDelegate) != nil) {
                // overload delegate function
                master(object, RCompareDelegate)->virtualCompareMethod = m(compareWith, RClassNamePair);
            } else {
                RPrintf("Warning. RCT. Bad allocation on delegate.");
            }
            // we store pairs, and set destructor for pair, and printer for pair
            master(object, RArray)->destructorDelegate = d(RClassNamePair);
            master(object, RArray)->printerDelegate    = p(RClassNamePair);

            // 4 it's for self
            object->classId = 3;
            object->cacheTable = nil;
        }
    }
#if RAY_SHORT_DEBUG == 1
    RPrintf("----- RCT constructor END of %p\n", object);
#endif
    return object;
}

destructor(RClassTable) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("RCT destructor of %p\n", object);
#endif
    if (object != nil) {
        deleteRA(master(object, RArray));
        deleteRA(object->cacheTable);
        deallocator(master(object, RCompareDelegate));
    }
}

method(size_t, registerClassWithName, RClassTable), char *name) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("--- RCT Register Class with name:\"%s\" of %p\n", name, object);
#endif
    if(name != nil) {
        register size_t result = $(object, m(getIdentifierByClassName, RClassTable)), name);
        if(result == 0) {
            RClassNamePair *pair = $(nil, c(RClassNamePair)));
            if (pair != nil) {
                $(master(pair, RCString), m(setConstantString, RCString)), name);
                pair->idForClassName = master(object, RArray)->count;

                if(master(object, RArray)->count > 20) {
                    object->cacheTable = makeRArray();
                    // do not set destructor
                    object->cacheTable->printerDelegate = p(RClassNamePair);
                }

                // successfully register new class
                if ($(master(object, RArray), m(addObject, RArray)), pair) == no_error) {
                    if(object->cacheTable != nil) {
                        if(object->cacheTable->count > 20) {
                            $(object->cacheTable, m(deleteObjects, RArray)), makeRRange(0, 5));
                        }
                        $(object->cacheTable, m(addObject, RArray)), pair);
                    }
#if RAY_SHORT_DEBUG == 1
                        RPrintf("--- RCT Register Class SUCCESS on %p\n\n", object);
#endif
                    return pair->idForClassName;
                } else {
                    return 0;
                }
            } else {
                RError("RCT. Allocation of pair error", object);
                return 0;
            }
        } else {
            return result;
        }
    } else {
        RError("RCT. Register classname is nil, do nothig, please remove function call, or fix it.", object);
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
    RClassNamePair *pair = $(nil, c(RClassNamePair)));
    $(master(pair, RCString), m(setConstantString, RCString)), name);
    master(object, RCompareDelegate)->etaloneObject = pair;
    RFindResult foundedObject;
    foundedObject.object = nil;
    // search cache
    if(object->cacheTable != nil) {
        foundedObject = $(object->cacheTable, m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));
    }
    // if not found in cache
    if(foundedObject.object == nil) {
        foundedObject = $(master(object, RArray), m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));
        if(object->cacheTable != nil) {
            if(object->cacheTable->count > 20) {
                $(object->cacheTable, m(deleteObjects, RArray)), makeRRange(0, 5));
            }
            $(object->cacheTable, m(addObject, RArray)), foundedObject.object);
        }
    }
    if(foundedObject.object == nil){
        return 0;
    } else {
        return ((RClassNamePair*)foundedObject.object)->idForClassName;
    }
}

singleton(RClassTable) {
    static RClassTable *instance;
    if (instance == nil) {
#if RAY_SHORT_DEBUG == 1
        RPrintf("--------------------- RCTS FIRST_CALL ---------------------\n", instance);
#endif
        instance = $(nil, c(RClassTable)));
        // register classes on that  RClassTable was built (only our singleton)
        $(instance, m(registerClassWithName, RClassTable)), toString(RArray));
        $(instance, m(registerClassWithName, RClassTable)), toString(RCString));
        $(instance, m(registerClassWithName, RClassTable)), toString(RClassNamePair));
        $(instance, m(registerClassWithName, RClassTable)), toString(RClassTable));

#if RAY_SHORT_DEBUG == 1
        RPrintf("--------------------- RCTS FIRST_CALL ---------------------\n\n", instance);
#endif
    }
    return instance;
}