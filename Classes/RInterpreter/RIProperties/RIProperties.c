#include "RIProperties.h"

const char * PropertyTypeToString(RayPropertyType type) {
    if(type < PropertyTypeCount) {
        return propertyTypesConsts[type];
    } else {
        return nil;
    }
}

RayPropertyType PropertyTypeFromString(const char* const string) {
    unsigned result = 0;
    const char *etalon = propertyTypesConsts[result];
    while(result < PropertyTypeCount) {
        if(strcmp(etalon, string) == 0) {
            break;
        }
        ++result;
        etalon = propertyTypesConsts[result];
    }
    return (RayPropertyType) result;
}

constructor (RayProperty)) {
    object = allocator(RayProperty);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RayProperty));
        object->type = default_property_qualifier;
        object->names = nil;
    }
    return object;
}

destructor(RayProperty) {
    nilDeleter(object->names, RArray);
}

printer(RayProperty) {
    RPrintf("%s - object - %p {\n", toString(RayProperty), object);
    RPrintf("\t Names : \n");
    $(object->names, p(RArray)));
    RPrintf("\t MemType : %lu",  object->memSizeType);
    RPrintf("\t Property qualifier : %s",  PropertyTypeToString(object->type));
    RPrintf("} - %p\n", object);
}

static RCString *stored = nil;
static RCString *qualifierString = nil;
static size_t   totalCount = 0;

rbool nameEnumerator(pointer object, size_t iterator) {
    RCString *tempName = $(qualifierString, m(copy, RCString)));
    $(tempName, m(concatenate, RCString)), (RCString*)object);
    $(stored, m(concatenate, RCString)), tempName);

    if(iterator != totalCount) {
        $(stored, m(appendString, RCString)), ", ");
    } else {
        $(stored, m(append, RCString)), ';');
    }

    deleter(tempName, RCString);
    return yes;
};

method(RCString *, serializeToCType, RayProperty), RClassTable *delegate) {
    RCString *result = RSC("");
    if(result != nil) {
        REnumerateDelegate enumerator;
        enumerator.virtualCheckObject = nameEnumerator;

        // add type declaration
        RCString *type = $(delegate, m(getClassNameByIdentifier, RClassTable)), object->memSizeType);
        $(result, m(concatenate, RCString)), type);

        // add space
        $(result, m(append, RCString)), ' ');

        // add prefix to names
        RCString *propertyString = RSC(PropertyTypeToString(object->type));
        $(propertyString, m(appendString, RCString)), property_type_prefix_postfix);

        qualifierString = propertyString;

        // add names
        stored = result;
        totalCount = object->names->count - 1;
        $(object->names, m(enumerate, RArray)), &enumerator, yes);

        stored = nil;
        qualifierString = nil;
        totalCount = 0;

        // cleanup
        deleter(propertyString, RCString);
    }
    return result;
}

RayProperty* ParsePropertyString(RCString *code, RClassTable *delegate) {
    RayProperty *property = $(nil, c(RayProperty)));
    RCString    *errorString = nil;

    if(property != nil) {
        char *codeIterator = code->baseString;
        size_t codeSize = code->size;
        size_t tokenEnd;
        size_t tokenStart = indexOfFirstCharacterCString(codeIterator, codeSize,
                                                               property_qualifier_start_symbol);

        // found start qualifier
        if(tokenStart < code->size) {
            codeSize -= tokenStart + 1;
            codeIterator += tokenStart + 1;
            tokenEnd = indexOfFirstCharacterCString(codeIterator, codeSize,
                                                                 property_qualifier_end_symbol);

            // end qualifier
            if(tokenEnd < codeSize) {
                char *qualifierString = substringInRange(codeIterator, makeRRange(0, tokenEnd));
                property->type = PropertyTypeFromString(qualifierString);
                deallocator(qualifierString);
                codeIterator += tokenEnd + 1;
                codeSize     -= tokenEnd + 1;

                // one for separator property_type_separator symbol if its not there - :(
                --codeSize;
                ++codeIterator;

            } else {
                errorString = RSC("Not found property_qualifier_end_symbol in code.");
                goto error;
            }
        }

        // not set qualifier constructor uses default
        // search type
        tokenEnd = indexOfFirstCharacterCString(codeIterator, codeSize, property_type_separator);
        if(tokenEnd < codeSize) {
            char * memSizeType = substringInRange(codeIterator, makeRRange(0, tokenEnd));
            size_t memoryType = $(delegate, m(getIdentifierByClassName, RClassTable)), memSizeType);
            deallocator(memSizeType);

            if(memoryType == 0) {
                errorString = RSC("Unknown memory type qualifier.");
                goto error;
            }
            property->memSizeType = memoryType;

            codeIterator += tokenEnd + 1;
            codeSize     -= tokenEnd + 1;
        } else {
            errorString = RSC("Not found property_type_serparetor in code.");
            goto error;
        }

        // next must be names separated
        RCString *tempCode = RCStringInit(codeIterator, codeSize);
        RCString *tempSeparator = RString(property_names_separator_string);

        // parse names
        RArray *names = $(tempCode, m(substringsSeparatedByString, RCString)), tempSeparator);

        if(names != nil) {
            property->names = names;

        // names not separated
        } else {
            property->names = makeRArrayOptions(1, 2, nil);
            if(property->names != nil) {
                $(property->names, m(addObject, RArray)), $(tempCode, m(copy, RCString))));
            } else {
                errorString = RSC("Bad property names array.");
                goto error;
            }

        }

        deallocator(tempCode);
        deallocator(tempSeparator);
    } else {
        errorString = RSC("Bad property allocation.");
        goto error;
    }

error:
    if(errorString != nil) {
        deallocator(property);
        RErrStr "Error. ParsePropertyString. %s\n", errorString->baseString);
        deleter(errorString, RCString);
        property = nil;
    }

    return property;
}

inline
RayProperty* ParsePropertyCString(char *code, RClassTable *delegate) {
    RCString *source = RString(code);
    RayProperty *result = ParsePropertyString(source, delegate);
    deallocator(source);
    return result;
}
