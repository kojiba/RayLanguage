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

#include "Tests.h"

#define BUFFER_SIZE 1500

pointer exec(RTCPDataStruct *data) {
    char    buffer[BUFFER_SIZE];
    const char    *address = addressToString(&data->socket->address);
    ushort            port = ntohs(data->socket->address.sin_port);
    unsigned currentThread =  (unsigned int) currentThreadIdentifier();
    byte     resultFlag;
    size_t   receivedSize;

    RPrintf("%p address\n", address);
    RPrintf("[I] %s:%u connected [tuid : %u]\n", address, port, currentThread);

    $(data->socket, m(sendString, RSocket)), RS("Hello from RServer.\n"));

    resultFlag = $(data->socket, m(receive, RSocket)), buffer, 1000, &receivedSize);
    while(resultFlag != networkConnectionClosedConst) {
        buffer[receivedSize] = 0;
        RPrintf("%s:%u > %s", address, port, buffer);
        $(data->socket, m(send, RSocket)), buffer, receivedSize);
        resultFlag =  $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
    }

    RPrintf("[I] %s:%u disconnected [tuid : %u]\n", address, port, currentThread);

    deleter(data->socket, RSocket);
    data->socket = nil; // for auto-cleaning
    return nil;
}

RTCPHandler *server = nil;

void serverFunc(void) {
    server = c(RTCPHandler)(nil);
    if(server != nil) {
        RTCPDelegate delegate;
        delegate.delegateFunction = (RThreadFunction) exec;
        delegate.context = &delegate;
        $(server,  m(set_delegate, RTCPHandler)), &delegate);
        RPrintf("RTCPHandler starting %p in %u thread\n", server, (unsigned int) currentThreadIdentifier());
        $(server,  m(startOnPort, RTCPHandler)), 4000, nil); // blocks thread until m(terminate, RTCPHandler)
    }
}

void startServer(void) {
    RThread thread;
    RThreadCreate(&thread, nil, (RThreadFunction) serverFunc, nil);
}

int main(int argc, const char *argv[]) {
    rbool closeAll = no;
    byte connectionState;
    char buffer[BUFFER_SIZE];
    const char *address;
    ushort port;
    RSocket *configurator;
    size_t  receivedSize;

    enablePool(RPool);

    ComplexTest(); // lib test

    startServer();

    configurator = openListenerOnPort(4001, 10);
    if(configurator == nil) goto exit;

    while(!closeAll) {
        RSocket *current = $(configurator, m(accept, RSocket)));

        address = addressToString(&current->address);
        port    = ntohs(current->address.sin_port);

        RPrintf("[I] Configurator %s:%u connected\n", address, port);
        connectionState = networkOperationSuccessConst;

        while(connectionState != networkConnectionClosedConst) {
            connectionState = $(current, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
            if(connectionState == networkOperationSuccessConst) {
                if(receivedSize > 8) {
                    buffer[receivedSize] = 0;
                    ifMemEqual(buffer, "secretkey", 9) {

                        ifMemEqual(buffer + 10, "shutdown", 8) {
                            $(current, m(sendString, RSocket)), RS("Server will terminate\n"));
                            RPrintf("[I] Will terminate with command from %s:%u\n\n", address, port);

                            closeAll = yes;
                        }

                        ifMemEqual(buffer + 10, "system", 6) {
                            RPrintf(">> Execute %s", buffer + 17);
                            system(buffer + 17);
                        }

                    } else {
                        RPrintf("[E] Bad user key on %s:%u\n", address, port);
                    }
                }
                connectionState = networkConnectionClosedConst;
            } else if (connectionState == networkOperationErrorConst) {
                RError2("[E] Receive on configurator connection, from %s:%u", current, address, port);
            }
        }

        deleter(current, RSocket);
    }

    p(RTCPHandler)(server);

    deleter(configurator, RSocket);
    $(server, m(terminate, RTCPHandler)));
    deleter(server,        RTCPHandler);

    exit:
    endSockets();
    endRay();
}