/**
 * RTCPHandler.c
 * Threaded TCP connection handler.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 6/16/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RTCPHandler.h"
#include "RayFoundation/RClassTable/RClassTable.h"

#ifndef RAY_EMBEDDED

rbool RTCPHandlerInactiveContextDeleter(pointer context, pointer object, size_t iterator) {
    if(((RTCPDataStruct*)object)->socket == nil) {
        return yes;
    }
    return no;
}

void PrivateArgPrinter(RTCPDataStruct* object){
    RPrintf("%p socket : %p, id : %lu\n", object, object->socket, object->identifier);
}

void PrivateArgDeleter(RTCPDataStruct* object) {
    if(object->handler->dataStructContextDestructor != nil
          && object->context != nil) {
        object->handler->dataStructContextDestructor(object->context); // delete user-data
    }
    nilDeleter(object->socket, RSocket);
    RFree(object);
}

rbool RTCPHandlerMulticastEnumerator(RString *context, RTCPDataStruct *data, size_t iterator) {
    if(data->socket != nil) {
        $(data->socket, m(sendString, RSocket)), context);
    }
    return yes;
}

constructor(RTCPHandler)) {
    object = allocator(RTCPHandler);
    if(object != nil) {
        object->delegate = nil;
        object->threads = c(RThreadPool)(nil);
        if(object->threads != nil) {
            object->arguments = makeRArray();
            if(object->arguments != nil) {
                object->classId = registerClassOnce(toString(RTCPHandler));

                $(object->arguments, m(setPrinterDelegate,    RArray)), (PrinterDelegate)    PrivateArgPrinter);
                $(object->arguments, m(setDestructorDelegate, RArray)), (DestructorDelegate) PrivateArgDeleter);

                // init destructorPredicate and enumerator
                object->destructorPredicate.virtualEnumerator = RTCPHandlerInactiveContextDeleter;
                object->destructorPredicate.context           = object;
                object->multicastEnumerator.virtualEnumerator = (EnumeratorDelegate) RTCPHandlerMulticastEnumerator;

                object->terminateFlag = no;
                mutexWithType(&object->mutex, RMutexNormal);
            }
        }
    }
    return object;
}

destructor(RTCPHandler) {
    RMutexLock(&object->mutex);
    deleter(object->threads,   RThreadPool);
    deleter(object->arguments, RArray);
    RMutexUnlock(&object->mutex);
    RMutexDestroy(&object->mutex);
}

getterImpl(delegate, RTCPDelegate *, RTCPHandler)
setterImpl(delegate, RTCPDelegate *, RTCPHandler)

printer(RTCPHandler) {
    RPrintf("RTCPHandler %p -----------\n", object);
    RPrintf("\tMode: ");
    if(object->connectorMode) {
        RPrintf("Connector\n");
    } else {
        RPrintf("Server\n");
    }
    RPrintf("\tRunning thread tuid %u\n", (unsigned) RThreadIdOfThread(object->runningThread));
    RPrintf("------ Arguments : ");
    p(RArray)(object->arguments);
    RPrintf("RTCPHandler %p -----------\n\n", object);
}

method(void, privateStartInMode, RTCPHandler)) {
    $(object->threads, m(setDelegateFunction, RThreadPool)), (RThreadFunction) object->delegate->delegateFunction);

    if(!object->connectorMode) {
        $(object->listener, m(listen, RSocket)), RTCPHandlerListenerQueueSize);
    }

    if(object->handlerStartedNotifier != nil) {
        object->handlerStartedNotifier(object);
    }

    while(!object->terminateFlag) {
        rbool selfCleanup = yes;
        RSocket *socketInProcess = nil;
        if(!object->connectorMode) {
            socketInProcess = $(object->listener, m(accept, RSocket)));
        } else {
            selfCleanup = no;
            if(object->connectionsCount != 0) {
                socketInProcess = socketConnectedTo((const char *const) object->ipAddress->data, object->port);
                --object->connectionsCount;
            } else {
                return; // stop opening connections
            }
        }

        if(socketInProcess != nil) {
            RTCPDataStruct *argument = allocator(RTCPDataStruct);
            if(argument != nil) {
                argument->handler    = object;
                argument->delegate   = object->delegate;
                argument->context    = object->delegate->context;
                argument->socket     = socketInProcess;
                argument->identifier = object->arguments->count;

                $(object->arguments, m(addObject,  RArray)), argument);

                if(!object->connectorMode) {
                    // delete inactive worker arguments
                    if(object->arguments->count != 0
                       && (object->arguments->count % RTCPHandlerCheckCleanupAfter) == 0) {
                        $(object->arguments, m(deleteWithPredicate, RArray)), &object->destructorPredicate);
                    }
                }

                // finally, add new worker with auto-cleanup = selfCleanup
                $(object->threads, m(addWithArg, RThreadPool)), argument, selfCleanup);
            } elseError(
                    RError("RTCPHandler. Can't allocate thread argument.", object)
            );
        }
    }
}

method(void, startOnPort, RTCPHandler), uint16_t port) {
    if(object->delegate != nil
            && object->delegate->delegateFunction != nil) {
        if(!object->terminateFlag) {
            RMutexLock(&object->mutex);
            object->connectorMode = no;
            object->listener      = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
            if(object->listener != nil
               && $(object->listener, m(bindPort, RSocket)), port) == yes) {

                RThreadCreate(&object->runningThread, nil, (RThreadFunction) m(privateStartInMode, RTCPHandler), object);
            } elseError(
                    RError("RTCPHandler. startOnPort\n", object)
            );
            RMutexUnlock(&object->mutex);
        } elseWarning(
                RWarning("RTCPHandler. startOnPort. Already running, do nothing.", object)
        );
    } elseWarning(
            RWarning("RTCPHandler. startOnPort. Delegate or delgate function is nil. What do you want to start?!", object)
    );
}

method(void, startWithHost, RTCPHandler), const RString *address, u16 port, size_t connectionsCount) {
    if(object->delegate != nil
        && object->delegate->delegateFunction != nil
        && address != nil
        && address->data != nil
        && connectionsCount > 0
        && connectionsCount < (65536 - 1000)) {

        if(!object->terminateFlag) {
            if(!(address->size > 15
               || address->size < 7)) {

                RMutexLock(&object->mutex);
                object->connectionsCount = connectionsCount;
                object->ipAddress = address;
                object->port = port;
                object->connectorMode = yes;

                RThreadCreate(&object->runningThread, nil, (RThreadFunction) m(privateStartInMode, RTCPHandler),
                              object);
                RThreadJoin(object->runningThread); // wait when all connected

                RMutexUnlock(&object->mutex);

            } elseError(
                RError1("RTCPHandler. startWithHost. Bad host address string %s", object, address->data)
            )
        } elseWarning(
                RWarning("RTCPHandler. startWithHost. Handler already running.", object)
        );
    } elseWarning(
            RWarning("RTCPHandler. startWithHost. Delegate, or virtual function, or connectionsCount is nil. What do you want to start?!", object)
    );
}

method(void, waitConnectors, RTCPHandler)) {
    RMutexLock(&object->mutex);
    $(object->threads, m(join, RThreadPool)));
    RMutexUnlock(&object->mutex);
}

method(void, terminate,  RTCPHandler)) {
    if(!object->terminateFlag) {
        object->terminateFlag = yes;
        RMutexLock(&object->mutex);
        if(!object->connectorMode) {
            deleter(object->listener, RSocket);
        }
        $(object->threads,   m(cancel, RThreadPool)));
        $(object->arguments, m(flush, RArray)));
        RMutexUnlock(&object->mutex);
    } elseWarning(
            RWarning("RTCPHandler. terminate. Nothing to terminate.", object)
    );
}

method(void, multicast, RTCPHandler), REnumerateDelegate *predicate, const pointer buffer, size_t size) {
    RMutexLock(&object->mutex);
    RArray *resultGroup = object->arguments;  // broadcast
    if(predicate != nil && predicate->virtualEnumerator != nil) { // multicast
        resultGroup = $(object->arguments, m(subarrayWithPredicate, RArray)), predicate);
    }
    object->multicastEnumerator.context = makeRData(buffer, size, RDataTypeASCII);
    $(resultGroup, m(enumerate, RArray)), &object->multicastEnumerator, yes);

    if(resultGroup != object->arguments) {
        resultGroup->destructorDelegate = nil; // do not delete data
        deleter(resultGroup, RArray);
    }

    deallocator(object->multicastEnumerator.context);
    object->multicastEnumerator.context = nil;
    RMutexUnlock(&object->mutex);
}

inline
method(void, broadcast, RTCPHandler), RString *string) {
    $(object, m(multicast, RTCPHandler)), nil, string->data, string->size);
}

#endif /* RAY_EMBEDDED */