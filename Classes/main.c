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
#include <RayFoundation/RCString/RCString_File.h>

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
    result->nickname = stringWithFormat("Anon#%lu", count);
    result->chatRoom = RSC("default");
    return result;
}

ChatData *createWithNickname(RString *nick, RString *chatRoom) {
    ChatData *result = allocator(ChatData);
    result->nickname = nick;
    result->chatRoom = chatRoom;
    return result;
}

rbool nickNameFinder(ChatData *context, RTCPDataStruct *object, size_t iterator) {
    if($(((ChatData*)object->context)->chatRoom, m(isEqualTo, RCString)), context->chatRoom)) {
        if($(((ChatData*)object->context)->nickname, m(isEqualTo, RCString)), context->nickname)) {
            return no;
        }
    }
    return yes;
}

rbool ChatRoomPredicate(RString *context, RTCPDataStruct *object, size_t iterator) {
    if($(((ChatData*)object->context)->chatRoom, m(isEqualTo, RCString)), context)) {
       return yes;
    }
    return no;
}

destructor(ChatData) {
    nilDeleter(object->nickname, RString);
    nilDeleter(object->chatRoom, RString);
}

void ChatDataDeleter(ChatData *object) {
    deleter(object, ChatData);
}

void processCommandString(const RString *command, ChatData *context, RTCPDataStruct *data) {
    REnumerateDelegate chatRoomPredicate;
    chatRoomPredicate.virtualEnumerator = (EnumeratorDelegate) ChatRoomPredicate;

    if($(command, m(startsOn, RCString)), RS("set nickname "))) {
        REnumerateDelegate nickNameEnumerator;
        RFindResult result;

        RString  *newNickName = $(command, m(substringInRange, RCString)), makeRRange(RS("set nickname ")->size, command->size - RS("set nickname ")->size));

        if(newNickName != nil) {
            ChatData forCompare;
            $(newNickName, m(deleteAllCharacters, RCString)), '\r');
            $(newNickName, m(deleteAllCharacters, RCString)), '\n');
            $(newNickName, m(deleteAllCharacters, RCString)), '\t');
            $(newNickName, m(deleteAllCharacters, RCString)), ' ');

            forCompare.nickname = newNickName;
            forCompare.chatRoom = context->chatRoom;

            nickNameEnumerator.virtualEnumerator = (EnumeratorDelegate) nickNameFinder;
            nickNameEnumerator.context = &forCompare;

            chatRoomPredicate.context = context->chatRoom;

            result = $(data->handler->arguments, m(enumerate, RArray)), &nickNameEnumerator, yes); // search duplicates in current chat room

            if(result.object == nil) {
                RString *messageString = $(context->nickname, m(copy, RCString)));
                $(messageString, m(concatenate, RCString)), RS(" changed nickname to "));
                $(messageString, m(concatenate, RCString)), newNickName);
                $(messageString, m(concatenate, RCString)), RS("\n"));

                $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->baseString, messageString->size);

                deleter(messageString, RString);
                nilDeleter(context->nickname, RString);

                context->nickname = newNickName;

            } else {
                $(data->socket, m(sendString, RSocket)), RS("Nickname already exist, please, choose another one.\n"));
                deleter(newNickName, RString);
            }
        }

    } else if($(command, m(startsOn, RCString)), RS("change chatroom "))) {

        RString *newChatRoom = $(command, m(substringInRange, RCString)), makeRRange(RS("change chatroom ")->size, command->size - RS("change chatroom ")->size));
        RString *messageString = $(context->nickname, m(copy, RCString)));



        $(messageString, m(concatenate, RCString)), RS(" leave room\n"));

        chatRoomPredicate.context = context->chatRoom;
        $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->baseString, messageString->size); // old chat room

        $(messageString, m(trimTail, RCString)), RS(" leave room\n")->size); // remove old message, store nickname
        $(messageString, m(concatenate, RCString)), RS(" enter room\n"));

        chatRoomPredicate.context = newChatRoom;
        $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->baseString, messageString->size); // new chat room

           deleter(messageString,     RString);
        nilDeleter(context->chatRoom, RString);

        context->chatRoom = newChatRoom;
    }
}

pointer exec(RTCPDataStruct *data) {
    char    buffer[BUFFER_SIZE];
    const char    *address = addressToString(&data->socket->address);
    ushort            port = ntohs(data->socket->address.sin_port);
    unsigned currentThread =  (unsigned int) currentThreadIdentifier();
    byte     resultFlag;
    size_t   receivedSize;
    REnumerateDelegate chatRoomPredicate;
    ChatData *chatData;
    chatRoomPredicate.virtualEnumerator = (EnumeratorDelegate) ChatRoomPredicate;
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

            RPrintf("    %s:%u[%u] %s:%s > %s", address, port, currentThread, chatData->chatRoom->baseString, chatData->nickname->baseString, buffer); // log

            if(!$(&temp, m(startsOn, RCString)), RS("--"))) {

                RString *stringToSend = RSC("");
                $(stringToSend, m(concatenate, RCString)), ((ChatData *)data->context)->nickname);
                $(stringToSend, m(concatenate, RCString)), RS(" : "));
                $(stringToSend, m(concatenate, RCString)), &temp);

                chatRoomPredicate.context = chatData->chatRoom;

                $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, stringToSend->baseString, stringToSend->size);

                deleter(stringToSend, RString);
            } else {
                temp.baseString += 2; // remove --
                temp.size -= 3;       // remove \n
                processCommandString(&temp, chatData, data);
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

    RPrintf("Please, input server secretkey to admin on %u port\n", configurator_port);
    RString *password = getInputString();

    while(password->size <= 12) {
        RPrintf("Please, reenter a secretkey at least 12 bytes\n");
        password = getInputString();
    }

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
                    ifMemEqual(buffer, password->baseString, password->size) {

                        ifMemEqual(buffer + password->size + 1, "shutdown", 8) {
                            $(current, m(sendString, RSocket)), RS("Server will terminate\n"));
                            RPrintf("[I] Will terminate with command from %s:%u\n\n", address, port);

                            closeAll = yes;
                        }

                        ifMemEqual(buffer + password->size + 1, "system", 6) {
                            RPrintf(" >> Execute %s", buffer + 17);
                            system(buffer + 17);
                        }

                        ifMemEqual(buffer + password->size + 1, "print", 5) {
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
    deleter(password, RString);
    endSockets();


    endRay();
}