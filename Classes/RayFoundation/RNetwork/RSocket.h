/**
 * RSocket.h
 * Sockets simple wrapper.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
 * 12/19/14 Ukraine Kharkiv
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

#include "RayFoundation/RayConfig.h"

#ifndef RAY_EMBEDDED

#include "RayFoundation/RMemoryOperations/RByteOperations.h"
#include "RayFoundation/RString/RString_UTF8.h"

#ifndef __WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    typedef int SocketDescriptor;
    #define endSockets()
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>

    typedef SOCKET SocketDescriptor;
    #define endSockets() WSACleanup();
#endif

extern const byte networkConnectionClosedConst;
extern const byte networkOperationErrorConst;
extern const byte networkOperationSuccessConst;


typedef struct sockaddr_in SocketAddressIn;
typedef struct sockaddr    SocketAddress;
typedef struct ip_mreq     MulticastAddress;

extern const
char* addressToString(SocketAddressIn *address);

class(RSocket)
        SocketDescriptor socket;

        u16              port;
        SocketAddressIn  address;
        socklen_t        addressLength;
        size_t           packetCounter;

endOf(RSocket)

RSocket * makeRSocket       (RSocket *object,            int socketType,   int protocolType);
RSocket * socketBindToPort  (int socketType,             int protocolType, u16 port);
RSocket * openListenerOnPort(u16 port,                   int queueCount);
RSocket * socketConnectedTo (const char * const address, u16 port);

extern constructor (RSocket));
       destructor  (RSocket);
       printer     (RSocket);

// Setters
       method(rbool, bindPort,           RSocket),    u16 port); // for receiver
       method(void,  setPort,            RSocket),    u16 port); // for sender

       method(rbool, enableBroadCast,    RSocket),    rbool enable);

       method(rbool, joinMulticastGroup, RSocket),    const char * const address);
       method(void,  setAddress,         RSocket),    const char * const address);
       method(void,  reuseAddress,       RSocket));

       method(int,       listen,         RSocket),    int queueCount);
       method(RSocket *, accept,         RSocket));
       method(rbool,     connect,        RSocket));

// Main methods
       method(byte,      send,           RSocket),    const pointer buffer, size_t size);  // sends to receiver size bytes
       method(byte,      sendTo,         RSocket),    const pointer buffer, size_t size);

extern method(byte,      sendString,     RSocket),    const RString *string);  // sends to receiver size bytes
extern method(byte,      sendData,       RSocket),    const RData *data);  // sends to receiver size bytes

// buffer must be pre allocated at least 1500 bytes, returns network consts
       method(byte,      receive,        RSocket),    pointer buffer, size_t bufferSize, size_t *receivedSize);
       method(byte,      receiveFrom,    RSocket),    pointer buffer, size_t bufferSize, size_t *receivedSize);

extern method(RString *, receiveString, RSocket));  // receives string must be destroyed, buff up to 1500

#endif

#endif /*__R_SOCKET_H__*/