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
#include <unistd.h>
#include "Tests.h"
#include "RayChat.h"

void exec() {
    RSocket *socket = socketConnectedTo("216.58.209.206", 80);
    RPrintf("Try connect\n");
    nilDeleter(socket, RSocket);
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test


    size_t iterator;
    RThreadPool *pool = c(RThreadPool)(nil);
    $(pool, m(setDelegateFunction, RThreadPool)), (RThreadFunction) exec);

    forAll(iterator, 1000) {
        $(pool, m(addWithArg, RThreadPool)), nil, yes);
    }


    $(pool, m(joinSelfDeletes, RThreadPool)));

    deleter(pool, RThreadPool);


    endRay();
}