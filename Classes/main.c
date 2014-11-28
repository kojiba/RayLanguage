/**
 * main.c
 * Ray additions test.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include <time.h>
#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RInterpreter/RInterpreter.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RIProperties/RIProperties.h"

createSandBoxSingleton(someSandBox)

int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();

    switchToSandBox(someSandBox());
    RCString *some  = RSC("Some string");
    RCString *hello  = RSC("hello");
    RCString *mor = RSC("mor");
//    deleter(some, RCString);
    $(someSandBox(), p(RSandBox)));

    deleter(someSandBox(), RSandBox); // automaticaly disables
    return 0;
}
