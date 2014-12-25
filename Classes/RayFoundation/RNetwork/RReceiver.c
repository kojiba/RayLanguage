/**
 * VReceiverThread.c
 * Messages receiver for Vila.
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

#include <RReceiver.h>
#include <RClassTable.h>

constructor(RReceiver), uint16_t port) {
    object = allocator(RReceiver);
    if(object != nil) {
        object->socket = socket(AF_INET, SOCK_DGRAM, 0);
        if (object->socket < 0) {
            RError("RReceiver. Socket creation error.", object);
            deallocator(object);
            return nil;
        } else {

            int YES = 1;
            SocketAddressIn address = {};
            // set reuse address
            if (setsockopt(object->socket,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &YES,
                    sizeof(int)) == -1) {
                RError("RReceiver. Set socket option SO_REUSEADDR failed.", object);
                deallocator(object);
                return nil;
            }

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = htonl(INADDR_ANY);
            address.sin_port = htons(port);
            object->addressLength = sizeof(address);

            if (bind(object->socket, (SocketAddress*) &address, sizeof(address)) < 0) {
                perror("bind");
                exit(1);
            }

            object->port          = port;
            object->packetCounter = 0;
            object->classId       = registerClassOnce(toString(VRecevier));
        }
    }
    return object;
}

destructor(RReceiver) {
    shutdown(object->socket, 2);
}

printer(RReceiver) {
    RPrintf("%s object - %p {\n", toString(VReceiverThread), object);
    RPrintf("\t Socket descriptor : %p\n", (pointer) object->socket);
    RPrintf("\t Total packets received : %lu\n", object->packetCounter);
    RPrintf("} - %p\n", object);
}

#pragma mark Setters

method(rbool, rebindPort, RReceiver), uint16_t port) {
    SocketAddressIn address;
    flushAllToByte((byte *) &address, sizeof(address), 0);
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

method(rbool, joinMulticastGroup, RReceiver), char *address) {
    struct ip_mreq multicastAddress;
    multicastAddress.imr_multiaddr.s_addr = inet_addr(address);
    multicastAddress.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(object->socket,
            IPPROTO_IP,
            IP_ADD_MEMBERSHIP,
            &multicastAddress,
            sizeof(multicastAddress)) < 0) {
        return no;
    } else {
        return yes;
    }
}

#pragma mark Main Method

method(byte, receive, RReceiver), RByteArray *buffer) {
    ssize_t messageLength = recvfrom(object->socket,
            buffer->array,
            buffer->size,
            0,
            (SocketAddress*) &object->senderAddress,
            (socklen_t *)    &object->addressLength);

    if (messageLength < 0) {
        return 255;
    } else if(messageLength != 0) {
        buffer->array[messageLength] = 0;
        ++object->packetCounter;
        return 1;
    } else {
        return 0;
    }
}