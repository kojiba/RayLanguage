/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"



int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    RPrintf("Pointer 1 - %p, pointer 2 - %p", "Some", "Some");
    return 0;
}
