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

#include "Tests.h"

#define BUFFER_SIZE 1500

pointer exec(RTCPDataStruct *data) {
    if(data->socket != nil) {
        const char   *address    = addressToString(&data->socket->address);
              char    buffer[BUFFER_SIZE];
              ushort  port       = ntohs(data->socket->address.sin_port);
              byte    resultFlag;

        RPrintf("%s:%u connected, tuid : %u\n", address, port, (unsigned int) currentThreadIdentifier());

        $(data->socket, m(sendString, RSocket)), RS("Hello world!\n"));

        resultFlag = $(data->socket, m(receive, RSocket)), buffer, 1000);
        while(resultFlag != networkConnectionClosedConst) {
            RPrintf("%s", buffer);
            resultFlag =  $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE);
        }

        RPrintf("%s:%u disconnected\n", address, port);

        deleter(data->socket, RSocket);
    }
    deallocator(data);
    return nil;
}

RTCPHandler *server = nil;

void serverFunc(void) {
    server = c(RTCPHandler)(nil);
    if(server != nil) {
        RPrintf("RTCPHandler %p in %u thread\n", server, (unsigned int) currentThreadIdentifier());

        RTCPDelegate delegate;
        delegate.delegateFunction = (RThreadFunction) exec;
        delegate.context = &delegate;
        if($(server->listener, m(bindPort, RSocket)), 4000) == yes) {
            $(server,  m(set_delegate, RTCPHandler)), &delegate);
            $(server,  m(start,        RTCPHandler)), server); // blocks thread until m(terminate, RTCPHandler)
        }
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

    enablePool(RPool);
    ComplexTest();

    startServer();

    configurator = openListenerOnPort(4001, 10);
    if(configurator == nil) goto exit;

    while(!closeAll) {
        RSocket *current = $(configurator, m(accept, RSocket)));

        address = addressToString(&current->address);
        port    = ntohs(current->address.sin_port);

        RPrintf("Configurator %s:%u connected\n", address, port);
        connectionState = networkOperationSuccessConst;

        while(connectionState != networkConnectionClosedConst) {
            connectionState = $(current, m(receive, RSocket)), buffer, BUFFER_SIZE);
            if(connectionState == networkOperationSuccessConst) {
                ifMemEqual(buffer, "secretkey", 9) {

                    ifMemEqual(buffer + 10, "shutdown", 8) {
                        $(current, m(sendString, RSocket)), RS("Server will terminate\n"));
                        RPrintf("Will terminate with command from %s:%u\n\n", address, port);

                        closeAll = yes;
                    }
                } else {
                    RPrintf("[E] Bad user key on %s:%u\n", address, port);
                }
                connectionState = networkConnectionClosedConst;
            } else if (connectionState == networkOperationErrorConst) {
                RError2("Receive on configurator connection, from %s:%u", current, address, port);
            }
        }

        deleter(current, RSocket);
    }

    deleter(configurator, RSocket);
    $(server, m(terminate, RTCPHandler)));
    deleter(server,        RTCPHandler);

exit:
    endSockets();
    endRay();
}
