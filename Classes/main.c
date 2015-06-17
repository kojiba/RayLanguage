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

#include <RayFoundation.h>
#include <unistd.h>

#include <RTCPHandler.h>
#include "Tests.h"

pointer exec(RTCPDataStruct *data) {
    RPrintLn("Some connected");
    char buffer[1000];

    $(data->socket, m(sendString, RSocket)), RS("Hello world!\n"));

    byte resultFlag =  $(data->socket, m(receive, RSocket)), buffer, 1000);

    while(resultFlag != networkConnectionClosedConst) {
        RPrintf("%s", buffer);
        resultFlag =  $(data->socket, m(receive, RSocket)), buffer, 1000);
    }

    deleter(data->socket, RSocket);
    deallocator(data);
    return nil;
}


int main(int argc, const char *argv[]) {
    size_t iterator;
    initRClock();
    enablePool(RPool);
    ComplexTest();

    RTCPHandler *server = c(RTCPHandler)(nil);
    $(server->listener, m(bindPort, RSocket)), 7777);
    $(server, m(set_delegate, RTCPHandler)), (RThreadFunction) exec);
    $(server, m(start, RTCPHandler)), server);

    tickRClock();
    endRay();
}
