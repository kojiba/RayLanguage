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
#include "RayFoundation/RMemoryOperations/RAutoPool.h"

autoPoolNamed(tempPool);

int main(int argc, const char *argv[]) {
    size_t iterator;
    initPointers();
    enablePool(tempPool());
    forAll(iterator, 20) {
        RCString *temp = randomRCString();
    }
    $(tempPool(), p(RAutoPool)));
    deleter(tempPool(), RAutoPool);

    return 0;
}
