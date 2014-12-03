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

#include "Tests.h"
#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/REncoding/RBase64.h"

createSandBoxSingleton(StandartSandbox, 65535)

int main(int argc, const char *argv[]) {
    initPointers();
    RPool;
    RCTSingleton;

    RCString *test = $(RSC("Misha"), m(encodeBase64, RCString)));
    $(test, p(RCString)));
    RCString *test2 = $(test, m(decodeBase64, RCString)));
    $(test2, p(RCString)));

    deleter(RCTSingleton, RClassTable);
    $(RPool, p(RAutoPool)));
    deleter(RPool, RAutoPool);
    return 0;
}
