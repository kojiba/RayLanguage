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

void sendMsg(RTCPDataStruct *data) {
    $(data->socket, m(sendString, RSocket)), RS("Hello=)\n"));
}

pointer startServerThread (pointer some) {
    RTCPHandler  *server;
    RTCPDelegate *delegate;

    startServer(&server,
                &delegate,
                4000,
                4001);
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test
    RTCPDelegate delegate2;

    delegate2.context = nil;
    delegate2.delegateFunction = (pointer (*)(struct RTCPDataStruct *)) sendMsg;

    RTCPHandler *connector = c(RTCPHandler)(nil);
    connector->delegate = &delegate2;


    RThread serverThread = makeRThread(startServerThread);

//    RPrintf("hellosecretkey\n");
    sleep(5);
    $(connector, m(startWithHost, RTCPHandler)), RS("127.0.0.1"), 4000, 20);
    $(connector, m(waitConnectors, RTCPHandler)));
    deleter(connector, RTCPHandler);

    RThreadJoin(serverThread);


    endRay();
}