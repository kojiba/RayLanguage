/**
 * RSender.c
 * Simple UDP sender for Vila.
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

#include <RSender.h>
#include <RClassTable.h>

constructor(RSender), uint16_t port) {
    object = allocator(RSender);
    if(object != nil) {
        object->socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (object->socket < 0) {
            RError("RSender. Socket creation error.", object);
            deallocator(object);
            return nil;
        } else {

            SocketAddressIn address;
            flushAllToByte((byte *) &address, sizeof(address), 0);

            address.sin_family      = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_ANY);
            address.sin_port        = htons(port);
            object->receiverAddress = address;
            object->addressLength = sizeof(object->receiverAddress);

            object->port          = port;
            object->packetCounter = 0;
            object->classId       = registerClassOnce(toString(RSender));
        }
    }
    return object;
}

destructor(RSender) {
    shutdown(object->socket, 2);
}

printer(RSender) {
    RPrintf("%s object - %p {\n", toString(VReceiverThread), object);
    RPrintf("\t Socket descriptor : %p\n", (pointer) object->socket);
    RPrintf("\t Total packets received : %lu\n", object->packetCounter);
    RPrintf("} - %p\n", object);
}

#pragma mark Setters

method(rbool, rebindPort, RSender), uint16_t port) {
    SocketAddressIn address;
    flushAllToByte((byte *)&address, sizeof(address), 0);
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(port);

    // bind port
    if (bind(object->socket, (SocketAddress*) &address, sizeof(address)) < 0) {
        return no;
    }

    object->port = port;
    return yes;
}

method(void, setReceiverAddress, RSender), const char * const address) {
    flushAllToByte((byte *)&object->receiverAddress, sizeof(object->receiverAddress), 0);
    object->receiverAddress.sin_family = AF_INET;
    object->receiverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    object->receiverAddress.sin_port = htons(object->port);
    object->addressLength = sizeof(object->receiverAddress);
    object->receiverAddress.sin_addr.s_addr = inet_addr(address);
}

#pragma mark Main Method

method(byte, send, RSender), RByteArray *buffer) {
    ssize_t messageLength = sendto(object->socket,
            buffer->array,
            buffer->size,
            0,
            (SocketAddress*) &object->receiverAddress,
            (socklen_t) object->addressLength);

    if (messageLength < 0) {
        return 255;
    } else if(messageLength != 0) {
        ++object->packetCounter;
        return 1;
    } else {
        return 0;
    }
}