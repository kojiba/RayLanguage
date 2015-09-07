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
        object->listener = makeRSocket(nil, SOCK_STREAM, IPPROTO_TCP);
        if(object->listener != nil) {
            object->threads = c(RThreadPool)(nil);
            if(object->threads != nil) {
                object->arguments = makeRArray();
                if(object->arguments != nil) {

                    mutexWithType(&object->mutex, RMutexNormal);

                    $(object->arguments, m(setPrinterDelegate,    RArray)), (PrinterDelegate)    PrivateArgPrinter);
                    $(object->arguments, m(setDestructorDelegate, RArray)), (DestructorDelegate) PrivateArgDeleter);

                    // init destructorPredicate and enumerator
                    object->destructorPredicate.virtualEnumerator = RTCPHandlerInactiveContextDeleter;
                    object->destructorPredicate.context           = object;
                    object->multicastEnumerator.virtualEnumerator = (EnumeratorDelegate) RTCPHandlerMulticastEnumerator;
                }
            }
        }
    }
    return object;
}

destructor(RTCPHandler) {
    deleter(object->listener,  RSocket);
    deleter(object->threads,   RThreadPool);
    deleter(object->arguments, RArray);
}

getterImpl(delegate, RTCPDelegate *, RTCPHandler)
setterImpl(delegate, RTCPDelegate *, RTCPHandler)

printer(RTCPHandler) {
    RPrintf("RTCPHandler %p -----------\n", object);
#ifndef _WIN32
    RPrintf("\tRunning thread tuid %u\n", (unsigned) RThreadIdOfThread(&object->runningThread));
#endif
    RPrintf("------ Arguments ------");
    p(RArray)(object->arguments);
    RPrintf("RTCPHandler %p -----------\n\n", object);
}

method(void, privateStartOnPort, RTCPHandler)) {
    if($(object->listener, m(bindPort, RSocket)), object->listener->port) == yes) {

        object->terminateFlag = no;
        $(object->threads,  m(setDelegateFunction, RThreadPool)), object->delegate->delegateFunction);
        $(object->listener, m(listen,              RSocket)),     RTCPHandlerListenerQueueSize);

        while(!object->terminateFlag) {
            RSocket *socket = $(object->listener, m(accept, RSocket)));
            if(socket != nil) {
                RTCPDataStruct *argument = allocator(RTCPDataStruct);
                if(argument != nil) {
                    argument->handler    = object;
                    argument->delegate   = object->delegate;
                    argument->context    = object->delegate->context;
                    argument->socket     = socket;
                    argument->identifier = object->arguments->count;

                    $(object->arguments, m(addObject,  RArray)), argument);

                    // delete inactive worker arguments
                    if(object->arguments->count != 0
                       && (object->arguments->count % RTCPHandlerCheckCleanupAfter) == 0) {
                        $(object->arguments, m(deleteWithPredicate, RArray)), &object->destructorPredicate);
                    }

                    // finally, add new worker with auto-cleanup
                    $(object->threads, m(addWithArg, RThreadPool)), argument, yes);
                } elseError(
                        RError("RTCPHandler. Can't allocate thread argument.", object)
                );
            }
        }
    } elseError(
            RError1("RTCPHandler. startOnPort. Bind error, port %u", object, object->listener->port)
    );
}

method(void, startOnPort, RTCPHandler), uint16_t port) {
    if(object->delegate != nil
            && object->delegate->delegateFunction != nil) {
        $(object->listener, m(setPort, RSocket)), port);
        RThreadCreate(&object->runningThread, nil, (RThreadFunction) m(privateStartOnPort, RTCPHandler), object);
    } elseWarning(
            RWarning("RTCPHandler. startOnPort. Delegate or delgate function is nil. What do you want to start?!", object)
    );
}

method(void, terminate,  RTCPHandler)) {
    object->terminateFlag = yes;
//    RThreadCancel(&object->runningThread);
    $(object->threads,   m(cancel, RThreadPool)));
    $(object->arguments, m(flush, RArray)));
}

method(void, multicast, RTCPHandler), REnumerateDelegate *predicate, const pointer buffer, size_t size) {
    RMutexLock(&object->mutex);
    RArray *resultGroup = object->arguments;  // broadcast
    if(predicate != nil && predicate->virtualEnumerator != nil) { // multicast
        resultGroup = $(object->arguments, m(subarrayWithPredicate, RArray)), predicate);
    }
    object->multicastEnumerator.context = RCStringInit(buffer, size);
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
    $(object, m(multicast, RTCPHandler)), nil, string->baseString, string->size);
}

#endif /* RAY_EMBEDDED */