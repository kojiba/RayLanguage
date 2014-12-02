/**
 * _____
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

#include <time.h>
#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RInterpreter/RInterpreter.h"
#include "RayFoundation/RSystem.h"
#include "RInterpreter/RIProperties/RIProperties.h"
#include "RayFoundation/RMemoryOperations/RAutoPool.h"

createSandBoxSingleton(StandartSandbox, 65535)

int main(int argc, const char *argv[]) {
    initPointers();
    enablePool(RPool);

    StandartSandbox()->allocationMode = RSandBoxAllocationModeStandart;


    RDynamicArrayTest();
    $(StandartSandbox(), p(RSandBox)));
    deleter(StandartSandbox(), RSandBox);


    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}
