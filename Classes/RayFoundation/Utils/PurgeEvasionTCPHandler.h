/**
 * PurgeEvasionTCPHandler.h
 * RTCPHandler with PESessions
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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
#include "PurgeEvasionConnection.h"

#ifndef RAY_EMBEDDED

typedef struct RTCPDataStructPE {
    pointer context;
    PEConnection *connection;
    DestructorDelegate dataStructContextDestructor;

    RTCPDataStruct *ownerData;
} RTCPDataStructPE;

protocol(PEKeyGeneratorDelegate)
    RData* (*keyForConnectionData)(RTCPDataStruct *connectionData, pointer context);
endOf(PEKeyGeneratorDelegate)

class(RTCPHandlerPE)
    RTCPDelegate selfDelegate;
    RTCPHandler    *handler;

    RTCPDelegate           *delegate;
    PEKeyGeneratorDelegate *keyGeneratorDelegate;

    DestructorDelegate   dataStructContextDestructor; // RTCPDataStruct->context->context
endOf(RTCPHandlerPE)

constructor(RTCPHandlerPE));
destructor(RTCPHandlerPE);

printer(RTCPHandlerPE);

getter(delegate, RTCPDelegate *, RTCPHandlerPE);
setter(delegate, RTCPDelegate *, RTCPHandlerPE);

getter(keyGeneratorDelegate, PEKeyGeneratorDelegate *, RTCPHandlerPE);
setter(keyGeneratorDelegate, PEKeyGeneratorDelegate *, RTCPHandlerPE);

extern method(void, startOnPort,    RTCPHandlerPE),    uint16_t port);
extern method(void, startWithHost,  RTCPHandlerPE),    const RString *address, u16 port, size_t connectionsCount);
extern method(void, waitConnectors, RTCPHandlerPE));
extern method(void, terminate,      RTCPHandlerPE));
extern method(void, multicast,      RTCPHandlerPE),    REnumerateDelegate *predicate, const pointer buffer, size_t size);
extern method(void, broadcast,      RTCPHandlerPE),    RString *string);

#endif /* RAY_EMBEDDED */

#endif /*__PURGE_EVASION_TCP_HANDLER__*/
