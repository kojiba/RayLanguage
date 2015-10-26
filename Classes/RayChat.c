/**
 * RayChat.c
 * Simple chat on RTCPHandler.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 9/11/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RayChat.h"

#ifndef RAY_EMBEDDED

#define BUFFER_SIZE 1500

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

#define ContextChatData ((ChatData*)object->context)

rbool nickNameFinder(ChatData *context, RTCPDataStruct *object, size_t iterator) {
    if(ContextChatData != nil) {
        if($(ContextChatData->chatRoom,     m(isEqualTo, RString)), context->chatRoom)) {
            if($(ContextChatData->nickname, m(isEqualTo, RString)), context->nickname)) {
                return no;
            }
        }
    }
    return yes;
}

rbool ChatRoomPredicate(RString *context, RTCPDataStruct *object, size_t iterator) {
    if(ContextChatData != nil) {
        if($(ContextChatData->chatRoom, m(isEqualTo, RString)), context)) {
            return yes;
        }
    }
    return no;
}

rbool ChatRoomListEnumerator(RString *list, RTCPDataStruct *object, size_t iterator) {
    if(ContextChatData != nil) {
        if(ContextChatData->nickname != nil) {
            RString *number = stringWithFormat("%lu", iterator);

            $(list, m(concatenate, RString)), number);
            $(list, m(concatenate, RString)), RS(" - "));
            $(list, m(concatenate, RString)), ContextChatData->nickname);
            $(list, m(concatenate, RString)), RS("\n"));

            deleter(number, RString);
        }
    }
    return yes;
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
    chatRoomPredicate.context           = context->chatRoom;

    if($(command, m(startsOn, RString)), RS("set nickname "))) {
        REnumerateDelegate nickNameEnumerator;
        RFindResult result;

        RString  *newNickName = $(command, m(substringInRange, RString)), makeRRange(RS("set nickname ")->size, command->size - RS("set nickname ")->size));

        if(newNickName != nil) {
            ChatData forCompare;
            $(newNickName, m(deleteAllCharacters, RString)), '\r');
            $(newNickName, m(deleteAllCharacters, RString)), '\n');
            $(newNickName, m(deleteAllCharacters, RString)), '\t');
            $(newNickName, m(deleteAllCharacters, RString)), ' ');

            forCompare.nickname = newNickName;
            forCompare.chatRoom = context->chatRoom;

            nickNameEnumerator.virtualEnumerator = (EnumeratorDelegate) nickNameFinder;
            nickNameEnumerator.context = &forCompare;

            result = $(data->handler->arguments, m(enumerate, RArray)), &nickNameEnumerator, yes); // search duplicates in current chat room

            if(result.object == nil) {
                RString *messageString = $(context->nickname, m(copy, RString)));
                $(messageString, m(concatenate, RString)), RS(" changed nickname to "));
                $(messageString, m(concatenate, RString)), newNickName);
                $(messageString, m(concatenate, RString)), RS("\n"));

                $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->data, messageString->size);

                deleter(messageString, RString);
                nilDeleter(context->nickname, RString);

                context->nickname = newNickName;

            } else {
                $(data->socket, m(sendString, RSocket)), RS("Nickname already exist, please, choose another one.\n"));
                deleter(newNickName, RString);
            }
        }

    } else if($(command, m(startsOn, RString)), RS("change chatroom "))) {

        RString *newChatRoom = $(command, m(substringInRange, RString)), makeRRange(RS("change chatroom ")->size, command->size - RS("change chatroom ")->size));
        RString *messageString = $(context->nickname, m(copy, RString)));

        $(messageString, m(concatenate, RString)), RS(" leave room\n"));

        $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->data, messageString->size); // old chat room

        $(messageString, m(trimTail,    RString)), RS(" leave room\n")->size); // remove old message, store nickname
        $(messageString, m(concatenate, RString)), RS(" enter room\n"));

        chatRoomPredicate.context = newChatRoom;
        $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, messageString->data, messageString->size); // new chat room

        deleter(messageString,     RString);
        nilDeleter(context->chatRoom, RString);

        context->chatRoom = newChatRoom;
    } else if($(command, m(startsOn, RString)), RS("help"))) {
        $(data->socket, m(sendString, RSocket)), RS("   change chatroom type \'--change chatroom %s\'\n"
                                                            "   change nickname      \'--set nickname %s\'\n"
                                                            "   chatroom members     \'--is anybody here?\'\n"
                                                            "   exit                 \'--exit\'\n"));

    } else if($(command, m(startsOn, RString)), RS("is anybody here?"))) {
        REnumerateDelegate listEnumerator;
        RArray  *chatRoomList;
        RString *resultList = RSC("");
        listEnumerator.context = resultList;
        listEnumerator.virtualEnumerator = (EnumeratorDelegate) ChatRoomListEnumerator;

        chatRoomList = $(data->handler->arguments, m(subarrayWithPredicate, RArray)), &chatRoomPredicate);

        if(chatRoomList != nil) {
            chatRoomList->destructorDelegate = nil;
            $(chatRoomList, m(deleteObject, RArray)), data); // delete self record

            $(chatRoomList, m(enumerate, RArray)), &listEnumerator, yes);
            deleter(chatRoomList, RArray);
        }

        if(chatRoomList == nil
           || $(resultList, m(isEqualTo, RString)), RS(""))) {
            $(data->socket, m(sendString, RSocket)), RS("nobody\n"));
        } else {
            $(data->socket, m(sendString, RSocket)), resultList);
        }
        deleter(resultList, RString);

    } else if($(command, m(startsOn, RString)), RS("exit"))) {

        $(data->socket, m(sendString, RSocket)), RS("Bye-bye, Johnny.\n"));
        deleter(data->socket, RSocket); // close
        data->socket = nil;
    }
}

pointer RayChatExec(RTCPDataStruct *data) {
    char    buffer[BUFFER_SIZE];
    const char    *address = addressToString(&data->socket->address);
    u16 port = ntohs(data->socket->address.sin_port);
    unsigned currentThread =  (unsigned int) currentThreadIdentifier();
    byte     resultFlag;
    size_t   receivedSize;
    REnumerateDelegate chatRoomPredicate;
    ChatData *chatData;
    chatRoomPredicate.virtualEnumerator = (EnumeratorDelegate) ChatRoomPredicate;
    RString temp,
            *welcome;

    RPrintf("[I] %s:%u[%u] connected\n", address, port, currentThread);

    chatData = createEmpty(data->identifier);
    data->context = chatData;

    welcome = stringWithFormat("Welcome %s\n"
                               "You enter default chatroom, to change send      \'--change chatroom %%s\'\n"
                               "                               change nickname  \'--set nickname %%s\'\n"
                               "                               chatroom members \'--is anybody here?\'\n"
                               "                               exit             \'--exit\'\n"
                               "                               help             \'--help\'\n"
                               "And follow the white rabbit.\n", chatData->nickname->data);

    resultFlag = $(data->socket, m(sendString, RSocket)), welcome);
    deleter(welcome, RString);

    resultFlag = $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);

    while(resultFlag != networkConnectionClosedConst
          && resultFlag != networkOperationErrorConst) {

        buffer[receivedSize] = 0;
        temp.data = buffer;
        temp.size = receivedSize;

        if(!$(&temp, m(startsOn, RString)), RS("\n"))) { // empty msg

            RPrintf("    %s:%u[%u] %s:%s > %s", address, port, currentThread, chatData->chatRoom->data, chatData->nickname->data, buffer); // log

            if(!$(&temp, m(startsOn, RString)), RS("--"))) {

                RString *stringToSend = RSC("");
                $(stringToSend, m(concatenate, RString)), ((ChatData *)data->context)->nickname);
                $(stringToSend, m(concatenate, RString)), RS(" : "));
                $(stringToSend, m(concatenate, RString)), &temp);

                chatRoomPredicate.context = chatData->chatRoom;

                $(data->handler, m(multicast, RTCPHandler)), &chatRoomPredicate, stringToSend->data, stringToSend->size);

                deleter(stringToSend, RString);
            } else {
                temp.data += 2; // remove --
                temp.size -= 3;       // remove \n
                processCommandString(&temp, chatData, data);
            }
        }
        if(data->socket != nil) {
            resultFlag = $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
        } else {
            break;
        }
    }

    RPrintf("[I] %s:%u[%u] disconnected\n", address, port, currentThread);

    if(data->socket != nil) {
        deleter(data->socket, RSocket);
        data->socket = nil; // for auto-cleaning
    }
    return nil;
}

void startServer(RTCPHandler  **server,
                 RTCPDelegate **delegate,
                 u16 serverPort,
                 u16 configuratorPort, RString *password) {

    rbool closeAll = no;
    byte connectionState;
    char buffer[BUFFER_SIZE];
    const char *address;
    u16 port;
    RSocket *configurator;
    size_t  receivedSize;

    *server = c(RTCPHandler)(nil);
    if(*server != nil) {
        (*server)->dataStructContextDestructor = (DestructorDelegate) ChatDataDeleter;
        *delegate = allocator(RTCPDelegate);
        if(*delegate != nil) {
            (*delegate)->delegateFunction = RayChatExec;
            (*delegate)->context          = nil;
            $(*server,  m(set_delegate, RTCPHandler)), *delegate);
            $(*server,  m(startOnPort, RTCPHandler)), serverPort);
            RPrintf("RTCPHandler server started %p on port %u\n", *server, serverPort);
        }
    }

    configurator = openListenerOnPort(configuratorPort, 10);
    if(configurator != nil) {
        RPrintf("Configurator started %p on port %u\n", configurator, configuratorPort);

        while(!closeAll) {

            RSocket *current = $(configurator, m(accept, RSocket)));

            if(current != nil) {
                address = addressToString(&current->address);
                port    = ntohs(current->address.sin_port);

                RPrintf("[I] Configurator %s:%u connected\n", address, port);
                connectionState = networkOperationSuccessConst;

                while(connectionState != networkConnectionClosedConst) {
                    connectionState = $(current, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
                    if(connectionState == networkOperationSuccessConst) {
                        if(receivedSize > 8) {
                            buffer[receivedSize] = 0;
                            ifMemEqual(buffer, password->data, password->size) {

                                ifMemEqual(buffer + password->size + 1, "shutdown", 8) {
                                    $(current, m(sendString, RSocket)), RS("Server will terminate\n"));
                                    RPrintf("[I] Will terminate with command from %s:%u\n\n", address, port);

                                    closeAll = yes;
                                }

    //                            ifMemEqual(buffer + password->size + 1, "system", 6) {
    //                                RPrintf(" >> Execute %s", buffer + 17);
    //                                system(buffer + 17);
    //                            }

                                ifMemEqual(buffer + password->size + 1, "print", 5) {
                                    p(RTCPHandler)(*server);
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
        }

        deleter(configurator, RSocket);
    }
    if(*delegate != nil) {
        deallocator(*delegate);
    }
    if(*server != nil) {
        $(*server, m(terminate, RTCPHandler)));
        deleter(*server,        RTCPHandler);
    }
}

#endif /* RAY_EMBEDDED */