/**
 * RSocket.h
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
 
#ifndef __RSocket_H__
#define __RSocket_H__

#ifndef __WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    typedef int SocketDescriptor;
#else
    #include <winsock2.h>
    typedef SOCKET SocketDescriptor;
#endif

typedef struct sockaddr_in SocketAddressIn;
typedef struct sockaddr    SocketAddress;

#endif /*__RSocket_H__*/
