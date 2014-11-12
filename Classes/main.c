/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"



int main(int argc, const char *argv[]) {
    ComplexTest();
    RCString *string = fromFileRCString(RResourcesDir"Hello.ray");
    $(string, p(RCString)));
    deleteRCS(string);
    return 0;
}
