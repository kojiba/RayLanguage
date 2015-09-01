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

#define server_port 4000
#define configurator_port server_port + 1

typedef struct ChatData {
    RString *nickname;
    RString *chatRoom;
} ChatData;

ChatData *createEmpty(size_t count) {
    ChatData *result = allocator(ChatData);
    result->nickname = stringWithFormat("Anon-#%lu", count);
    result->chatRoom = RSC("default");
    return result;
}

destructor(ChatData) {
    nilDeleter(object->nickname, RString);
    nilDeleter(object->chatRoom, RString);
}

void ChatDataDeleter(ChatData *object) {
    deleter(object, ChatData);
}

void processCommandString(const RString *command, ChatData *context) {
    if($(command, m(startsOn, RCString)), RS("set nickname "))) {
        rbool success = yes;

        // todo search duplicates

        if(success) {
            nilDeleter(context->nickname, RString);
            context->nickname = $(command, m(substringInRange, RCString)), makeRRange(RS("set nickname ")->size, command->size - RS("set nickname ")->size));
        }
    }

    // todo change chatroom
}

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
    ChatData *chatData;
    executor.virtualEnumerator = (EnumeretorDelegate) sendToArgument;
    RCString temp;

    RPrintf("[I] %s:%u[%u] connected\n", address, port, currentThread);

    chatData = createEmpty(data->identifier);
    data->context = chatData;

    RString *welcome = stringWithFormat("Welcome %s\n"
                                        "You enter default chatroom, to change type \'--change chatroom name\'\n"
                                        "To change nickname \'--set nickname name\'\n", chatData->nickname->baseString);

    $(data->socket, m(sendString, RSocket)), welcome);
    deleter(welcome, RString);

    resultFlag = $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
    while(resultFlag != networkConnectionClosedConst) {
        buffer[receivedSize] = 0;
        temp.baseString = buffer;
        temp.size = receivedSize;

        if(!$(&temp, m(startsOn, RCString)), RS("\n"))) { // empty msg
            RPrintf("    %s:%u[%u] > %s", address, port, currentThread, buffer);
            if(!$(&temp, m(startsOn, RCString)), RS("--"))) {
                RString *stringToSend = RSC("");
                $(stringToSend, m(concatenate, RCString)), ((ChatData *)data->context)->nickname);
                $(stringToSend, m(concatenate, RCString)), RS(" : "));
                $(stringToSend, m(concatenate, RCString)), &temp);

                executor.context = stringToSend;

                $(data->handler->arguments, m(enumerate, RArray)), &executor, yes);

                deleter(stringToSend, RString);
            } else {
                temp.baseString += 2; // remove --
                temp.size -= 3;       // remove \n
                processCommandString(&temp, chatData);
            }
        }
        resultFlag = $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
    }

    RPrintf("[I] %s:%u[%u] disconnected\n", address, port, currentThread);

    deleter(data->socket, RSocket);
    data->socket = nil; // for auto-cleaning
    return nil;
}

RTCPHandler  *server   = nil;
RTCPDelegate *delegate = nil;

void startServer(void) {
    server = c(RTCPHandler)(nil);
    if(server != nil) {
        server->dataStructContextDestructor = (DestructorDelegate) ChatDataDeleter;
        delegate = allocator(delegate);
        if(delegate != nil) {
            delegate->delegateFunction = (RThreadFunction) exec;
            delegate->context          = server;
            $(server,  m(set_delegate, RTCPHandler)), delegate);
            $(server,  m(startOnPort, RTCPHandler)), server_port);
            RPrintf("RTCPHandler starting %p on port %u\n", server, server_port);
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

    configurator = openListenerOnPort(configurator_port, 10);
    if(configurator == nil) goto exit;
    RPrintf("Configurator started %p on port %u\n", configurator, configurator_port);

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
                            RPrintf(" >> Execute %s", buffer + 17);
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