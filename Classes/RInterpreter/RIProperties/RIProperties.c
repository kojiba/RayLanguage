#include "RIProperties.h"

const char * PropertyTypeToString(RayPropertyType type) {
    if(type < PropertyTypeCount) {
        return propertyTypesConst[type];
    } else {
        return nil;
    }
}

RayPropertyType PropertyTypeFromString(const char* const string) {
    unsigned result = 0;
    const char *etalon = propertyTypesConst[result];
    while(result < PropertyTypeCount) {
        if(strcmp(etalon, string) == 0) {
            break;
        }
        ++result;
        etalon = propertyTypesConst[result];
    }
    return (RayPropertyType) result;
}


constructor (RayProperty)) {
    object = allocator(RayProperty);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RayProperty));
        object->type    = default_property_qualifier;
        object->name    = nil;
    }
    return object;
}

destructor(RayProperty) {
    nilDeleter(object->name, RCString);
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
    RCString *propertyString = RSC(PropertyTypeToString(object->type));
    $(propertyString, m(appendString, RCString)), property_type_prefix_postfix);

    // add name
    $(result, m(concatenate, RCString)), object->name);
    deallocator(space);
    return result;
}

RayProperty* ParsePropertyString(RCString *code, RClassTable *delegate) {
    rbool        isTypised = no;
    size_t       iterator;
    RayProperty *property = $(nil, c(RayProperty)));

    if(property != nil) {
        size_t startOfQualifier = indexOfFirstCharacterCString(code->baseString, code->size,
                                                               property_qualifier_start_symbol);

        // found start qualifier
        if(startOfQualifier < code->size) {
            size_t endOfQualifier = indexOfFirstCharacterCString(code->baseString + startOfQualifier, code->size - startOfQualifier,
                                                                 property_qualifier_end_symbol);
            // end qualifier
            if(endOfQualifier < code->size - startOfQualifier) {
                char *qualifierString = substringInRange(code->baseString + startOfQualifier, makeRRange(0, endOfQualifier));
                property->type = PropertyTypeFromString(qualifierString);
                deallocator(qualifierString);
            } else {
                deallocator(property);
                RError("ParsePropertyString. Not found end of property qualifier.", code);
                return nil;
            }
        }

        // not set qualifier constructor uses default


    } else {
        RError("ParsePropertyString. Bad property allocation.", code);
    }
    return property;
}

inline
RayProperty* parseSourceCRayProperty(char *code, RClassTable *delegate) {
    RCString *source = RS(code);
    RayProperty *result = ParsePropertyString(source, delegate);
    deallocator(source);
    return result;
}
