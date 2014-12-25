/**
 * RReceiver.h
 * Simple UDP receiver for Vila. (bundled to libRay as networking part)
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/16/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/
 
#ifndef __R_RECEIVER_H__
#define __R_RECEIVER_H__


#include <RSyntax.h>
#include <RThread.h>
#include <RCString.h>
#include <RByteOperations.h>
#include "RSocket.h"

class(RReceiver)
       size_t           packetCounter;
       SocketDescriptor socket;
       uint16_t         port;
       SocketAddressIn  senderAddress;
       size_t           addressLength;
endOf(RReceiver)

constructor (RReceiver),    uint16_t port);
destructor  (RReceiver);
printer     (RReceiver);

// Setters
method(rbool, rebindPort,         RReceiver),    uint16_t port);
method(rbool, joinMulticastGroup, RReceiver),    char *address);

// Main method
method(byte,  receive,            RReceiver),    RByteArray *buffer); // buffer must be pre allocated at least 1500 bytes

#endif /*__R_RECEIVER_H__*/
