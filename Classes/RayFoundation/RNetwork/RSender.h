/**
 * RSender.h
 * Simple UDP sender for Vila. (bundled to libRay as networking part)
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
 
#ifndef __R_SENDER_H__
#define __R_SENDER_H__

#include <RSyntax.h>
#include <RCString.h>
#include <RByteOperations.h>
#include <RSocket.h>

class(RSender)
    SocketDescriptor socket;

    uint16_t         port;
    SocketAddressIn  receiverAddress;
    size_t           addressLength;
    size_t           packetCounter;
endOf(RSender)

constructor (RSender),    uint16_t port);
destructor  (RSender);
printer     (RSender);

// Setters
method(rbool, rebindPort,         RSender),    uint16_t port);
method(void,  setReceiverAddress, RSender),    const char * const address);

// Main method
method(byte,  send,               RSender),    RCString *string); // sends to receiver size bytes


#endif /*__R_SENDER_H__*/
