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

int main(int argc, const char *argv[]) {
    initPointers();
    ComplexTest();
    initPointers();
    initRClock();
    RCString *text = RCStringFromFile(argv[1]);

    RPrintLn("Time for reading file :");
    tickRClock();
    RArray *hello = $(text, substringsSeparatedBySymCStrRCString), " ,.-+/=;:!?\n\t\r\'\"\0");

    RPrintLn("Time for separating buffer:");
    tickRClock();

    if(hello != nil) {
        deleter(hello, RArray);
    }

    if(text != nil) {
        deleter(text, RCString);
    }
    RPrintLn("Time for cleanup:");
    tickRClock();

    return 0;
}
