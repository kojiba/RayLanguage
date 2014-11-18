#include "RIClass.h"
#include "../RIMethods/RIMethods.h"

#define ifdel(object, property, class) if(object->property != nil) { \
                                       deleter(object->property, class) }

constructor(RayClass)) {
    object = allocator(RayClass);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RayClass));
        object->methods       = makeRArray();
        object->properties    = makeRArray();
        object->masterClasses = makeRArray();
        object->statics       = makeRArray();
    }
    return object;
}

destructor(RayClass) {
    ifdel(object, methods,       RArray);
    ifdel(object, properties,    RArray);
    ifdel(object, masterClasses, RArray);
    ifdel(object, statics,       RArray);
    ifdel(object, name,          RCString);
}

printer(RayClass) {
    RPrintf("%s - object - %p { \n", toString(RayClass), object);
    RPrintf("} %p object\n", object);
}

method(RCString*, methodSingletonName, RayClass)) {
    // copy name
    RCString *result = $(object->name, m(copy, RCString)) );
    $(result, m(appendString, RCString)), "_MethodSingleton");
    return result;
}

method(RCString*, methodSingletonFor, RayClass), RClassTable *table) {
    size_t iterator;
    RCString *result = RSC("");
    RCString *name = $(object, m(methodSingletonName, RayClass)));
    // declare struct
    $(result, m(appendString, RCString)), "typedef struct ");
    $(result, m(concatenate, RCString)), name );
    $(result, m(appendString, RCString)), " {\n");

    // add some methods-pointers
    forAll(iterator, object->methods->count) {
        RCString *methodPtr = $((RayMethod*)object->methods->array[iterator], m(serializetoCPointer, RayMethod)), table);
        $(result, m(appendString, RCString)), "    ");
        $(result, m(concatenate, RCString)),methodPtr);
        $(result, m(appendString, RCString)), ";\n");
        deleter(methodPtr, RCString);
    }

    // end declare struct
    $(result, m(appendString, RCString)), "} ");
    $(result, m(concatenate, RCString)), name );
    $(result, m(appendString, RCString)), ";");

    deleter(name, RCString);

    return result;
}

method(RCString*, classStructWithMasterClass, RayClass)) {
    return nil;
}