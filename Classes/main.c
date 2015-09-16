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
#include <signal.h>
#include "Tests.h"
#include "RayChat.h"

void sendMsg(RTCPDataStruct *data) {
    RString *string = stringWithFormat("Hello from bot %llu\n", data->identifier);
    $(data->socket, m(sendString, RSocket)), string);
    deleter(string, RString);
}

pointer startServerThread (pointer some) {
    RTCPHandler  *server;
    RTCPDelegate *delegate;
//    RPrintf("Please, input server secretkey to admin on %u port\n", 4000);
//    RString *password = getInputString();
//
//    while(password->size <= 12) {
//        RPrintf("Please, reenter a secretkey at least 12 bytes\n");
//        password = getInputString();
//    }
    signal(SIGPIPE, SIG_IGN);

    startServer(&server,
                &delegate,
                4000,
                4001, RS("hellosecretkey"));
}

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest(); // lib test

    signal(SIGPIPE, SIG_IGN);

    RTCPDelegate delegate2;

    delegate2.context = nil;
    delegate2.delegateFunction = (pointer (*)(struct RTCPDataStruct *)) sendMsg;

    RTCPHandler *connector = c(RTCPHandler)(nil);
    connector->delegate = &delegate2;


    RThread serverThread = makeRThread(startServerThread);
    sleep(1);
    $(connector, m(startWithHost, RTCPHandler)), RS("127.0.0.1"), 4000, 200);
    $(connector, m(waitConnectors, RTCPHandler)));
    deleter(connector, RTCPHandler);

    RThreadJoin(serverThread);

    endSockets();

    endRay();
}