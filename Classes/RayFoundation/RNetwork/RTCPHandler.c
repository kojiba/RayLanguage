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

rbool RTCPHandlerInactiveContextDeleter(pointer context, pointer object, size_t iterator) {
    if(((RTCPDataStruct*)object)->socket == nil) {
        return yes;
    }
    return no;
}

void PrivateArgPrinter(pointer object){
    RPrintf("%p : socket : %p\n", object, ((RTCPDataStruct*)object)->socket);
}

void PrivateArgDeleter(RTCPDataStruct* object) {
    nilDeleter(object->socket, RSocket);
    RFree(object);
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

                    $(object->arguments, m(setPrinterDelegate,    RArray)), PrivateArgPrinter);
                    $(object->arguments, m(setDestructorDelegate, RArray)), (DestructorDelegate) PrivateArgDeleter);
                    // init predicate
                    object->predicate.virtualEnumerator = RTCPHandlerInactiveContextDeleter;
                    object->predicate.context           = object;
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
    RPrintf("\tRunning thread tuid %u\n", (unsigned) RThreadIdOfThread(&object->runningThread));
    p(RThreadPool)(object->threads);
    RPrintf("------ Arguments ------");
    p(RArray)(object->arguments);
    RPrintf("------ Arguments ------\n");
    RPrintf("RTCPHandler %p -----------\n\n", object);
}

method(void, startOnPort, RTCPHandler), uint16_t port, pointer context) {
    if(object->delegate != nil
            && object->delegate->delegateFunction != nil) {
        if($(object->listener, m(bindPort, RSocket)), port) == yes) {

            object->runningThread = currentThread();
            object->terminateFlag = no;
            $(object->threads,  m(setDelegateFunction, RThreadPool)), object->delegate->delegateFunction);
            $(object->listener, m(listen,              RSocket)),     RTCPHandlerListenerQueueSize);

            while(!object->terminateFlag) {
                RTCPDataStruct *argument = allocator(RTCPDataStruct);
                if(argument != nil) {

                    argument->handler  = object;
                    argument->delegate = object->delegate;
                    argument->context  = context;
                    argument->socket   = $(object->listener, m(accept, RSocket)));

                    if(argument->socket != nil) {
                        RPrintf("%p arg\n", argument);
                        $(object->arguments, m(addObject,  RArray)),      argument);

                        if(object->arguments->count != 0
                           && (object->arguments->count % RTCPHandlerCheckCleanupAfter) == 0) {
                            $(object->arguments, m(deleteWithPredicate, RArray)), &object->predicate);
                        }
                        $(object->threads,   m(addWithArg, RThreadPool)), argument, yes);
                    } else {
                        deallocator(argument);
                    }
                } elseError(
                        RError("RTCPHandler. Can't allocate thread argument.", object)
                );
            }
        } elseError(
            RError1("RTCPHandler. startOnPort. Bind error, port %u", object, port)
        );
    } elseWarning(
            RWarning("RTCPHandler. startOnPort. Delegate or delgate function is nil. What do you want to start?!", object)
    );
}

method(void, terminate,  RTCPHandler)) {
    RThreadCancel(&object->runningThread);
    $(object->threads, m(cancel, RThreadPool)));
    $(object->arguments, m(flush, RArray)));
}