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
    RArray *hello = $(RS("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
            "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit "
            "in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
            "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."), substringsSeparatedBySymCStrRCString), " ,.-+/=;:!?\n\t\r");
    if(hello != nil) {
        $(hello, p(RArray)));
        $(hello, m(flush, RArray)));
        $(hello, p(RArray)));
        deleter(hello, RArray);
    }

    return 0;
}
