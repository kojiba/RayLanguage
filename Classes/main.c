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

    RayMethod *method = $(nil, c(RayMethod)), MTInline | MTVirtual, RSC("void"));
    method->nativeName = RSC("printA");
    $(method, m(addArgument, RayMethod)), 0, RSC("argument1"));
    $(method, m(addArgument, RayMethod)), 1, RSC("argument2"));
    RCString *cstring = $(method, m(serializetoCFunc, RayMethod)), RISingleton->typesTable);
    if(cstring != nil) {
        $(cstring, p(RCString)));
        deleteRCS(cstring);
    }
    $(method, d(RayMethod)) );
    deallocator(method);
    return 0;
}
