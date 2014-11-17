#include "RIProperties.h"

constructor (RayProperty)) {
    object = allocator(RayProperty);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RayProperty));
        object->type = PTReadOnly;
    }
    return object;
}

destructor(RayProperty) {
    if(object->name != nil) {
        deallocator(object->name);
    }
}

printer(RayProperty) {
    RPrintf("%s - object - %p {\n", toString(RayProperty), object);
    RPrintf("\t Name    : %s\n", object->name->baseString);
    RPrintf("\t MemType : %lu",  object->memSizeType);
    RPrintf("} - %p\n", object);
}

method(RCString *, serializeToCType, RayProperty), RClassTable *delegate) {
    RCString *result = RSC("");
    RCString *space = RS(" ");

    // add type declaration
    RCString *type = $(delegate, m(getClassNameByIdentifier, RClassTable)), object->memSizeType);
    $(result, m(concatenate, RCString)), type);

    // add space
    $(result, m(concatenate, RCString)), space);

    // add prefix to name
    switch (object->type) {
        case PTReadOnly: {
            RCString *temp = RS("readonly_");
            $(result, m(concatenate, RCString)), temp);
            deallocator(temp);
            break;
        }

        case PTReadWrite: {
            RCString *temp = RS("readwrite_");
            $(result, m(concatenate, RCString)), temp);
            deallocator(temp);
            break;
        }

        case PTInner: {
            RCString *temp = RS("inner_");
            $(result, m(concatenate, RCString)), temp);
            deallocator(temp);
            break;
        }

    }

    // add name
    $(result, m(concatenate, RCString)), object->name);
    deallocator(space);
    return result;
}