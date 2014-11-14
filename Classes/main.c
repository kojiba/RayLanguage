/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RInterpreter.h"

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
    RayToC(RResourcesDir"Simple.ray");
    $(RISingleton->stringConsts, p(RArray)));
    $(RISingleton, d(RInterpreter)) );
    deallocator(RISingleton);
    return 0;
}
