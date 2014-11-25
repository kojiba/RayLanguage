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
            RCString *temp = RS("readOnly_");
            $(result, m(concatenate, RCString)), temp);
            deallocator(temp);
            break;
        }

        case PTReadWrite: {
            RCString *temp = RS("readWrite_");
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

RayProperty* parseSourceRayProperty(RCString *code, RClassTable *delegate) {
    rbool        isTypised = no;
    size_t       iterator;
    RArray *tokens = $(code, m(substringsSeparatedBySymCStr, RCString)), "  ;\r\n\t");

    if(tokens != nil) {
        $(tokens, p(RArray)));
        RayProperty *property  = $(nil, c(RayProperty)));

        forAll(iterator, tokens->count) {
            RCString *token = $(tokens, m(elementAtIndex, RArray)), iterator);

            if(RMemCmp(token->baseString, "readOnly", token->size) == 0) {
                if(!isTypised) {
                    property->type = PTReadOnly;
                    isTypised = yes;
                    continue;
                } else {
                    RErrStr "Already have type. Token : %lu\n", iterator + 1);
                }
            }

            if(RMemCmp(token->baseString, "readWrite", token->size) == 0) {
                if (!isTypised) {
                    property->type = PTReadWrite;
                    isTypised = yes;
                    continue;
                } else {
                    RErrStr "Already have type. Token : %lu\n", iterator + 1);
                }
            }

            if(RMemCmp(token->baseString, "inner", token->size) == 0) {
                if (!isTypised) {
                    property->type = PTInner;
                    isTypised = yes;
                    continue;
                } else {
                    RErrStr "Already have type. Token : %lu\n", iterator + 1);
                }
            }
        }

        // must be type
        RCString *type = $(tokens, m(elementAtIndex, RArray)), tokens->count - 2);
        property->memSizeType = $(delegate, m(getIdentifierByClassName, RClassTable)), type->baseString);

        if(property->memSizeType == 0) {
            RErrStr "Unknown type. Token : %lu\n", tokens->count - 2);
        }

        // last must be name
        property->name = copyRCString($(tokens, m(lastObject, RArray))));
        return property;
    }
    return nil;
}

RayProperty* parseSourceCRayProperty(char *code, RClassTable *delegate) {
    RCString *source = RS(code);
    RayProperty *result = parseSourceRayProperty(source, delegate);
    deallocator(source);
    return result;
}
