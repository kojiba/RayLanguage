/**
 * RFloatingEnum.c
 * Realization of enum, that can change values in runtime in Ray additions.
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

#include <RFloatingEnum.h>
#include <RClassTable.h>

constructor (RFloatingEnum), pointer (*nextElementForCode)(pointer), RRange range) {
    pointer iterator;
    object = allocator(RFloatingEnum);
    if(object != nil && nextElementForCode != nil) {
        master(object, RDictionary) = makeRDictionary();

        if(master(object, RDictionary) != nil) {
            object->classId            = registerClassOnce(toString(RFloatingEnum));
            object->nextElementForCode = nextElementForCode;
            object->isChangesAfterCall = 0;

            fromStartForAll(iterator, range.start, range.size) {
                $(master(object, RDictionary), m(setObjectForKey, RDictionary)), object->nextElementForCode(iterator), iterator);
            }
        }
    }
    return object;
}

destructor(RFloatingEnum) {
    if(object != nil) {
        deleteRD(master(object, RDictionary));
    } else {
        RPrintf("ERROR. RFE. Destruct nil!\n");
    }
}

printer(RFloatingEnum) {
    size_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RFloatingEnum), object);
    RPrintf(" Count : %lu \n", master(object, RDictionary)->keys->count);
    RPrintf(" Free  : %lu \n", master(object, RDictionary)->keys->freePlaces);
    forAll(iterator, master(object, RDictionary)->keys->count) {
        RPrintf("\t %lu - {", iterator);
        RPrintf(" %lu : %lu } \n", (unsigned long) $(master(object, RDictionary)->keys, m(elementAtIndex, RArray)), iterator),
                (unsigned long) $(master(object, RDictionary)->values, m(elementAtIndex, RArray)), iterator));
    }
    RPrintf("} end of %s object %p \n\n", toString(RFloatingEnum), object);
}

method(RCompareFlags, compareValueToKey, RFloatingEnum), pointer value, pointer key) {
    RCompareFlags flag;
    pointer obj = $(master(object, RDictionary), m(getObjectForKey, RDictionary)), key);
    if(obj == value) {
        flag =  equals;
    } else {
        flag = not_equals;
    }
    if(object->isChangesAfterCall == 1) {
        $(object, m(rebase, RFloatingEnum)) );
    }

    return flag;
}

method(void, changesAfterCall,  RFloatingEnum), byte flag) {
    object->isChangesAfterCall = flag;
}

method(void, rebase, RFloatingEnum)) {
    pointer iterator;
    if(object->nextElementForCode != nil) {
        $(master(object, RDictionary)->values, m(flush, RArray)) );
        // rebase only values array
        forAll(iterator, master(object, RDictionary)->keys->count) {
            addObjectToRA(master(object, RDictionary)->values, object->nextElementForCode(iterator));
        }
    } else {
        RPrintf("ERROR. RFE. Delegate is empty");
    }
}