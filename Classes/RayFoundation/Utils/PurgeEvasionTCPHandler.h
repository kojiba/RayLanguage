/**
 * PurgeEvasionTCPHandler.h
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

#ifndef __PURGE_EVASION_TCP_HANDLER__
#define __PURGE_EVASION_TCP_HANDLER__

#include "RayFoundation/RNetwork/RTCPHandler.h"

#ifndef RAY_EMBEDDED

protocol(PEKeyGeneratorDelegate)
    RData* (*keyForConnectionData)(RTCPDataStruct *connectionData);
endOf(PEKeyGeneratorDelegate)

class(RTCPHandlerPE)
    RTCPHandler *handler;
    RTCPDelegate selfDelegate;
    PEKeyGeneratorDelegate *delegate;
endOf(RTCPHandlerPE)

constructor(RTCPHandlerPE));
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

#endif /* RAY_EMBEDDED */

#endif /*__PURGE_EVASION_TCP_HANDLER__*/
