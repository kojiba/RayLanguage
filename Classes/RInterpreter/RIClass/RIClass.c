#include "RIClass.h"
#include "../RIMethods/RIMethods.h"
#include "../RIProperties/RIProperties.h"

constructor(RayClass)) {
    object = allocator(RayClass);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RayClass));
        object->methods       = makeRArray();

        object->properties    = makeRArray();
        if(object->properties != nil) {
            object->properties->destructorDelegate = (void (*)(pointer)) d(RayProperty);
            object->properties->printerDelegate    = (void (*)(pointer)) p(RayProperty);
        }

        object->masterClasses = makeRArray();
        if(object->masterClasses != nil) {
            object->properties->destructorDelegate = (void (*)(pointer)) d(RayClass);
            object->properties->printerDelegate    = (void (*)(pointer)) p(RayClass);
        }

        object->statics       = makeRArray();

    }
    return object;
}

destructor(RayClass) {
    nilDeleter(object->methods,       RArray);
    nilDeleter(object->properties,    RArray);
    nilDeleter(object->masterClasses, RArray);
    nilDeleter(object->statics,       RArray);
    nilDeleter(object->name,          RString);
}

printer(RayClass) {
    RPrintf("%s - object - %p { \n", toString(RayClass), object);
    RPrintf("} %p object\n", object);
}

#pragma mark Workers

method(RString*, methodSingletonName, RayClass)) {
    // copy name
    RString *result = $(object->name, m(copy, RString)) );
    $(result, m(appendString, RString)), "_MethodSingleton");
    return result;
}

method(RString*, methodSingletonFor, RayClass), RClassTable *table) {
    size_t iterator;
    size_t masterClassIterator;
    RString *result = RSC("");
    RString *name = $(object, m(methodSingletonName, RayClass)));
    // declare struct
    $(result, m(appendString, RString)), "typedef struct ");
    $(result, m(concatenate, RString)), name );
    $(result, m(appendString, RString)), " {\n");

    // add some methods-pointers start master-classes
    forAll(masterClassIterator, object->masterClasses->count){
        RayClass *masterClass = object->masterClasses->array[masterClassIterator];
        forAll(iterator, masterClass->methods->count) {
//fixme
//            // if method is not inner
//            if(!(((RayMethod*)masterClass->methods->array[iterator])->methodType & MTInner)) {
//
//                // serialize it
//                RString *methodPtr = $((RayMethod*)masterClass->methods->array[iterator], m(serializetoCPointer, RayMethod)), table);
//                $(result, m(appendString, RString)), "    ");
//                $(result, m(concatenate, RString)),methodPtr);
//                $(result, m(appendString, RString)), ";\n");
//                deleter(methodPtr, RString);
//            }
        }
    }
//fixme
//    // add some methods-pointers
//    forAll(iterator, object->methods->count) {
//        RString *methodPtr = $((RayMethod*)object->methods->array[iterator], m(serializetoCPointer, RayMethod)), table);
//        $(result, m(appendString, RString)), "    ");
//        $(result, m(concatenate, RString)),methodPtr);
//        $(result, m(appendString, RString)), ";\n");
//        deleter(methodPtr, RString);
//    }

    // end declare struct
    $(result, m(appendString, RString)), "} ");
    $(result, m(concatenate, RString)), name );
    $(result, m(appendString, RString)), ";");

    deleter(name, RString);

    return result;
}

method(RString*, classStructWithMasterClass, RayClass), RClassTable *table) {
    size_t iterator;
    size_t masterClassIterator;
    RString *result = RSC("");
    RString *singletonName = $(object, m(methodSingletonName, RayClass)));

    // declare struct
    $(result, m(appendString, RString)), "typedef struct ");
    $(result, m(concatenate, RString)), object->name );
    $(result, m(appendString, RString)), " {\n");

    // add method singleton property
    $(result, m(appendString, RString)), "    ");
    $(result, m(concatenate, RString)), singletonName);
    $(result, m(appendString, RString)), " *methodSingleton;\n");

    // add masterClass properties
    forAll(masterClassIterator, object->masterClasses->count) {
        // get masterClass
        RayClass *masterClass = object->masterClasses->array[masterClassIterator];
        forAll(iterator, masterClass->properties->count) {

            // if property is not inner
            if(((RayProperty*)masterClass->properties->array[iterator])->type != Inner) {
                RString *property = $((RayProperty*)masterClass->properties->array[iterator], m(serializeToCType, RayProperty)), table);

                // serialize it
                $(result, m(appendString, RString)), "    ");
                $(result, m(concatenate, RString)), property);
                $(result, m(appendString, RString)), ";\n");
                deleter(property, RString);
            }
        }
    }

    // add self properties
    forAll(iterator, object->properties->count) {
        RString *property = $((RayProperty*)object->properties->array[iterator], m(serializeToCType, RayProperty)), table);
        $(result, m(appendString, RString)), "    ");
        $(result, m(concatenate, RString)), property);
        $(result, m(appendString, RString)), ";\n");
        deleter(property, RString);
    }

    // end declare struct
    $(result, m(appendString, RString)), "} ");
    $(result, m(concatenate, RString)), object->name );
    $(result, m(appendString, RString)), ";");
    return result;
}