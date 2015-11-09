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

#include "RSocket.h"

#ifndef RAY_EMBEDDED

#include "RayFoundation/RClassTable/RClassTable.h"
#include <unistd.h>

const byte networkConnectionClosedConst = 0;
const byte networkOperationErrorConst   = 254;
const byte networkOperationSuccessConst = 1;

#ifdef _WIN32
    static rbool   isFirstSocket = yes;
    static WSADATA wsaData;

    #define wsaStartUp() WSAStartup(MAKEWORD(2, 2), &wsaData)
    #define NOT_RAISE_SIGPIPE_OPTION 0
#else
    #ifdef __APPLE__
        #define NOT_RAISE_SIGPIPE_OPTION SO_NOSIGPIPE
    #endif
    #ifdef __linux
        #define NOT_RAISE_SIGPIPE_OPTION MSG_NOSIGNAL
    #endif
#endif

inline const char* addressToString(SocketAddressIn *address) {
    return inet_ntoa(address->sin_addr);
}

RSocket * makeRSocket(RSocket *object, int socketType, int protocolType) {
    object = allocator(RSocket);
    if (object != nil) {
#ifdef _WIN32
        if(isFirstSocket) {
            int result = wsaStartUp();
            if(result != 0) {
                RError("Windows sockets startup failed, error code in begin of this line.", (pointer)result);
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

RSocket * socketBindToPort(int socketType, int protocolType, u16 port) {
    RSocket *result = makeRSocket(nil, socketType, protocolType);
    if(result != nil) {
        if($(result, m(bindPort, RSocket)), port) == no) {
            deallocator(result);
            result = nil;
        }
    }
    return result;
}

RSocket * openListenerOnPort(u16 port, int queueCount) {
    RSocket *result = socketBindToPort(SOCK_STREAM, IPPROTO_TCP, port);
    if(result != nil) {
        if($(result, m(listen, RSocket)), queueCount) < 0) {
            RError("RSocket. Error open listening socket.", result);
            deleter(result, RSocket);
            result = nil;
        }
    }
    return result;
}

RSocket * socketConnectedTo(const char * const address, u16 port) {
    RSocket *result = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
    if(result != nil) {
        $(result, m(setPort, RSocket)), port);
        $(result, m(setAddress, RSocket)), address);
        if($(result, m(connect, RSocket))) == no) {
            RError2("RSocket. Error connecting to %s:%u.", result, address, port);
            deleter(result, RSocket);
            result = nil;
        }
    }
    return result;
}

inline constructor(RSocket)) {
    return makeRSocket(object, SOCK_DGRAM, IPPROTO_IP);
}

destructor(RSocket) {
#ifdef _WIN32
    closesocket(object->socket);
#else
    shutdown(object->socket, SHUT_RDWR);
#ifndef __APPLE__                    // get SIGPIPE here on OSX
    if(close(object->socket) < 0) {
        RError("RSocket. Can't close socket.", object);
    }
#endif
    object->socket = -1;
#endif
}

printer(RSocket) {
    RPrintf("%s object - %p {\n", toString(RSocket), object);
    RPrintf("\t Socket descriptor : %d\n", object->socket);
    RPrintf("\t Packet counter    : %lu\n", object->packetCounter);
    RPrintf("} - %p\n", object);
}

#pragma mark Setters

method(rbool, bindPort, RSocket), u16 port) {
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

method(void, setPort, RSocket), u16 port) {
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

method(int, listen, RSocket), int queueCount) {
    return $(object->socket, listen), queueCount);
}

method(RSocket *, accept, RSocket)) {
    RSocket *result = allocator(RSocket);
    if(result != nil) {
        flushAllToByte((byte *) result, sizeof(RSocket), 0);
        result->socket = accept(object->socket,
                                (SocketAddress*) &result->address,
                                                 &result->addressLength);
        if(result->socket < 0) {
            // did not display error here
            deallocator(result);
            result = nil;
        }
    }
    return result;
}

method(rbool, connect, RSocket)) {
#ifdef _WIN32
    if(connect(object->socket,
               (SocketAddress*) &object->address,
                                &object->addressLength) != SOCKET_ERROR){
        return yes;
    } else {
        return no;
    }
#else
    if(connect(object->socket,
               (SocketAddress*) &object->address,
                                 object->addressLength) == 0) {
        return yes;
    } else {
        return no;
    }
#endif
}


#pragma mark Main Methods

method(byte, send, RSocket), const pointer buffer, size_t size) {
    ssize_t messageLength = send(object->socket,
                                 buffer,
                                 size,
                                 0);
//    RPrintf("Sent size %lu\n", messageLength);
    if (messageLength < 0) {
        return networkOperationErrorConst;
    } else if (messageLength != 0) {
        ++object->packetCounter;
        return networkOperationSuccessConst;
    } else {
        return networkConnectionClosedConst;
    }
}

method(byte, sendTo, RSocket), const pointer buffer, size_t size) {
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


inline method(byte, sendString, RSocket), const RString *string) {
    return $(object, m(send, RSocket)), string->data, string->size);
}

inline method(byte, sendData, RSocket), const RData *data) {
    return $(object, m(send, RSocket)), data->data, data->size);
}

method(byte, receive, RSocket), pointer buffer, size_t bufferSize, size_t *receivedSize) {
    ssize_t messageLength = recv(object->socket,
                                 buffer,
                                 bufferSize,
                                 0);

    if (messageLength < 0) {
        return networkOperationErrorConst;
    } else if(messageLength != 0) {
        ++object->packetCounter;
        *receivedSize = (size_t) messageLength;
        return networkOperationSuccessConst;
    } else {
        return networkConnectionClosedConst;
    }
}

method(byte, receiveFrom, RSocket), pointer buffer, size_t bufferSize, size_t *receivedSize) {
    ssize_t messageLength = recvfrom(object->socket,
                                     buffer,
                                     bufferSize,
                                     0,
                                     (SocketAddress*) &object->address,
                                                      &object->addressLength);

    if (messageLength < 0) {
        return networkOperationErrorConst;
    } else if(messageLength != 0) {
        ++object->packetCounter;
        *receivedSize = (size_t) messageLength;
        return networkOperationSuccessConst;
    } else {
        return networkConnectionClosedConst;
    }
}

inline method(RString *, receiveString, RSocket)) {
    char *buffer = arrayAllocator(char, 1500);
    size_t size;
    if(buffer != nil) {
        if($(object, m(receive, RSocket)), buffer, 1500, &size) == networkOperationSuccessConst) {
            return RCS(buffer);
        }
    }
    return nil;
}

#endif