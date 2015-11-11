/**
 * PurgeEvasionTCPHandler.c
 * RTCPHandler with PESessions
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 11/10/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "PurgeEvasionTCPHandler.h"

constructor(RTCPHandlerPE)) {
    object = allocator(RTCPHandlerPE);
    if(object != nil) {
        object->handler = c(RTCPHandler)(nil);
        if(object->handler != nil) {
            $(object->handler, m(set_delegate, RTCPHandler)), &object->selfDelegate);

        } else {
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(RTCPHandlerPE);

printer(RTCPHandlerPE);

getter(delegate, RTCPDelegate *, RTCPHandlerPE);
setter(delegate, RTCPDelegate *, RTCPHandlerPE);

method(void, startOnPort,    RTCPHandlerPE),    uint16_t port);
method(void, startWithHost,  RTCPHandlerPE),    RString *address, u16 port, size_t connectionsCount);
method(void, waitConnectors, RTCPHandlerPE));
method(void, terminate,      RTCPHandlerPE));
method(void, multicast,      RTCPHandlerPE),    REnumerateDelegate *predicate, const pointer buffer, size_t size);

extern
method(void, broadcast,     RTCPHandlerPE), RString *string);
