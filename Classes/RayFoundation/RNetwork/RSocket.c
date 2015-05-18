/**
 * RSocket.h
 * Sockets simple wrapper.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 4/8/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include <RSocket.h>

#ifndef RAY_EMBEDDED

#include <RClassTable.h>

const byte networkConnectionClosedConst = 0;
const byte networkOperationErrorConst   = 254;
const byte networkOperationSuccessConst = 1;

#ifdef _WIN32
    static rbool   isFirstSocket = yes;
    static WSADATA wsaData;

    #define wsaStartUp() WSAStartup(MAKEWORD(2, 2), &wsaData)
#endif

const char* addressToString(SocketAddressIn *address) {

    char buffer[4];
    buffer[0] = (byte)( address->sin_addr.s_addr & 0xFF);
    buffer[1] = (byte)((address->sin_addr.s_addr & 0xFF00) >> 8);
    buffer[2] = (byte)((address->sin_addr.s_addr & 0xFF0000) >> 16);
    buffer[3] = (byte)((address->sin_addr.s_addr & 0xFF000000) >> 24);
    return cstringWithFormat("%d.%d.%d.%d",buffer[0], buffer[1],  buffer[2], buffer[3]);
}

RSocket * makeRSocket(RSocket *object, int socketType, int protocolType) {
    object = allocator(RSocket);
    if (object != nil) {
#ifdef _WIN32
        if(isFirstSocket) {
            int result = wsaStartUp();
            if(result != 0) {
                RError("Windows sockets startup failed, errorcode in begin of line.", (pointer)result);
            }
            isFirstSocket = no;
        }
#endif
        object->socket = socket(AF_INET, socketType, protocolType);
        if (object->socket < 0) {
            RError("RSocket. Socket creation error.", object);
            deallocator(object);
            return nil;
        } else {
            object->packetCounter = 0;
            object->classId       = registerClassOnce(toString(RSocket));
        }
    }
    return object;
}

inline constructor(RSocket)) {
    return makeRSocket(object, SOCK_DGRAM, IPPROTO_IP);
}

destructor(RSocket) {
    shutdown(object->socket, 2);
}

printer(RSocket) {
    RPrintf("%s object - %p {\n", toString(RSocket), object);
    RPrintf("\t Socket descriptor : %d\n", object->socket);
    RPrintf("\t Packet counter    : %lu\n", object->packetCounter);
    RPrintf("} - %p\n", object);
}

#pragma mark Setters

method(rbool, bindPort, RSocket), uint16_t port) {
    SocketAddressIn address = {};
    address.sin_family = AF_INET;
    address.sin_port   = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind port
    if (bind(object->socket, (SocketAddress *) &address, sizeof(address)) < 0) {
        #ifdef _WIN32
            WSAGetLastError();
        #else
        #endif
        RError1("RSocket. Bind to port %u error.", object, port);
        return no;
    }

    object->port = port;
    return yes;
}

method(void, setPort, RSocket), uint16_t port) {
    object->port = port;
}

method(rbool, enableBroadCast, RSocket), rbool enable) {
    if (setsockopt(object->socket, SOL_SOCKET, SO_BROADCAST, (void *) &enable,
                   sizeof(enable)) < 0) {
        RError1("RSocket. Set broadcast to %d failed.", object, enable);
        return no;
    } else {
        return yes;
    }
}

method(rbool, joinMulticastGroup, RSocket), const char * const address) {
    MulticastAddress multicastAddress;
    multicastAddress.imr_multiaddr.s_addr = inet_addr(address);
    multicastAddress.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(object->socket,
                   IPPROTO_IP,
                   IP_ADD_MEMBERSHIP,
                   (char const *) &multicastAddress,
                   sizeof(multicastAddress)) < 0) {
        return no;
    } else {
        return yes;
    }
}

method(void, setAddress, RSocket), const char *const address) {
    flushAllToByte((byte *) &object->address, sizeof(object->address), 0);
    object->address.sin_family = AF_INET;
    object->address.sin_addr.s_addr = inet_addr(address);
    object->address.sin_port = htons(object->port);
    object->addressLength = sizeof(object->address);
}

method(void, reuseAddress, RSocket)) {
    int YES = 1;
    // set reuse address
    if (setsockopt(object->socket,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   (char const *) &YES,
                   sizeof(int)) == -1) {
        RError("RSocket. Set socket option SO_REUSEADDR failed.", object);
    }
}

method(RSocket *, accept, RSocket)) {
    RSocket *result = allocator(RSocket);
    if(result != nil) {
        flushAllToByte(result, sizeof(RSocket), 0);
        result->socket = accept(object->socket,
                                (SocketAddress*) &result->address,
                                                 &result->addressLength);
        if(result->socket < 0) {
            RError("RSocket. Accept socket error.", object);
            deallocator(result);
            result = nil;
        }
    }
    return result;
}

#pragma mark Main Method

method(byte, send, RSocket), const pointer buffer, size_t size) {
    ssize_t messageLength = sendto(object->socket,
                                   buffer,
                                   size,
                                   0,
                                   (SocketAddress *) &object->address,
                                                      object->addressLength);

    if (messageLength < 0) {
        return networkOperationErrorConst;
    } else if (messageLength != 0) {
        ++object->packetCounter;
        return networkOperationSuccessConst;
    } else {
        return networkConnectionClosedConst;
    }
}

inline method(byte, sendString, RSocket), const RCString *string) {
    return $(object, m(send, RSocket)), string->baseString, string->size);
}

#pragma mark Main Method

method(byte, receive, RSocket), pointer buffer, size_t size) {
    ssize_t messageLength = recvfrom(object->socket,
                                     buffer,
                                     size,
                                     0,
                                     (SocketAddress*) &object->address,
                                                      &object->addressLength);

    if (messageLength < 0) {
        return networkOperationErrorConst;
    } else if(messageLength != 0) {
        ++object->packetCounter;
        return networkOperationSuccessConst;
    } else {
        return networkConnectionClosedConst;
    }
}

#endif

/*
 * some samples, code to store
    char buffer[1500];

    RCString *html = stringFromFile("../Resources/hello.html");

    if(html == nil)
        goto exit;

    printf("Html content begin =================\n");
    p(RCString)(html);
    printf("Html content end ===================\n");

    printf("Html page size %lu\n", html->size);

    RCString *content = stringWithFormat("HTTP/1.1 200 OK\n"
                                         "Content-Type: text/html\n"
                                         "Content-Length: %lu\n"
                                         "\n"
                                         "%s", (html->size), html->baseString);


    if(content != nil) {
        RSocket *socket = makeRSocket(nil, SOCK_STREAM, IPPROTO_IP);
        if($(socket, m(bindPort, RSocket)), 5000) == no) {
            goto exit;
        } // open on http port

        $(socket->socket, listen), 10);
        for(;;) {
            RSocket * child = $(socket, m(accept, RSocket)));

            if (child != nil) {
                $(child, m(receive, RSocket)), &buffer, 1500);
                const char *addr = addressToString(&child->address);
                ushort port = ntohs(child->address.sin_port);
                printf("%s:%u connected\n", addr, port);

                printf("REQ ============\n"
                       "%s\n"
                       "REQ END ========\n", buffer);

                $(child, m(setAddress, RSocket)), "127.0.0.1");
                if($(child, m(sendString, RSocket)), content) == networkOperationSuccessConst) {
                    printf("send success\n");
                } else {
                    RError("Send data to socket", nil);
                    goto exit;
                }
                deleter(child, RSocket);

            } else {
                RError("Bad create child socket.", nil);
                goto exit;
            }
        }

        deleter(socket, RSocket);
        deleter(content, RCString);
    }
    deleter(html, RCString);

    exit:

    endSockets();
*/