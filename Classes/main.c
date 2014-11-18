/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RInterpreter.h"
#include "RInterpreter/RIMethods/RIMethods.h"
#include "RInterpreter/RIProperties/RIProperties.h"
#include "RInterpreter/RIClass/RIClass.h"

//typedef struct Hello_MethodSingeton Hello_MethodSingeton;
//struct Hello_MethodSingeton {
//    void (*printA)(void);
//    void (*printB)(void);
//};
//
//typedef struct Hello {
//    Hello_MethodSingeton singleton;
//    int readOnly_a;
//    int readOnly_b;
//} Hello;





int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();
    RayToC(RResourcesDir"Simple.ray");
    $(RISingleton->stringConsts, p(RArray)));
    $(RISingleton->typesTable, p(RClassTable)));

    $(RISingleton, d(RInterpreter)) );
    deallocator(RISingleton);

//    RayMethod *method = $(nil, c(RayMethod)), MTInline | MTVirtual, RSC("void"));
//    method->nativeName = RSC("printA");
//    $(method, m(addArgument, RayMethod)), 1, RSC("argument1"));
//    $(method, m(addArgument, RayMethod)), 2, RSC("argument2"));
//    RCString *cstring = $(method, m(serializetoCFunction, RayMethod)), RISingleton->typesTable);
//    RCString *cstring3 = $(method, m(serializetoCPointer, RayMethod)), RISingleton->typesTable);
//    if(cstring != nil) {
//        $(cstring, p(RCString)));
//        $(cstring3, p(RCString)));
//
//        deleter(cstring3, RCString);
//        deleter(cstring, RCString);
//    }
//
//    RayMethod *method2 = $(nil, c(RayMethod)), MTVirtual, RSC("void"));
//    method2->nativeName = RSC("printB");
//    $(method2, m(addArgument, RayMethod)), 1, RSC("self"));
//
//
//    RayProperty *rayProperty = $(nil, c(RayProperty)));
//    rayProperty->memSizeType = 1;
//    rayProperty->name = RSC("someProp");
//
//    RayProperty *rayProperty2 = $(nil, c(RayProperty)));
//    rayProperty2->memSizeType = 2;
//    rayProperty2->name = RSC("someProp");
//    rayProperty2->type = PTInner;
//
//    RCString *cstring2 = $(rayProperty, m(serializeToCType, RayProperty)), RISingleton->typesTable);
//    if(cstring2 != nil) {
//        $(cstring2, p(RCString)));
//        deleter(cstring2, RCString);
//    }
//
//    RayClass *class = $(nil, c(RayClass)) );
//    class->name = RSC("Hello");
//    addObjectToRA(class->methods, method);
//    addObjectToRA(class->methods, method2);
//
//    addObjectToRA(class->properties, rayProperty);
//    addObjectToRA(class->properties, rayProperty2);
//
//    RCString* singleton = $(class, m(methodSingletonFor, RayClass)), RISingleton->typesTable);
//    if(singleton != nil) {
//        $(singleton, p(RCString)));
//        deleter(singleton, RCString);
//    }
//
//    RCString* classStruct = $(class, m(classStructWithMasterClass, RayClass)), RISingleton->typesTable);
//    if(classStruct != nil) {
//        $(classStruct, p(RCString)));
//        deleter(classStruct, RCString);
//    }
//
//    deleter(method, RayMethod);
//    deleter(method2, RayMethod);
//    deleter(rayProperty, RayProperty);
//    deleter(rayProperty2, RayProperty);
//    deleter(class, RayClass);
    return 0;
}
