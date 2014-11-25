/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RInterpreter/RInterpreter.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RIProperties/RIProperties.h"

int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();

    return 0;
}
