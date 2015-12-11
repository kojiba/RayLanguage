/**
 * PurgeEvasionTCPHandler.c
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

#include <RayFoundation/RClassTable/RClassTable.h>
#include "RayFoundation/REncoding/purge.h"
#include "PurgeEvasionTCPHandler.h"

RTCPDataStructPE* RTCPDataStructPEWithSession(PEConnection *connection){
    if(connection != nil) {
        RTCPDataStructPE *result = allocator(RTCPDataStructPE);
        if(result) {
            result->connection = connection;
            return result;
        }
    }
    return nil;
}

void RTCPDataStructPEWithSessionDeleter(RTCPDataStructPE *data) {
    if(data->connection != nil) {
        PEConnectionDeleter(data->connection);
        data->connection = nil;
        data->ownerData->socket = nil;
    }

    if(data->context != nil && data->dataStructContextDestructor != nil) {
        data->dataStructContextDestructor(data->context);
    }
    deallocator(data);
}

pointer startPESessionOnConnection(RTCPDataStruct *data) {
    RTCPHandlerPE* peHandler   = ((RTCPHandlerPE*)data->delegate->context);
         uint64_t  blankKey[8] = {};
            RData* sessionKey  = nil;

    if(peHandler) {
        RPrintf("startPESessionOnConnection %p\n", data);
        sessionKey = peHandler->keyGeneratorDelegate->keyForConnectionData(data, peHandler->keyGeneratorDelegate->context);
        if(sessionKey) {
            if(sessionKey->size != purgeBytesCount) {
                RWarning1("RTCPHandlerPE. Weak session key, size in bytes %lu", peHandler, sessionKey->size);
            }
            if(sessionKey->data == nil) {
                RError1("RTCPHandlerPE. Bad session key data, cancelling session identifier %lu", peHandler, data->identifier);
                return nil;
            }

            data->context = RTCPDataStructPEWithSession(PEConnectionInit(data->socket, initPEContext((uint64_t *) sessionKey->data)));
        } else {
            data->context = RTCPDataStructPEWithSession(PEConnectionInit(data->socket, initPEContext(blankKey)));
        }

        ((RTCPDataStructPE *)data->context)->dataStructContextDestructor = peHandler->dataStructContextDestructor;
        ((RTCPDataStructPE *)data->context)->context = peHandler->delegate->context;
        ((RTCPDataStructPE *)data->context)->ownerData = data;

        return peHandler->delegate->delegateFunction(data);


    } else {
        nilDeleter(data->socket, RSocket);
        data->socket = nil;
    }

    return nil;
}

rbool RTCPHandlerPEMulticastEnumerator(RString *context, RTCPDataStruct *data, size_t iterator) {
    if(data->socket != nil) {
        PEConnectionSendString(((RTCPDataStructPE*)data->context)->connection, context);
    }
    return yes;
}

constructor(RTCPHandlerPE)) {
    object = allocator(RTCPHandlerPE);
    if(object != nil) {
        object->handler = c(RTCPHandler)(nil);
        if(object->handler != nil) {
            object->keyGeneratorDelegate = nil;
            object->dataStructContextDestructor = nil;

            object->selfDelegate.context = object;
            object->selfDelegate.delegateFunction = startPESessionOnConnection;
            object->handler->dataStructContextDestructor = (DestructorDelegate) RTCPDataStructPEWithSessionDeleter;
            $(object->handler, m(set_delegate, RTCPHandler)), &object->selfDelegate);
            object->handler->multicastEnumerator.virtualEnumerator = (EnumeratorDelegate) RTCPHandlerPEMulticastEnumerator;

            object->classId = registerClassOnce(toString(RTCPHandlerPE));
        } else {
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(RTCPHandlerPE) {
    deleter(object->handler, RTCPHandler);
}

printer(RTCPHandlerPE) {
    $(object->handler, p(RTCPHandler)));
}

getterImpl(delegate, RTCPDelegate *, RTCPHandlerPE);
setterImpl(delegate, RTCPDelegate *, RTCPHandlerPE);

getterImpl(keyGeneratorDelegate, PEKeyGeneratorDelegate *, RTCPHandlerPE);
setterImpl(keyGeneratorDelegate, PEKeyGeneratorDelegate *, RTCPHandlerPE);

method(rbool, checkDelegates, RTCPHandlerPE)) {
    if(object->keyGeneratorDelegate == nil) {
        RWarning("RTCPHandlerPE. Key generator delegate is nil, What do you want to start?!", object);
        return no;
    }
    return yes;
}

inline
method(void, startOnPort, RTCPHandlerPE), uint16_t port) {
    if($(object, m(checkDelegates, RTCPHandlerPE)))){
        $(object->handler, m(startOnPort, RTCPHandler)), port);
    }
}

inline
method(void, startWithHost, RTCPHandlerPE), const RString *address, u16 port, size_t connectionsCount) {
    if($(object, m(checkDelegates, RTCPHandlerPE)))){
        $(object->handler, m(startWithHost, RTCPHandler)), address, port, connectionsCount);
    }
}

inline
method(void, waitConnectors, RTCPHandlerPE)) {
    $(object->handler, m(waitConnectors, RTCPHandler)));
}

inline
method(void, terminate, RTCPHandlerPE)) {
    $(object->handler, m(terminate, RTCPHandler)));
}

inline
method(void, multicast, RTCPHandlerPE), REnumerateDelegate *predicate, const pointer buffer, size_t size) {
    $(object->handler, m(multicast, RTCPHandler)), predicate, buffer, size);
}

inline
method(void, broadcast, RTCPHandlerPE), RString *string) {
    $(object->handler, m(broadcast, RTCPHandler)), string);
}
