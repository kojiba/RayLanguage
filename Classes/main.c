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

createSandBoxSingleton(someSandBox, 65535)

int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    size_t iterator;
    initPointers();
    switchToSandBox(someSandBox());
    forAll(iterator, 20) {
        RCString *temp = randomRCString();
    }

    $(someSandBox(), p(RSandBox)));
    deleter(someSandBox(), RSandBox); // automaticaly disables

    return 0;
}
