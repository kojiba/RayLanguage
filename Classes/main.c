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
//    RayToC(RResourcesDir"Simple.ray");
//    $(RISingleton->stringConsts, p(RArray)));
//    $(RISingleton, d(RInterpreter)) );
//    deallocator(RISingleton);

    RayMethod *method = $(nil, c(RayMethod)), MTInline | MTVirtual, RSC("void"));
    method->nativeName = RSC("printA");
    $(method, m(addArgument, RayMethod)), 0, RSC("argument1"));
    $(method, m(addArgument, RayMethod)), 1, RSC("argument2"));
    RCString *cstring = $(method, m(serializetoCFunction, RayMethod)), RISingleton->typesTable);
    RCString *cstring3 = $(method, m(serializetoCPointer, RayMethod)), RISingleton->typesTable);
    if(cstring != nil) {
        $(cstring, p(RCString)));
        $(cstring3, p(RCString)));

        deleter(cstring3, RCString);
        deleter(cstring, RCString);
    }


    RayProperty *rayProperty = $(nil, c(RayProperty)));
    rayProperty->memSizeType = 1;
    rayProperty->name = RSC("someProp");

    RCString *cstring2 = $(rayProperty, m(serializeToCType, RayProperty)), RISingleton->typesTable);
    if(cstring2 != nil) {
        $(cstring2, p(RCString)));
        deleter(cstring2, RCString);
    }

    RayClass *class = $(nil, c(RayClass)) );
    class->name = RSC("Hello");
    addObjectToRA(class->methods, method);
    RCString* singleton = $(class, m(methodSingletonFor, RayClass)), RISingleton->typesTable);
    if(singleton != nil) {
        $(singleton, p(RCString)));
        deleter(singleton, RCString);
    }
    deleter(method, RayMethod);
    deleter(rayProperty, RayProperty);
    deleter(class, RayClass);
    return 0;
}
