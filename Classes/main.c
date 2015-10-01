/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation/RayFoundation.h>
#include <RayFoundation/Utils/PurgeEvasionConnection.h>
#include <unistd.h>

#include "Tests.h"

sandBoxNamed(sandbox, 1024*1024*1)

sandBoxNamed(sandbox2, 1024*1024*2)


int main(int argc, const char *argv[]) {
    enablePool(RPool);
    enableSandBox(sandbox2());
    enableSandBox(sandbox());

    sandbox()->allocationMode = RSandBoxAllocationModeRandom;

    ComplexTest(); // lib test

    RArray *array = RA(RS("Hello"), RS("hello"), nil);

    p(RArray)(array);

    deleter(stringConstantsTable(), RDictionary);
    deleter(RCTSingleton, RClassTable);

    RPrintf("Inner sandbox\n");

    p(RSandBox)(sandbox());
    deleter(sandbox(), RSandBox);


    RPrintf("Most high lvl\n");
    p(RSandBox)(sandbox2());
    deleter(sandbox2(), RSandBox);

    p(RAutoPool)(RPool);
    deleter(RPool, RAutoPool);
    stopConsole();
}