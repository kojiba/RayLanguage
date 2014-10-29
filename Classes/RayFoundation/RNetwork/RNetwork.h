/**
 * RNetwork.h
 * Simple socket init for cross-platform.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/


#ifndef __R_NETWORK_H__
#define __R_NETWORK_H__

#include "../RSyntax.h"

#ifdef _WIN32
    // Windows Sockets
    #include <winsock.h>
#else
    // pure BSD Sockets
    #include <sys/types.h>
    #include <sys/socket.h>

#endif

class(RSocket)
    int socket;
endOf(RSocket)

constructor(RSocket), int domain, int type, int protocol);

#endif /*__R_NETWORK_H__*/