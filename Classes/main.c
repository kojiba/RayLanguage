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

//void processCommandString(RString *command, RTCPDataStruct *data) {
//    if ($(command, m(startsOn, RCString)), RS("--nickname")) ) {
//        $(command, m(trimHead, RCString)), RS("--nickname")->size);
//        $(command, m(deleteAllSubstrings, RCString)), RS("\n"));
//        $(command, m(deleteAllSubstrings, RCString)), RS("\t"));
//        $(command, m(deleteAllSubstrings, RCString)), RS(" "));
//        setNickName(command);
//
//    } else if($(command, m(startsOn, RCString)), RS("--nickname"))) {
//
//    }
//}

rbool sendToArgument(pointer context, RTCPDataStruct *data, size_t iterator) {
    if(data->socket != nil) {
        $(data->socket, m(sendString, RSocket)), context);
    }
    return yes;
}

pointer exec(RTCPDataStruct *data) {
    char    buffer[BUFFER_SIZE];
    const char    *address = addressToString(&data->socket->address);
    ushort            port = ntohs(data->socket->address.sin_port);
    unsigned currentThread =  (unsigned int) currentThreadIdentifier();
    byte     resultFlag;
    size_t   receivedSize;
    REnumerateDelegate executor;
    executor.virtualEnumerator = (EnumeretorDelegate) sendToArgument;

    RPrintf("[I] %s:%u connected [tuid : %u]\n", address, port, currentThread);

    $(data->socket, m(sendString, RSocket)), RS("Hello from RServer.\n"));

    resultFlag = $(data->socket, m(receive, RSocket)), buffer, 1000, &receivedSize);
    while(resultFlag != networkConnectionClosedConst) {
        buffer[receivedSize] = 0;
        RPrintf("%s:%u[%u] > %s", address, port, currentThread, buffer);

        executor.context = RCStringInit(buffer, receivedSize);

        $(((RTCPHandler*)data->context)->arguments, m(enumerate, RArray)), &executor, yes);
        deallocator(executor.context);

        resultFlag =  $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
    }

    RPrintf("[I] %s:%u disconnected [tuid : %u]\n", address, port, currentThread);

    deleter(data->socket, RSocket);
    data->socket = nil; // for auto-cleaning
    return nil;
}

RTCPHandler  *server   = nil;
RTCPDelegate *delegate = nil;

void startServer(void) {
    server = c(RTCPHandler)(nil);
    if(server != nil) {
        delegate = allocator(delegate);
        if(delegate != nil) {
            delegate->delegateFunction = (RThreadFunction) exec;
            delegate->context          = server;
            $(server,  m(set_delegate, RTCPHandler)), delegate);
            RPrintf("RTCPHandler starting %p\n", server);
            $(server,  m(startOnPort, RTCPHandler)), 4000);
        }
    }
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

                        ifMemEqual(buffer + 10, "print", 5) {
                            p(RTCPHandler)(server);
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

    deleter(configurator, RSocket);

    deallocator(delegate);
    $(server, m(terminate, RTCPHandler)));
    deleter(server,        RTCPHandler);

    exit:
    endSockets();
    endRay();
}