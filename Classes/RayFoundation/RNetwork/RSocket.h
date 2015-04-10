/**
 * RSocket.h
 * Sockets simple wrapper.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/19/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/
 
#ifndef __R_SOCKET_H__
#define __R_SOCKET_H__

#include <RCString.h>
#include <RByteOperations.h>

#ifndef __WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

typedef int SocketDescriptor;
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef SOCKET SocketDescriptor;
#endif

extern const byte networkConnectionClosedConst;
extern const byte networkOperationErrorConst;
extern const byte networkOperationSuccessConst;


typedef struct sockaddr_in SocketAddressIn;
typedef struct sockaddr    SocketAddress;

class(RSocket)
        SocketDescriptor socket;

        uint16_t         port;
        SocketAddressIn  address;
        size_t           addressLength;
        size_t           packetCounter;

endOf(RSocket)

constructor (RSocket));
destructor  (RSocket);
printer     (RSocket);

// Setters
method(rbool, bindPort,           RSocket),    uint16_t port); // for receiver
method(void,  setPort,            RSocket),    uint16_t port); // for sender

method(rbool, joinMulticastGroup, RSocket),    const char * const address);
method(void,  setAddress,         RSocket),    const char * const address);
method(void,  reuseAddress,       RSocket));

// Main methods
method(byte,  send,               RSocket),    RByteArray *buffer);  // sends to receiver size bytes
method(byte,  receive,            RSocket),    RByteArray *buffer);  // buffer must be pre allocated at least 1500 bytes, return 255 if fails, 1 if received some

#endif /*__R_SOCKET_H__*/
