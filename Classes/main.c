/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RInterpreter.h"


int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    RayToC(RResourcesDir"Simple.ray");
    $(RISingleton, d(RInterpreter)) );
    deallocator(RISingleton);
    return 0;
}
