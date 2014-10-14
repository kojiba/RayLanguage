#include "RFloatingEnum.h"
#include "../RClassTable/RClassTable.h"

constructor (RFloatingEnum), pointer (*)(pointer code), pointer enumStartPointer, pointer enumEndPointer) {
    va_list arguments;
    pointer next = 2;
    master(object, RDictionary) = makeRDictionary();
    if(master(object, RDictionary) != NULL) {
        object->classId = registerClassOnce(toString(RFloatingEnum));
        // fixme
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
    $(master(object, RDictionary), p(RDictionary)) );
}
