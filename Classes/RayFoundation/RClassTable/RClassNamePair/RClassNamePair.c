#include <stdio.h>
#include "RClassNamePair.h"

constructor(RClassNamePair)) {
    object = allocator(RClassNamePair);

    if (object != NULL) {

        // 1 - it's for RClassNamePair
        object->classId = 1;
    }

    return object;
}

destructor(RClassNamePair) {
}

printer(RClassNamePair) {
    printf("%p , { %qu - %s }\n", object, object->idForClassName, object->className);
}
