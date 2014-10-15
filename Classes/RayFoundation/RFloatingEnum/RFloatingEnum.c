#include "RFloatingEnum.h"
#include "../RClassTable/RClassTable.h"

constructor (RFloatingEnum), pointer (*nextElementForCode)(pointer), RRange range) {
    pointer iterator;
    object = allocator(RFloatingEnum);
    if(object != NULL) {
        master(object, RDictionary) = makeRDictionary();

        if(master(object, RDictionary) != NULL) {
            object->classId            = registerClassOnce(toString(RFloatingEnum));
            object->nextElementForCode = nextElementForCode;
            object->isChangesAfterCall = 0;

            fromStartForAll(iterator, range.from, range.count) {
                $(master(object, RDictionary), m(setObjectForKey, RDictionary)), object->nextElementForCode(iterator), iterator);
            }
        }
    }
    return object;
}

destructor(RFloatingEnum) {
    if(object != NULL) {
        deleteRD(master(object, RDictionary));
    } else {
        RPrintf("ERROR. RFE. Destruct null!\n");
    }
}

printer(RFloatingEnum) {
    uint64_t iterator;
    RPrintf("\n%s object %p: { \n", toString(RFloatingEnum), object);
    RPrintf(" Count : %qu \n", master(object, RDictionary)->keys->count);
    RPrintf(" Free  : %qu \n", master(object, RDictionary)->keys->freePlaces);
    forAll(iterator, master(object, RDictionary)->keys->count) {
        RPrintf("\t %qu - {", iterator);
        RPrintf(" %qu : %qu } \n", $(master(object, RDictionary)->keys, m(elementAtIndex, RArray)), iterator),
                $(master(object, RDictionary)->values, m(elementAtIndex, RArray)), iterator) );
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

method(void, rebase, RFloatingEnum)) {
    pointer iterator;
    $(master(object, RDictionary)->values, m(flush, RArray)) );
    // rebase only values array
    forAll(iterator, master(object, RDictionary)->keys->count) {
        addObjectToRA(master(object, RDictionary)->values, object->nextElementForCode(iterator));
    }
}