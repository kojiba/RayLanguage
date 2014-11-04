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
    if (object != nullPtr) {

        // alloc RArray
        master(object, RArray) = makeRArray();
        if (master(object, RArray) != nullPtr) {
            master(object, RCompareDelegate) = allocator(RCompareDelegate);
            if(master(object, RCompareDelegate) != nullPtr) {
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
    if (object != nullPtr) {
        // destructor for RArray
        $(master(object, RArray), d(RArray)));
        deallocator(master(object, RArray));
        deallocator(master(object, RCompareDelegate));
    }
}

method(size_t, registerClassWithName, RClassTable), char *name) {
#if RAY_SHORT_DEBUG == 1
    RPrintf("--- RCT Register Class with name:\"%s\" of %p\n", name, object);
#endif
    if(name != nullPtr) {
        register size_t result = $(object, m(getIdentifierByClassName, RClassTable)), name);
        if(result == 0) {
            RClassNamePair *pair = $(nullPtr, c(RClassNamePair)));
            if (pair != nullPtr) {
                $(master(pair, RCString), m(setConstantString, RCString)), name);
                pair->idForClassName = master(object, RArray)->count;

                // successfully register new class
                if ($(master(object, RArray), m(addObject, RArray)), pair) == no_error) {
#if RAY_SHORT_DEBUG == 1
                        RPrintf("--- RCT Register Class SUCCESS on %p\n\n", object);
#endif
                    return pair->idForClassName;
                } else {
                    return 0;
                }
            // alloc error
            } else {
                return 0;
            }
        } else {
            return result;
        }
    } else {
        RWarning("RCT. Register classname is nullPtr, do nothig, please remove function call, or fix it.", object);
        return 0;
    }
}

method(size_t, getNumberOfClasses, RClassTable)) {
    return master(object, RArray)->count;
}

printer(RClassTable) {
    RPrintf("\n%s object %p: { \n", toString(RClassTable), object);
    $(master(object, RArray), p(RArray)));
    RPrintf("\t\tTOTAL: %qu classes registered \n", master(object, RArray)->count);
    RPrintf("} end of %s object %p \n\n", toString(RClassTable), object);
}

method(size_t, getIdentifierByClassName, RClassTable), char *name) {
    RClassNamePair *pair = $(nullPtr, c(RClassNamePair)));
    $(master(pair, RCString), m(setConstantString, RCString)), name);

    master(object, RCompareDelegate)->etaloneObject = pair;

    RFindResult foundedObject = $(master(object, RArray), m(findObjectWithDelegate, RArray)), master(object, RCompareDelegate));
    if(foundedObject.object == nullPtr){
        return 0;
    } else {
        return ((RClassNamePair*)foundedObject.object)->idForClassName;
    }
}

singleton(RClassTable) {
    static RClassTable *instance;
    if (instance == nullPtr) {
#if RAY_SHORT_DEBUG == 1
        RPrintf("--------------------- RCTS FIRST_CALL ---------------------\n", instance);
#endif
        instance = $(nullPtr, c(RClassTable)));
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