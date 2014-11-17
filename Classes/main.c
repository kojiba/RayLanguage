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

//typedef struct methodSingeton methodSingleton;
//typedef struct Some Some;
//struct Some {
//    methodSingleton *inner;
//    Some *a;
//    Some *b;
//    int e;
//};


int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();
//    RayToC(RResourcesDir"Simple.ray");
//    $(RISingleton->stringConsts, p(RArray)));
//    $(RISingleton, d(RInterpreter)) );
//    deallocator(RISingleton);

//    RayMethod *method = $(nil, c(RayMethod)), MTInline | MTVirtual, RSC("void"));
//    method->nativeName = RSC("printA");
//    $(method, m(addArgument, RayMethod)), 0, RSC("argument1"));
//    $(method, m(addArgument, RayMethod)), 1, RSC("argument2"));
//    RCString *cstring = $(method, m(serializetoCFunc, RayMethod)), RISingleton->typesTable);
//    if(cstring != nil) {
//        $(cstring, p(RCString)));
//        deleter(cstring, RCString);
//    }
//    $(method, d(RayMethod)) );
//    deallocator(method);

    RayProperty *rayProperty = $(nil, c(RayProperty)));
    rayProperty->memSizeType = 1;
    rayProperty->name = RSC("someProp");

    RCString *cstring2 = $(rayProperty, m(serializeToCType, RayProperty)), RISingleton->typesTable);
    if(cstring2 != nil) {
        $(cstring2, p(RCString)));

        (cstring2);
    }
    $(rayProperty, d(RayProperty)) );
    deallocator(rayProperty);
    return 0;
}
